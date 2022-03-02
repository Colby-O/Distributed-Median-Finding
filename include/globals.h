#ifndef GLOBALS_H_
#define GLOBALS_H_

#define NUM_CHILDREN CHILD_COUNT
#define NUM_PIPES NUM_CHILDREN * 2

#define SUCCESS 0
#define ERROR 1

#define ABS(num) ((num > 0) ? num : -num) 

/* Communication Codes */
#define EMPTY -1
#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500
#define NUMBER 600
#define TERMINATE 700

extern int fds[NUM_PIPES][2];

#endif
