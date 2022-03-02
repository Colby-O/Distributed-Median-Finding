#include "child.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>

void child_read(child_t* child, void* value, size_t size) {
	if (read(child->read_pipe, value, size) <= 0) {
		perror("child failed to read pipe");
		exit(EXIT_FAILURE);
	}
}

void child_write(child_t* child, void* value, size_t size) {
	if (write(child->write_pipe, value, size) <= 0) {
		perror("child failed to write to pipe");
		exit(EXIT_FAILURE);
	}	
}

void child_get_data(child_t* child) {
	char filename[128];
	sprintf(filename, "data/input_%d.txt", child->id - 1);

	FILE* file = fopen(filename, "r");

	int temp[1024];
	int count = 0;

	while(!feof(file)) {
		fscanf(file, "%d", &temp[count]);
		count++;
	}
	
	int* result = malloc(count*sizeof(int));
	
	for (int i = 0; i < count; ++i) {
		result[i] = temp[i];
	}
	child->num_data = count - 1;
	child->data = result;
}

void child_get_id(child_t* child) {
	child_read(child, &child->id, sizeof(child->id));
}

child_t child_init(int pipeIndex) {
	int read_pipe_index = pipeIndex - 1;
	int write_pipe_index = (pipeIndex - 1) + (NUM_PIPES / 2);
	close(fds[read_pipe_index][1]);
	close(fds[write_pipe_index][0]);

	child_t child;
	child.write_pipe = fds[write_pipe_index][1];
	child.read_pipe = fds[read_pipe_index][0];
	child_get_id(&child);
	child_get_data(&child);

	return child;
}

void child_send_ready_state(child_t* child) {
	int request = READY;
	printf("--- Child %d sends READY\n", child->id);	
	child_write(child, &request, sizeof(request));	
}

int child_read_request(child_t* child) {
	int response;
	child_read(child, &response, sizeof(response));
	return response;
}

int child_get_greater_than_pivot_count(child_t* child) {
	int count = 0;
	for (int i = 0; i < child->num_data; ++i) {
		if(child->data[i] > child->current_pivot) {
			count++;
		}
	}
        return count;	
}

void child_send_number_of_data(child_t* child) {
	child_write(child, &child->num_data, sizeof(child->num_data));
}

bool child_has_vaild_data(child_t* child) {
	bool has_vaild_data = false;
	for (int i = 0; i < child->num_data; ++i) {
		if (child->data[i] > 0) {
			has_vaild_data = true;
			break;
		}
	}
	return has_vaild_data;
}

void child_handle_request_request(child_t* child) {
	int data = -1;

	if(child_has_vaild_data(child)) {
		do {
			int index = rand() % child->num_data;
			data = child->data[index];
		} while (data <= 0);
	}

	printf("--- Child %d sends %d to parent\n", child->id, data);
	child_write(child, &data, sizeof(data));
}

void child_handle_pivot_request(child_t* child) {
	child_read(child, &child->current_pivot, sizeof(child->current_pivot));

	int count = child_get_greater_than_pivot_count(child);
	printf("--- Child %d receives pivot and replies %d\n", child->id, count);
	child_write(child, &count, sizeof(count));
}

void child_handle_large_request(child_t* child) {
	for (int i = 0; i < child->num_data; ++i) {
		if (child->data[i] > child->current_pivot) {
			child->data[i] = -1;
		}
	}
}

void child_handle_small_request(child_t* child) {
	for (int i = 0; i < child->num_data; ++i) {
		if (child->data[i] < child->current_pivot) {
			child->data[i] = -1;
		}
	}	
}

void child_handle_request(child_t* child, int request) {
	switch (request) {
		case READY: 
			break;
		case REQUEST:
			child_handle_request_request(child);
			break;
		case PIVOT: 
			child_handle_pivot_request(child);		
			break;
		case LARGE: 
			child_handle_large_request(child);
			break;
		case SMALL: 
			child_handle_small_request(child);
			break;
		case NUMBER:
			child_send_number_of_data(child);
			break;
		default:
			break;
	}
}

void child_main(child_t child) {
	int request;	
	child_send_ready_state(&child);
	do {
		request = child_read_request(&child);
		child_handle_request(&child, request);
	} while(request != TERMINATE);

	printf("--- Child %d terminates\n", child.id);
	exit(EXIT_SUCCESS);
}
