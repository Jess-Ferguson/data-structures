/* 
 * Filename:	stack.c
 * Author:		Jess Turner
 * Date:		10/10/19
 * Licence:		GNU GPL V3
 *
 * Library for a fully generic and thread-safe stack
 *
 * Return/exit codes:
 *		STACK_OK		- No error
 *		SIZE_ERROR		- Stack empty or invalid element width
 *		MEM_ERROR		- Memory allocation error
 *
 * Todo:
 *		- Add more stack operation functions (search, operate, sort, etc.)
 */

#include <pthread.h>
#include <stdlib.h>

#include "../include/stack.h"

int stack_init(stack_t * stack, size_t element_width)
{
	if(element_width <= 0)
		return SIZE_ERROR;

	if(!(stack->stack_base = malloc(element_width * BASE_STACK_LENGTH)))
		return MEM_ERROR;

	stack->length = 0;
	stack->element_width = element_width;
	stack->allocated_blocks = BASE_STACK_LENGTH;

	pthread_mutex_init(&stack->lock, NULL);

	return STACK_OK;
}

int stack_push(stack_t * stack, void * data)
{
	if(stack->length == stack->allocated_blocks) {
		stack->allocated_blocks <<= 1;

		if(!(stack->stack_base = realloc(stack->stack_base, stack->allocated_blocks * stack->element_width)))
			return MEM_ERROR;
	}

	memcpy(((char *) stack->stack_base) + stack->length * stack->element_width, data, stack->element_width);

	stack->length++;

	return STACK_OK;
}

int stack_pop(stack_t * stack, void * location)
{
	if(stack->length == 0)
		return SIZE_ERROR;

	if(stack->length == stack->allocated_blocks >> 1) {
		stack->allocated_blocks >>= 1;

		if(!(stack->stack_base = realloc(stack->stack_base, stack->allocated_blocks * stack->element_width)))
			return MEM_ERROR;
	}

	stack->length--;

	memcpy(location, ((char *) stack->stack_base) + stack->length * stack->element_width, stack->element_width);

	return STACK_OK;
}

void stack_destroy(stack_t * stack)
{
	stack->length = 0;
	stack->element_width = 0;
	free(stack->stack_base);

	pthread_mutex_destroy(&stack->lock);
}