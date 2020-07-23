#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_OK		0	/* Operation completed successfully */
#define MEM_ERROR		-1	/* Memory allocation error */
#define INVALID_ENTRY	-2	/* Key has no corresponding value in the table */

#define DEFAULT_TABLE_SIZE (size_t) 1024

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

int htinit(table_t * table, size_t entry_width, size_t bucket_count); 	/* Initialise the table data structure */
int htinsert(table_t * table, char * entry_name, void * data); 			/* Insert an entry into the table */
int htlookup(table_t * table, char * entry_name, void * value);			/* Check if a given key is valid and place the corresponding value into value. If value is NULL it will simply check if the value exists. */
int htdelete(table_t * table, char * entry_name);						/* Delete a key and value from the table */
void htdestroy(table_t * table);										/* Destroy the table and table metadata */

#endif