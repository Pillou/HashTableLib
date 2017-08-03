/*
 * HashTable.c
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

#include "HashTable.h"
#include <assert.h>
#include <stdlib.h>


static HashTableNode_t *HashTableCreateNode(void *key, void *data, uint32_t hash);


HashTable_t* HashTableCreate(HashTableCmp_t compare_fct, HashTableFct_t hash_fct)
{
	HashTable_t *hash_table = malloc(sizeof(*hash_table));
	assert(hash_table != NULL);

	hash_table->compare_fct = compare_fct;
	hash_table->hash_fct = hash_fct;
	hash_table->count = 0;
	
	return hash_table;
}

uint32_t HashTableGetCount(HashTable_t *hash_table)
{
	assert(hash_table != NULL);
	return hash_table->count;
}


uint32_t HashTableModularHashing(void *key)
{
	
	uint32_t bucket_index = *(int*)key % HASH_TABLE_NUM_BUCKETS;
	return bucket_index;
}

static HashTableNode_t *HashTableCreateNode(void *key, void *data, uint32_t hash)
{
	HashTableNode_t *node = malloc(sizeof(*node));
	assert(node != NULL);
	node->key = key;
	node->data = data;
	node->hash = hash;
	node->next = NULL;
	return node;
}

int32_t HashTableSet(HashTable_t *hash_table, void *key, void *data)
{
	assert(hash_table != NULL);
	assert(key != NULL);
	assert(data != NULL);

	uint32_t hash = hash_table->hash_fct(key);
	assert(hash < HASH_TABLE_NUM_BUCKETS);
	HashTableNode_t *node = HashTableCreateNode(key, data, hash);

	if(hash_table->buckets[node->hash] == NULL)
	{
		hash_table->buckets[node->hash] = node;
	}
	else // collision
	{
		//search last node
		HashTableNode_t *prev = hash_table->buckets[node->hash];
		while(prev->next != NULL)
		{
			prev = prev->next;
		}
		prev->next = node;
	}
	hash_table->count++;

	return node->hash;
}

void* HashTableGet(HashTable_t *hash_table, void *key)
{
	assert(hash_table != NULL);
	assert(key != NULL);

	uint32_t hash = hash_table->hash_fct(key);
	assert(hash < HASH_TABLE_NUM_BUCKETS);
	HashTableNode_t *node = hash_table->buckets[hash];

	while(node != NULL)
	{
		if(hash_table->compare_fct(key, node->key) != 0)
		{
			break;
		}
		node = node->next;
	}
	if(node == NULL)
	{
		return NULL;
	}
	else
	{
		return node->data;
	}
}

int HashTableUINTCompare(void* a, void *b)
{
	return (*(uint32_t *)a) == (*(uint32_t*)b);
}


int32_t HashTableRemove(HashTable_t *hash_table, void *key)
{
	assert(hash_table != NULL);
	assert(key != NULL);

	uint32_t hash = hash_table->hash_fct(key);
	assert(hash < HASH_TABLE_NUM_BUCKETS);
	HashTableNode_t *node = hash_table->buckets[hash];
	HashTableNode_t *prev = NULL;
	
	while(node != NULL)
	{
		
		if(hash_table->compare_fct(key, node->key) != 0)
		{
			break;
		}
		prev = node;
		node = node->next;
	}
	if(node != NULL)	// node found
	{
		if(prev != NULL)
		{
			prev->next = node->next;
		}
		else
		{
			hash_table->buckets[hash] = NULL;
		}
		free(node);
		hash_table->count--;
		return 0;
	}
	else
	{
		return -1;
	}
}

int32_t HashTableTraverse(HashTable_t *hash_table, HashTableTraverse_t traverse_fct)
{
	assert(hash_table != NULL);

	HashTableNode_t *node = NULL;
	int32_t ret = 0;

	for(uint32_t i= 0; i<HASH_TABLE_NUM_BUCKETS; i++)
	{
		node = hash_table->buckets[i];
		while(node != NULL)
		{
			ret += traverse_fct(node);
			node = node->next;
		}
	}
	return ret;
}

void HashTableDelete(HashTable_t *hash_table)
{
	assert(hash_table != NULL);
	
	HashTableNode_t *node = NULL;
	HashTableNode_t *next = NULL;

	for(uint32_t i= 0; i<HASH_TABLE_NUM_BUCKETS; i++)
	{
		node = hash_table->buckets[i];
		while(node != NULL)
		{ 
			next = node->next;
			
			free(node);
			hash_table->count--;
			
			node = next;
		}
	}
	free(hash_table);
}

 



 
