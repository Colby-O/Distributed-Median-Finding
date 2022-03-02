#ifndef PARNET_H_
#define PARNET_H_

#include "globals.h"

typedef struct {
	int read_pipes[NUM_PIPES / 2];
	int write_pipes[NUM_PIPES / 2];
	int total_num_data;
	int k;
} parent_t;

void parent_get_number_of_data(parent_t* parent);
int parent_main(void);
void parent_emit_process_id(parent_t* parent);
int parent_await_children_ready(parent_t* parent);
int parent_send_request_request(parent_t* parent, int child_id);
int parent_send_pivot_request(parent_t* parent, int pivot);
int parent_respond_large_or_small(parent_t* parent, int pivot);
void parent_request_termination(parent_t* parent);

#endif
