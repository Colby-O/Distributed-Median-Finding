CC = gcc
CHILD_COUNT = 5
CFLAGS = -O3 -Wall -Wno-unused-result -I ./include -DCHILD_COUNT=${CHILD_COUNT}

TARGET = findmedian
SRC_DIR = src
OBJ_DIR = build

CSRCS = $(shell find ./src/  -type f -name '*.c' -printf "%f\n")
OBJS = $(addprefix ${OBJ_DIR}/, $(CSRCS:.c=.o))

all: ${TARGET} clean

${TARGET}: ${OBJ_DIR} ${OBJS}
	$(CC) -o ${TARGET} ${OBJS} ${CFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	$(CC) -c $< -o $@ ${CFLAGS}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

clean:
	rm -rf ${OBJ_DIR}

	
	
