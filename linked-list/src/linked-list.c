/* 
 * Filename:	linked-list.c
 * Author:		Jess Turner
 * Date:		10/10/19
 * Licence:		GNU GPL V3
 *
 * Library for a fully generic and thread-safe singly linked list
 *
 * Return/exit codes:
 *		LIST_OK			- No error
 *		SIZE_ERROR		- list size error (invalid block size or number of datas)
 *		MEM_ERROR		- Memory allocation error
 *		INDEX_ERROR		- Couldn't pop data from the list
 *
 * All functions returning pointers will return NULL on memory allocation faliure, else they will return an error for the user to handle
 *
 * Todo:
 *		- Add secure versions of llist_destroy(), llist_pop(), and llist_remove() to overwrite memory blocks that are no longer in use
 *		- Add a parameter to llist_init() containing a function pointer detailing how to delete the data stored in each node
 */

#include "../include/linked-list.h"

#include <string.h>
#include <pthread.h>
#include <errno.h>

int llist_init(llist_t * list, size_t data_width)
{
	if(data_width <= 0)
		return SIZE_ERROR;
	
	list->tail			= NULL;
	list->head			= NULL;
	list->data_width	= data_width;
	list->length		= 0;

	pthread_mutex_init(&list->lock, NULL);

	return LIST_OK;
}

void llist_destroy(llist_t * list)
{
	if(list == NULL)
		return;

	while(list->head) {
		llist_element_t * temp = list->head->next;
		free(list->head);
		list->head = temp;
	}
	
	list->data_width	= 0;
	list->tail			= NULL;
	list->length		= 0;

	pthread_mutex_destroy(&list->lock);

	return;
}

int llist_push(void const * const data, llist_t * list)
{
	llist_element_t * new_element;

	if(!(new_element = malloc(sizeof(llist_element_t))) || !(new_element->data = malloc(list->data_width))) {
		if(new_element)
			free(new_element);
		return MEM_ERROR;
	}

	memcpy(new_element->data, data, list->data_width);

	if(list->head == NULL)
		list->head = new_element;
	else
		list->tail->next = new_element;

	list->tail = new_element;
	list->tail->next = NULL;

	list->length++;

	return LIST_OK;
}

int llist_pop(void * const data, llist_t * list)
{
	if(list->head == NULL)
		return INDEX_ERROR;

	memcpy(data, list->head->data, list->data_width);

	free(list->head->data);
	llist_element_t * temp = list->head;
	list->head = list->head->next;
	free(temp);
	list->length--;

	return LIST_OK;
}

int llist_remove(llist_element_t * element, llist_t * list)
{
	if(element == NULL || list->head == NULL)
		return INDEX_ERROR;

	if(list->head == element) {
		list->head = list->head->next;
		return LIST_OK;
	}

	llist_element_t * prev = NULL;
	llist_element_t * curr = list->head;

	while(curr != element) {
		prev = curr;
		curr = curr->next;

		if(curr == NULL)
			return INDEX_ERROR;
	}

	prev->next = curr->next;
	list->length--;

	return LIST_OK;
}

int	llist_insert_after(void const * const data, llist_element_t * element, llist_t * list)
{
	if(list->head == NULL)
		return llist_push(data, list);

	llist_element_t * new_element;

	if(!(new_element = malloc(sizeof(llist_element_t))) || !(new_element->data = malloc(list->data_width))) {
		if(new_element)
			free(new_element);
		return MEM_ERROR;
	}

	memcpy(new_element->data, data, list->data_width);

	new_element->next = element->next;
	element->next = new_element;

	if(element == list->tail)
		list->tail = new_element;

	list->length++;

	return LIST_OK;
}

int	llist_insert_before(void const * const data, llist_element_t * element, llist_t * list)
{
	if(list->head == NULL)
		return llist_push(data, list);

	llist_element_t * prev = list->head;
	llist_element_t * curr = prev->next;

	while(curr != NULL && curr != element) {
		curr = curr->next;
		prev = prev->next;
	}

	if(curr == NULL)
		return INDEX_ERROR;

	llist_element_t * new_element;

	if(!(new_element = malloc(sizeof(llist_element_t))) || !(new_element->data = malloc(list->data_width))) {
		if(new_element)
			free(new_element);
		return MEM_ERROR;
	}

	memcpy(new_element->data, data, list->data_width);

	if(curr == list->head) {
		new_element->next = curr;
		list->head = new_element;
	} else {
		new_element->next = prev->next;
		prev->next = new_element;
	}

	list->length++;

	return LIST_OK;
}

llist_element_t * llist_search(void const * const data, int (*compare)(const void * first_element, const void * second_element), llist_t * list)
{
	llist_element_t * curr;

	for(curr = list->head; curr != NULL; curr = curr->next)
		if(!(*compare)(curr->data, data))
			return curr;

	return NULL;
}

void * llist_peek(void * const data, llist_t * list)
{
	if(list->head == NULL)
		return NULL;

	if(data)
		memcpy(data, list->head->data, list->data_width);

	return list->head->data;
}

void * llist_peek_tail(void * const data, llist_t * list)
{
	if(list->tail == NULL)
		return NULL;

	if(data)
		memcpy(data, list->tail->data, list->data_width);

	return list->tail->data;
}

static void llist_sort_split(llist_element_t * source, llist_element_t ** front, llist_element_t ** back)
{
	llist_element_t * slow = source;
	llist_element_t * fast = source->next;
 
	while(fast != NULL) {
		fast = fast->next;
		if(fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	*front = source;
	*back = slow->next;
	slow->next = NULL;

	return;
}

static llist_element_t * llist_merge_sorted(int (*compare)(const void * first_element, const void * second_element), llist_element_t * head, llist_element_t * second_head)
{
	llist_element_t * result = NULL;

	if(head == NULL)
		return second_head;
	else if(second_head == NULL)
		return head;
	 
	if(compare(head->data, second_head->data) < 0) {
		result = head;
		result->next = llist_merge_sorted(compare, head->next, second_head);
	} else {
		result = second_head;
		result->next = llist_merge_sorted(compare, head, second_head->next);
	}

	return result;
}

static void llist_sort_internal(int (*compare)(const void * first_element, const void * second_element), llist_element_t ** head)
{
	llist_element_t * back = NULL;
	llist_element_t * front = NULL;

	if(*head == NULL || (*head)->next == NULL)
		return;

	llist_sort_split(*head, &front, &back);

	llist_sort_internal(compare, &front);
	llist_sort_internal(compare, &back);

	*head = llist_merge_sorted(compare, front, back);

	return;
}

int llist_sort(int (*compare)(const void * first_element, const void * second_element), llist_t * list)
{
	if(list->head == NULL)
		return INDEX_ERROR;

	llist_sort_internal(compare, &list->head);

	for(llist_element_t * curr = list->head; curr != NULL; curr = curr->next)
		if(curr->next == NULL)
			list->tail = curr;

	return LIST_OK;
}

void llist_operate(void(*operation)(const void * data, const void * parameter), const void * parameter, llist_t * list)
{
	for(llist_element_t * curr = list->head; curr != NULL; curr = curr->next)
		operation(curr->data, parameter);

	return;
}
