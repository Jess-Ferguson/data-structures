#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <pthread.h>

/* Return values */

#define LIST_OK 0
#define MEM_ERROR -1													/* Memory allocation error */
#define SIZE_ERROR -2													/* list dimension error */
#define INDEX_ERROR -3													/* No data at given index */

/* List element data structure */

typedef struct list_element_t
{
	void * data;														/* Contains the data stored at this node */
	struct list_element_t * next;										/* Contains the pointer to the next element, or NULL if it's the tail node */
} llist_element_t;

/* List master data structure */

typedef struct
{
	int 				length;											/* The running total length of the list */
	llist_element_t *	head;											/* Pointer to the head of the list */
	llist_element_t *	tail;											/* Pointer to the tail of the list */
	pthread_mutex_t		lock;											/* Read/write lock to ensure list concurrency */
	size_t				data_width;										/* The size of each element in the list */
} llist_t;

/* Interface Functions */

llist_element_t * llist_search(void const * const data, int (*compare)(const void * first_element, const void * second_element), llist_t * list); /* Search the list for an occurance of a given data value using a user defined comparison function */
int llist_init(llist_t * list, size_t data_size);															/* Initialise the list data structure */
int llist_insert_before(void const * const data, llist_element_t * element, llist_t * list);				/* Insert an element into the list at the position before a specified element */
int llist_insert_after(void const * const data, llist_element_t * element, llist_t * list);					/* Insert an element into the list at the position after a specified element */
int llist_pop(void * const data, llist_t * list);															/* Pop an element from the front of the list, deals with cleanup when the head node is empty */
int llist_push(void const * const data, llist_t * list);													/* Push an element to the back of the list, creates a new block when tail node is full */
int llist_remove(llist_element_t * element, llist_t * list);												/* Remove an element from the list and connect the two elements next to it */
int llist_sort(int (*compare)(const void * first_element, const void * second_element), llist_t * list);	/* Sort all elements in the list using a merge sort */
void llist_destroy(llist_t * list);																			/* Destroy the list data structure and any associated nodes */
void llist_operate(void (*operation)(const void * data, const void * parameter), const void * parameter, llist_t * list);	/* Perform a user defined action on every single element stored in the list */
void * llist_peek(void * const data, llist_t * list);														/* Check the contents of the element at the head of the list without popping the list */
void * llist_peek_tail(void * const data, llist_t * list);													/* Check the contents of the element at the tail of the list */

#endif
