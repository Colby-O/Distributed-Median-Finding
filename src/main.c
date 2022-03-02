#include "globals.h"
#include "child.h"
#include "parent.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int fds[NUM_PIPES][2];

void spawn_children(int nchildren) {
    pid_t pid;
    if(nchildren > 0) {
		if((pid = fork()) < 0) {
			perror("error occured while forking.");
		}
		else if(pid == 0) {
			child_main(child_init(nchildren));
		}
		else if(pid > 0) {
			spawn_children(nchildren - 1);
		}	
	}
}

int main(void) {
	srand(time(0));

	for(int i = 0; i < NUM_PIPES; ++i) {
		pipe(fds[i]);
	}	

	spawn_children(NUM_CHILDREN);
	
	int median = parent_main();	

	while(wait(NULL) > 0);
	
	printf("--- Median is %d!\n", median);

	return EXIT_SUCCESS;
}
