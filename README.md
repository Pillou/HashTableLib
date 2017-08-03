# HashTableLib
HashTableLib is a simple hash table implementation

The implementation has a fixed size array to contain the buckets.
It can be changed by modifying HASH_TABLE_NUM_BUCKETS.

WARNING: the user is reponsible of allocation/deallocation of the memory of the key and data.


This library can use any type of key, hash function or data. 
It provide an implementation and test of modular hashing with uin32_t key.
