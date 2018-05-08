#ifndef HASHTABLE_H
#define HASHTABLE_H

//Hashtable element structure
typedef struct hash_elem_t {
	struct hash_elem_t* next;	// Next element in case of a collision
	void* data;					// Pointer to the stored element
	char key[]; 				// Key of the stored element
} hash_elem_t;

//Hashtabe structure
typedef struct {
	unsigned int capacity;		// Hashtable capacity (in terms of hashed keys)
	unsigned int e_num;			// Number of element currently stored in the hashtable
	hash_elem_t** table;		// The table containaing elements
	hash_elem_t** parent;		// The tables parent
} hashtable_t;

//Structure used for iterations
typedef struct {
	hashtable_t* ht; 			// The hashtable on which we iterate
	unsigned int index;			// Current index in the table
	hash_elem_t* elem; 			// Curent element in the list
} hash_elem_it;

// Inititalize hashtable iterator on hashtable 'ht'
#define HT_ITERATOR(ht) {ht, 0, ht->table[0]}

char err_ptr;
void* HT_ERROR = &err_ptr;		// Data pointing to HT_ERROR are returned in case of error


static unsigned int HashTable_Calc_Hash(char* key);
hashtable_t* Create_HashTable(unsigned int capacity);
void* HashTable_Put(hashtable_t* hasht, char* key, void* data);
void* HashTable_Get(hashtable_t* hasht, char* key);
void* HashTable_Remove(hashtable_t* hasht, char* key);
void HashTable_List_Keys(hashtable_t* hasht, char** k, size_t len);
void HashTable_List_Values(hashtable_t* hasht, void** v, size_t len);
hash_elem_t* HashTable_Iterate(hash_elem_it* iterator);
char* HashTable_Interate_Keys(hash_elem_it* iterator);
void* HashTable_Interate_Values(hash_elem_it* iterator);
void HashTable_Clear(hashtable_t* hasht, int free_data);
void HashTable_Destroy(hashtable_t* hasht);



#endif // !HASHTABLE
