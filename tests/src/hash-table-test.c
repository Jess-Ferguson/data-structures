#include <stdio.h>

#include "../include/hash-table.h"

int main()
{
	table_t my_hash_table;
	int data[] = { 12, 432, 62, 145 };
	int data_out;
	char * entries[] = { "foo", "bar", "baz", "qux" };

	printf("[+] Generating table...\n");

	if(htinit(&my_hash_table, sizeof(int), DEFAULT_TABLE_SIZE) != TABLE_OK) {
		fprintf(stderr, "Error: Could not create table!\n");
		return MEM_ERROR;
	}

	printf("[+] Inserting values...\n");

	for(int i = 0; i < sizeof(entries) / sizeof(entries[0]) && i < sizeof(data) / sizeof(data[0]); i++) {
		if(htinsert(&my_hash_table, entries[i], &data[i]) != TABLE_OK) {
			fprintf(stderr, "Error: Could not insert element to table!\n");
			return MEM_ERROR;
		}

		printf("[-] Inserted value %d under key %s...\n", data[i], entries[i]);
	}

	printf("[+] Searching for values...\n");

	for(int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
		if(htlookup(&my_hash_table, entries[i], &data_out) != TABLE_OK) {
			printf("[-] Invalid key: \"%s\"!\n", entries[i]);
		} else {
			printf("[-] Found value \"%d\" under key %s!\n", data_out, entries[i]);
		}
	}

	printf("[+] Deleting values...\n");

	for(int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
		if(htdelete(&my_hash_table, entries[i]) != TABLE_OK) {
			printf("[-] Invalid key: \"%s\"!\n", entries[i]);
		} else {
			printf("[-] Successfully deleted value under key %s!\n", entries[i]);
		}
	}

	printf("[+] Searching for deleted values...\n");

	for(int i = 0; i < sizeof(entries) / sizeof(entries[0]); i++) {
		if(htlookup(&my_hash_table, entries[i], &data_out) != TABLE_OK) {
			printf("[-] Invalid key: \"%s\"!\n", entries[i]);
		} else {
			printf("[-] Found value \"%d\" under key %s!\n", data_out, entries[i]);
		}
	}

	printf("[+] Destroying table...\n");
	
	htdestroy(&my_hash_table);

	printf("[+] All tests complete, terminating...\n");

	return 0;
}