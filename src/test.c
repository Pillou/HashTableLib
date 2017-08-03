/*
 * test.c
 * 
 * Copyright 2017 guillaume <guillaume.lecam@zoho.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "HashTable.h"

struct TestData1_s
{
	int key;
	char buffer[30];
};

int32_t print_node(HashTableNode_t *node)
{
	printf("key = %d, data = %s, hash = %d, next node = %p\n", \
	*(int*)node->key, (char*) node->data, node->hash, node->next);
	return 0;
}

int main(int argc, char **argv)
{
	struct TestData1_s test_data_1[HASH_TABLE_NUM_BUCKETS*2];
	
	printf("Start testing HashTable.c\n\n");

	printf("Testing the modular hashing function.\n");

	for(uint32_t i=0; i<10*HASH_TABLE_NUM_BUCKETS; i++)
	{
		assert(HashTableModularHashing(&i) < HASH_TABLE_NUM_BUCKETS);
	}

	printf("Testing the HashTable with modular hashing function.\n");
	printf("Keys : uint32_t, data : strings\n\n");

	printf("Testing creation.\n");
	HashTable_t *ht1 = HashTableCreate(HashTableUINTCompare, HashTableModularHashing);

	assert(HashTableGetCount(ht1) == 0);

	printf("Testing adding elements.\n");
	for(int i=0; i<sizeof(test_data_1)/sizeof(test_data_1[0]); i++)
	{
		char buffer[30];
		sprintf(buffer, "%d", i);
		
		test_data_1[i].key = i;
		strcpy(test_data_1[i].buffer, buffer);
		
		HashTableSet(ht1, &test_data_1[i].key, test_data_1[i].buffer);
		assert(HashTableGetCount(ht1) == i+1);
	}

	printf("\n\nPrinting hash table :\n");
	assert( HashTableTraverse(ht1, print_node) == 0);

	printf("\n\nTesting getting elements.\n");
	for(int i=0; i<sizeof(test_data_1)/sizeof(test_data_1[0]); i++)
	{
		void *data = HashTableGet(ht1, &test_data_1[i].key);
		assert(data != NULL);
		assert(strcmp((char*)data, test_data_1[i].buffer) == 0);
	}
	int unused_key = HASH_TABLE_NUM_BUCKETS*2;
	assert(HashTableGet(ht1, &unused_key) == NULL);

	printf("Testing deleting elements.\n");
	uint32_t current_count = HashTableGetCount(ht1);
	int key_index = HASH_TABLE_NUM_BUCKETS+5;
	printf("Deleting element with key = %d...\n", test_data_1[key_index].key);
	void *data = HashTableGet(ht1, &test_data_1[key_index].key);
	
	assert(data != NULL);
	assert(strcmp((char*)data, test_data_1[key_index].buffer) == 0);

	HashTableRemove(ht1, &test_data_1[key_index].key);
	current_count--;

	assert(HashTableGet(ht1, &test_data_1[key_index].key) == NULL);
	assert(current_count == HashTableGetCount(ht1));

	printf("\n\nPrinting hash table :\n");
	assert( HashTableTraverse(ht1, print_node) == 0);

	key_index = 5;
	printf("Deleting element with key = %d...\n", test_data_1[key_index].key);
	data = HashTableGet(ht1, &test_data_1[key_index].key);
	
	assert(data != NULL);
	assert(strcmp((char*)data, test_data_1[key_index].buffer) == 0);

	HashTableRemove(ht1, &test_data_1[key_index].key);
	current_count--;

	printf("\n\nPrinting hash table :\n");
	assert( HashTableTraverse(ht1, print_node) == 0);

	assert(HashTableGet(ht1, &test_data_1[key_index].key) == NULL);
	assert(current_count == HashTableGetCount(ht1));
	
	
	printf("\n\nPrinting hash table :\n");
	assert( HashTableTraverse(ht1, print_node) == 0);
	

	
	
	return 0;
}

