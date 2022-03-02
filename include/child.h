#ifndef CHILD_H_
#define CHILD_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct {
	int write_pipe, read_pipe;
	int id;	
	int* data;
	int num_data;
	int current_pivot;
} child_t;

void child_read(child_t* child, void* value, size_t size);
void child_write(child_t* child, void* value, size_t size);
child_t child_init(int pipeIndex);
void child_main(child_t child);
void child_get_data(child_t* child);
void child_get_id(child_t* child);
void child_send_ready_state(child_t* child);
int child_read_request(child_t* child);
void child_send_number_of_data(child_t* child);
bool child_has_vaild_data(child_t* child);
void child_handle_request(child_t* child, int request);
void child_handle_request_request(child_t* child);
void child_handle_pivot_request(child_t* child);
void child_handle_large_request(child_t* child);
void child_handle_small_request(child_t* child);
int child_get_greater_than_pivot_count(child_t* child);

#endif
