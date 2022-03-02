#include "parent.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

void parent_get_number_of_data(parent_t* parent) {
	int total_num_data = 0;
	for (int i =0; i < NUM_CHILDREN; ++i) {
		int request = NUMBER;
		write(parent->write_pipes[i], &request, sizeof(request));

		int num_data;
		read(parent->read_pipes[i], &num_data, sizeof(num_data));
		total_num_data += num_data;
	}

	parent->total_num_data = total_num_data;
	parent->k = total_num_data / 2;
}

parent_t parent_init(void) {
	parent_t parent;
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		int write_pipe_index = i;
		int read_pipe_index = (NUM_PIPES / 2) + i;
		close(fds[write_pipe_index][0]);
		close(fds[read_pipe_index][1]);

		parent.read_pipes[i] = fds[read_pipe_index][0];
		parent.write_pipes[i] = fds[write_pipe_index][1];
	}

	return parent;
}


void parent_emit_process_id(parent_t* parent) {
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		int id = i + 1;
		write(parent->write_pipes[i], &id, sizeof(id));
	}
}

int parent_await_children_ready(parent_t* parent) {
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		int response;
		read(parent->read_pipes[i], &response, sizeof(response));

		if(response != READY) {
			perror("not all child return status ready");
			return ERROR;  
		}
	}
	return SUCCESS;	
}

int parent_send_request_request(parent_t* parent, int child_id) {
		int request = REQUEST;
		int response;
		printf("--- Parent sends REQUEST to Child %d\n", child_id);
		write(parent->write_pipes[child_id - 1], &request, sizeof(request));
		read(parent->read_pipes[child_id - 1], &response, sizeof(response));
		return response;

}

int parent_respond_large_or_small(parent_t* parent, int pivot) {
	int median = 0;
	int sum = 0;
	printf("--- Parent: m=");
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		int count;
		read(parent->read_pipes[i], &count, sizeof(count));
		printf((i != NUM_CHILDREN - 1) ? "%d+" : "%d", count);
		sum += count;
	}
	printf("=%d. %d=%d/2. ", sum, sum, parent->total_num_data);
	
	int k = parent->k;
	
	if (sum != k) {
		printf("Median not found!\n");
		int request = (k > sum) ? LARGE : SMALL;
		if(request == LARGE) parent->k -= sum;
		for (int i = 0; i < NUM_CHILDREN; ++i) {
			write(parent->write_pipes[i], &request, sizeof(request));
		}
	}
	else {
		printf("Median found!\n");
		median = pivot;
	}	

	return median;
}

int parent_send_pivot_request(parent_t* parent, int pivot) {
	int code = PIVOT;
	printf("--- Parnet broadcasts pivot %d to all children\n", pivot);
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		write(parent->write_pipes[i], &code, sizeof(code));
	}

	for (int i = 0; i < NUM_CHILDREN; ++i) {
		write(parent->write_pipes[i], &pivot, sizeof(pivot));
	}

	int median = parent_respond_large_or_small(parent, pivot);

	return median;
}

void parent_request_termination(parent_t* parent) {
	for (int i = 0; i < NUM_CHILDREN; ++i) {
		int request = TERMINATE;
		write(parent->write_pipes[i], &request, sizeof(request));
	}
}

int parent_main(void) {
	int median = 0;
	parent_t parent = parent_init();	
	parent_emit_process_id(&parent);
	
	if (parent_await_children_ready(&parent)) {
		return ERROR;
	}

	parent_get_number_of_data(&parent);

	while(!median) {
		int child_id = rand() % NUM_CHILDREN + 1;
		
		int pivot = parent_send_request_request(&parent, child_id);
		if (pivot > 0) {
			median = parent_send_pivot_request(&parent, pivot);
		}
	}
	
	printf("--- Parent sends kill singals to all children\n");
	parent_request_termination(&parent);

	return median;

}
