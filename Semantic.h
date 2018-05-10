#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "HashTable.h"

typedef enum eROLE {
	ROLE_VARIABLE,
	ROLE_USER_DEFINED_TYPE
}eROLE;

typedef enum eCATEGORY {
	CATEGORY_BASIC,
	CATEGORY_ARRAY,
	CATEGORY_POINTER
}eCATEGORY;

typedef struct attributes {
	char * name;
	char * type;
	struct attributes * subtype;
	eROLE role;
	eCATEGORY category;
	unsigned int size;
}Attributes;

typedef struct table
{
	hashtable_t * hash_table;
	struct table * father;
}Table;

Attributes * Create_Attribute();
void Table_Destroy(Table *);
Table * make_table(Table *);
Table * pop_table(Table * current_table);
Attributes * insert(Table * current_table, char * id_name);
Attributes * lookup(Table * current_table, char * id_name);
Attributes * find(Table * current_table, char * id_name);
void set_attr_type(Attributes * attr, char * id_type);
char * get_attr_type(Attributes * attr);
#endif
