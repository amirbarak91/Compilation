#ifndef SEMANTIC_H
#define SEMANTIC_H

typedef struct Var_Attribute {
	char * name;
	char * type;
	char * subtype;
	eROLE role;
	eCATEGORY category;
	unsigned int size;
}Var_Attribute;


typedef enum eROLE {
	VARIABLE,
	USER_DEFINED_TYPE
}eROLE;

typedef enum eCATEGORY {
	BASIC,
	ARRAY,
	POINTER
};


#endif
