#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_OK		0	/* Operation completed successfully */
#define MEM_ERROR		-1	/* Memory allocation error */
#define INVALID_ENTRY	-2	/* Key has no corresponding value in the table */

#define DEFAULT_TABLE_SIZE (size_t) 1024

typedef struct table_t table_t;

int htinit(table_t * table, size_t entry_width, size_t bucket_count); 	/* Initialise the table data structure */
int htinsert(table_t * table, char * entry_name, void * data); 			/* Insert an entry into the table */
int htlookup(table_t * table, char * entry_name, void * value);			/* Check if a given key is valid and place the corresponding value into value. If value is NULL it will simply check if the value exists. */
int htdelete(table_t * table, char * entry_name);						/* Delete a key and value from the table */
void htdestroy(table_t * table);										/* Destroy the table and table metadata */

#endif