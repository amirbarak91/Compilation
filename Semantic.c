#include <stdio.h>
#include <stdlib.h>
#include "Semantic.h"

Attributes * Create_Attribute()
{
	return (Attributes *)calloc(1, sizeof(Attributes));
}

void Table_Destroy(Table * table)
{
	HashTable_Destroy(table->hash_table);
	free(table);
}

//Called when entering into a nested block in the program;
//creates symbol table for this block, and links it to the table of the current scope
Table * make_table(Table * current_table)
{
	Table * to_return;

	if ((to_return = (Table *)malloc(sizeof(Table))) == NULL)
	{
		printf("couldn't allocate new table");
		exit(0);
	}

	to_return->hash_table = Create_HashTable(100);
	to_return->father = current_table;

	return to_return;
}

//Called when exiting from a nested block;
//returns pointer to the enclosing block’s table(i.e.table of the exited block's "father")
Table * pop_table(Table * current_table)
{
	Table * to_return = current_table->father;
	
	Table_Destroy(current_table);

	return to_return;
}

//Called when a variable declaration is processed.
//Inserts the variable name and type into symbol table of the current scope,
//and returns pointer to the new entry.
//Before insertion, checks whether the  id_name  already appears in the current table(for this, uses function lookup).
//If yes then returns NULL; this indicates to an error(duplicate declaration of the same name) that should be reported.
Attributes * insert(Table * current_table, char * id_name) {

	Attributes * to_return = NULL;

	if (!lookup(current_table, id_name))
	{
		to_return = HashTable_Put(current_table->hash_table, id_name, Create_Attribute());
		to_return->name = id_name;
	}

	return to_return;
}

//Searches for a variable name in symbol table of the current scope(only);
//returns pointer to the found entry, or NULL if the name is not found in that table
Attributes * lookup(Table * current_table, char * id_name)
{
	return (Attributes *)HashTable_Get(current_table->hash_table, id_name);
}
//Called when variable use is found in assignment;  allows to check whether the variable is declared.For this, performs hierarchical search of the variable name, starting from the symbol table of the current scope; returns the found entry pointer, or NULL(if the variable is undeclared).

Attributes * find(Table * current_table, char * id_name)
{
	Table * tab = current_table;
	while (tab != NULL)
	{
		Attributes * attr = lookup(tab, id_name);
		if (attr != NULL)
			return (attr);
		else
			tab = tab->father;
	}
	printf("ERROR: undeclared identifier %s \n", id_name);
	return NULL;
}


//Called when id declaration is processed, to store its type in the symbol type.
void set_attr_type(Attributes * attr, char * id_type) {
	attr->type = id_type;
}

//Called when assignment is processed, for variables that appear in its left and right sides.Returns variable's type (integer or real)
char * get_attr_type(Attributes * attr) {
	return attr->type;
}
