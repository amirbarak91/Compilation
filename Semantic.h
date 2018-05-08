#ifndef SEMANTIC_H
#define SEMANTIC_H

typedef enum eROLE {
	ROLE_VARIABLE,
	ROLE_USER_DEFINED_TYPE
}eROLE;

typedef enum eCATEGORY {
	CATEGORY_BASIC,
	CATEGORY_ARRAY,
	CATEGORY_POINTER
}eCATEGORY;

typedef struct Attribute {
	char * name;
	char * type;
	char * subtype;
	eROLE role;
	eCATEGORY category;
	unsigned int size;
}Attribute;

Attribute * Create_Attribute();

#endif
