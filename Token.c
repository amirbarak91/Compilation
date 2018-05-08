#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Token.h"

#define TOKEN_ARRAY_SIZE 100

Node * head;

int currentIndex = 0;
Node* currentNode = NULL;

Node * selectedNode = NULL;
int selectedIndex = -1;

void print_token_message(char * token_type_name, char * lexeme, int line)
{
	fprintf(yyout, "Token of kind '%s' was found at line: %d, lexeme: '%s'.\n", token_type_name, line, lexeme);
}

void print_token_error(char * lexeme, int line)
{
	fprintf(yyout, "The character '%s' at line: %d does not begin any legal token in the language.\n", lexeme, line);
}

void found_token(eTOKENS token, char * lexeme, int line)
{
	print_token_message(getTokenNameByEnum(token), lexeme, line);
	create_and_store_token(token, lexeme, line);
}



int is_at_start()
{
	return selectedNode == head && selectedIndex == -1;
}
int is_at_end()
{
	return selectedNode == currentNode && selectedIndex == currentIndex + 1;
}
const char * eTokensNames[] =
{
	"INT_NUM",
	"REAL_NUM",
	"TOKEN_ID",
	"KEYWORD_BLOCK",
	"KEYWORD_BEGIN",
	"KEYWORD_END",
	"KEYWORD_TYPE",
	"KEYWORD_IS",
	"KEYWORD_INTEGER",
	"KEYWORD_REAL",
	"KEYWORD_ARRAY",
	"KEYWORD_OF",
	"KEYWORD_WHEN",
	"KEYWORD_DO",
	"KEYWORD_DEFAULT",
	"KEYWORD_END_WHEN",
	"KEYWORD_FOR",
	"TOKEN_KEYWORD_END_FOR",
	"KEYWORD_MALLOC",
	"KEYWORD_SIZE_OF",
	"KEYWORD_FREE",
	"RELATION_OP_EQUAL",
	"RELATION_OP_NOT_EQUAL",
	"RELATION_OP_GREATER_EQUAL",
	"RELATION_OP_GREATER",
	"RELATION_OP_LESS_EQUAL",
	"RELATION_OP_LESS",
	"ARITMETIC_OP_PLUSE",
	"ARITMETIC_OP_MINUS",
	"ARITMETIC_OP_MUL",
	"ARITMETIC_OP_DIV",
	"ARITMETIC_OP_POWER",
	"UNARY_OP_PLUSE_PLUSE",
	"UNARY_OP_ADDRESS",
	"SEPARATION_SIGN_COLON",
	"SEPARATION_SIGN_SEMICOLON",
	"SEPARATION_SIGN_OPEN_PARENTHESES",
	"SEPARATION_SIGN_CLOSE_PARENTHESES",
	"SEPARATION_SIGN_OPEN_BRACKETS",
	"SEPARATION_SIGN_CLOSE_BRACKETS",
	"ASSIGNMENT_OP",
	"POINTER_IND",
	"EOF"
};





/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		//allocating new node
		selectedNode = head = currentNode = (Node*)malloc(sizeof(Node));
		//check allocation
		if (currentNode == NULL)//in case it is not successfull
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}

		//allocating new node tokensArray
		currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);
		//check allocation
		if (currentNode->tokensArray == NULL)//in case it is not successfull
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}

		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;

			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme) + 1));
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
}


Token *back_token()
{
	if (currentNode == NULL|| is_at_start())
	{
		return NULL;
	}

	selectedIndex--;
	if (!is_at_start() && selectedIndex == -1)
	{
		selectedIndex = TOKEN_ARRAY_SIZE-1;
		selectedNode = selectedNode->prev;
	}

	return selectedNode->tokensArray + selectedIndex;
}


Token *next_token()
{
	if (currentNode == NULL || is_at_end())
	{
		return NULL;
	}

	selectedIndex++;
	if (!is_at_end() && selectedIndex == TOKEN_ARRAY_SIZE)
	{
		selectedIndex = 0;
		selectedNode = selectedNode->next;
	}

	return selectedNode->tokensArray + selectedIndex;
}

void free_all_tokens()
{
	int i;
	Node * temp = head;
	Node * prev;

	while (temp != NULL)
	{
		for (i = 0; i <= ((temp == currentNode) ? currentIndex : TOKEN_ARRAY_SIZE - 1); i++)
		{
			free(temp->tokensArray[i].lexeme);
		}
		free(temp->tokensArray);
		prev = temp;
		temp = temp->next;
		free(prev);
	}

	currentNode = NULL;
	currentIndex = 0;
	selectedIndex = -1;
	selectedNode = NULL;

}

int match(eTOKENS t)
{
	Token * token = next_token();
	if (token->kind != t)
	{
		printf("Expected token '%s' at line : %d,\nActual token '%s',lexeme '%s'\n", getTokenNameByEnum(t), token->lineNumber, getTokenNameByEnum(token->kind), token->lexeme);
	}
}

void match_rel_op()
{
	Token * token = next_token();

	switch (token->kind)
	{
	case TOKEN_RELATION_OP_EQUAL:
	case TOKEN_RELATION_OP_NOT_EQUAL:
	case TOKEN_RELATION_OP_GREATER:
	case TOKEN_RELATION_OP_GREATER_EQUAL:
	case TOKEN_RELATION_OP_LESS:
	case TOKEN_RELATION_OP_LESS_EQUAL:
		break;
	default:
		printf("Expected tokens '==' '!=' '>' '>=' '<' '<='  at line : %d,\nActual token '%s','%s'\n", token->lineNumber, getTokenNameByEnum(token->kind), token->lexeme);

	}
}



const char * getTokenNameByEnum(eTOKENS kind)
{
	return eTokensNames[kind];
}
