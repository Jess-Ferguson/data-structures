#ifndef STACK_H
#define STACK_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define STACK_OK 0
#define MEM_ERROR -1
#define SIZE_ERROR -2

#define BASE_STACK_LENGTH (2 << 10)

typedef struct stack {
	pthread_mutex_t lock;
	size_t length;
	size_t element_width;
	size_t allocated_blocks;
	void * stack_base;
} stack_t;

int stack_init(stack_t * stack, size_t element_width);
int stack_push(stack_t * stack, void * data);
int stack_pop(stack_t * stack, void * location);
void stack_destroy(stack_t * stack);

#endif