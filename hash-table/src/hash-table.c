/* 
 * Filename:	hash-table.c
 * Author:		Jess Turner
 * Date:		24/6/20
 * Licence:		GNU GPL V3
 *
 * Library for a fully generic hash table
 *
 * Return/exit codes:
 *		TABLE_OK		- The operation completed successfuly
 *		MEM_ERROR		- Memory allocation error
 *		INVALID_ENTRY	- The referenced entry does not exist in the table
 *
 *	Future:
 *		- Do more comprehensive testing to check the table works under high loads and in edge cases
 *		- Add thread saftey
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/hash-table.h"

typedef struct entry_t {
	void * data;			/* The value corresponding to the key */
	char * name;			/* The full name of the key, stored to avoid issues with hash collisions */
	struct entry_t * next;	/* The next entry in the current bucket */
} entry_t;

typedef struct table_t {
	entry_t ** buckets;		/* The list of all current buckets */
	size_t bucket_count;	/* The number of buckets in the table */
	size_t entry_width;		/* The size of each value in the table */
} table_t;

static inline int new_entry(table_t * table, entry_t * cur_entry, char * name, void * data)
{
	if(!(cur_entry->name = malloc(strlen(name) + 1)) || !(cur_entry->data = malloc(table->entry_width))) {
		free(cur_entry->name);
		free(cur_entry);
		return MEM_ERROR;
	}

	strcpy(cur_entry->name, name);
	memcpy(cur_entry->data, data, table->entry_width);

	cur_entry->next = NULL;

	return TABLE_OK;
}

static inline void update_entry(table_t * table, entry_t * cur_entry, void * data)
{
	memcpy(cur_entry->data, data, table->entry_width);
}

static inline void delete_entry(table_t * table, entry_t * cur_entry)
{
	free(cur_entry->name);
	// TODO: Run user provided function to delete generic data stored in the entry 
	free(cur_entry->data);
}

static inline size_t hashstring(unsigned char *str)
{
	size_t hash = 5381;
	int c;

	while((c = *str++)) /* DJB hash algorithm */
		hash = ((hash << 5) + hash) + c;

	return hash;
}

static inline size_t get_key(table_t * table, char * entry_name)
{
	return hashstring((unsigned char *) entry_name) % table->bucket_count;
}

int htinit(table_t * table, size_t entry_width, size_t bucket_count)
{
	if(!(table->buckets = calloc(bucket_count, sizeof(entry_t *))))
		return MEM_ERROR;

	table->bucket_count = bucket_count;
	table->entry_width = entry_width;

	return TABLE_OK;
}

int htinsert(table_t * table, char * entry_name, void * data)
{
	size_t bucket = get_key(table, entry_name);
	entry_t * temp;

	if(table->buckets[bucket]) {
		entry_t * cur_entry;

		for(cur_entry = table->buckets[bucket]; cur_entry->next; cur_entry = cur_entry->next) {
			if(!strcmp(cur_entry->name, entry_name)) {
				update_entry(table, cur_entry, data);
				return TABLE_OK;
			}
		}

		if(!(temp = malloc(sizeof(entry_t))))
			return MEM_ERROR;

		if(new_entry(table, temp, entry_name, data) != TABLE_OK)
			return MEM_ERROR;

		cur_entry->next = temp;
	} else {
		if(!(temp = malloc(sizeof(entry_t))))
			return MEM_ERROR;

		if(new_entry(table, temp, entry_name, data) != TABLE_OK)
			return MEM_ERROR;

		table->buckets[bucket] = temp;
	}

	return TABLE_OK;
}

int htlookup(table_t * table, char * entry_name, void * value)
{
	size_t bucket = get_key(table, entry_name);

	for(entry_t * cur_entry = table->buckets[bucket]; cur_entry; cur_entry = cur_entry->next) {
		if(!strcmp(cur_entry->name, entry_name)) {
			if(value)
				memcpy(value, cur_entry->data, table->entry_width);
			return TABLE_OK;
		}
	}

	return INVALID_ENTRY;
}

int htdelete(table_t * table, char * entry_name)
{
	size_t bucket = get_key(table, entry_name);
	entry_t * cur_entry = table->buckets[bucket];
	entry_t * prev_entry = NULL;

	while(strcmp(cur_entry->name, entry_name)) {
		prev_entry = cur_entry;
		cur_entry = cur_entry->next;

		if(!cur_entry)
			return INVALID_ENTRY;
	}

	if(!prev_entry)
		table->buckets[bucket] = cur_entry->next;
	else
		prev_entry->next = cur_entry->next;
		
	delete_entry(table, cur_entry);
	free(cur_entry);

	return TABLE_OK;
}

void htdestroy(table_t * table)
{
	for(size_t i = 0; i < table->bucket_count; i++) {
		entry_t * cur_entry = table->buckets[i];

		while(cur_entry) {
			entry_t * temp = cur_entry->next;
			delete_entry(table, cur_entry);
			free(cur_entry);
			cur_entry = temp;
		}
	}

	free(table->buckets);
}