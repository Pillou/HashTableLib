/*
 * HashTable.h
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
 
#ifndef __HASH_TABLE__H__
#define __HASH_TABLE__H__

#include <stdint.h>
 
/* Some prime numbers : 53 101 151 199 251 307 353 401 449 503 */
#define HASH_TABLE_NUM_BUCKETS							(53U)

typedef int (*HashTableCmp_t)(void *d1, void *d2);
typedef uint32_t (*HashTableFct_t)(void *key);

 
 
typedef struct HashTableNode_s
{
	struct HashTableNode_s *next;
	void *key;
	void *data;
	uint32_t hash;
} HashTableNode_t;
 
typedef struct HashTable_s
{
	uint32_t count;
	HashTableNode_t *buckets[HASH_TABLE_NUM_BUCKETS];
	HashTableCmp_t compare_fct;
	HashTableFct_t hash_fct;
} HashTable_t;

typedef int32_t (*HashTableTraverse_t)(HashTableNode_t *node);
 
HashTable_t* HashTableCreate(HashTableCmp_t compare_fct, HashTableFct_t hash_fct);
void HashTableDelete(HashTable_t hash_table);
 
int32_t HashTableSet(HashTable_t *hash_table, void *key, void *data);
void* HashTableGet(HashTable_t *hash_table, void *key);
int32_t HashTableRemove(HashTable_t *hash_table, void *key);
 
uint32_t HashTableGetCount(HashTable_t *hash_table);

uint32_t HashTableModularHashing(void *key);
int HashTableUINTCompare(void* a, void *b);

int32_t HashTableTraverse(HashTable_t *hash_table, HashTableTraverse_t travers_fct);
 
 
 
 
 
 #endif //__HASH_TABLE__H__
