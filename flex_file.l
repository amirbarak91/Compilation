%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "Token.h"
	#include "Parsers.h"

	#undef yywrap
	#define yywrap() 1

	int lineNumber = 1;


	void run_test(const char * input,const char * lexical_output,const char * syntactic_output)
	{	
		if((yyin = fopen(input, "r")) == NULL)
		{
			printf("Error! cant open %s.");
			exit(0);
		}
		
		if((yyout = fopen(lexical_output, "w")) == NULL)
		{
			printf("Error! cant open %s.");
			exit(0);
		}

		yylex();

		if(fclose(yyin))
		{
			printf("Error! cant close %s.");
			exit(0);
		}


		if(fclose(yyout))
		{
			printf("Error! cant close %s.");
			exit(0);
		}

		if((yyout = fopen(syntactic_output, "w")) == NULL)
		{
			printf("Error! cant open %s.");
			exit(0);
		}


		parse_PROGRAM();

		free_all_tokens();


	
		if(fclose(yyout))
		{
			printf("Error! cant close %s.");
			exit(0);
		}
	}
%}

COMMENT								"--".*
WHITE_SPACE							" "|"\t"|"\v"|"\r"|"\f"

INT_NUM								[1-9][0-9]*|0
REAL_NUM							(0|([1-9][0-9]*))"."[0-9]+

KEYWORD_BLOCK						"block"
KEYWORD_BEGIN						"begin"
KEYWORD_END							"end"
KEYWORD_TYPE						"type"
KEYWORD_IS							"is"
KEYWORD_INTEGER						"integer"
KEYWORD_REAL						"real"
KEYWORD_ARRAY						"array"
KEYWORD_OF							"of"
KEYWORD_WHEN						"when"
KEYWORD_DO							"do"
KEYWORD_DEFAULT						"default"
KEYWORD_END_WHEN					"end_when"
KEYWORD_FOR							"for"
KEYWORD_END_FOR						"end_for"
KEYWORD_MALLOC						"malloc"
KEYWORD_SIZE_OF						"size_of"
KEYWORD_FREE						"free"

ID									[a-zA-Z]("_"?[a-zA-Z0-9]+)*

RELATION_OP_EQUAL					"=="
RELATION_OP_NOT_EQUAL				"!="
RELATION_OP_GREATER_EQUAL			">="
RELATION_OP_GREATER					">"
RELATION_OP_LESS_EQUAL				"<="
RELATION_OP_LESS					"<"

ARITMETIC_OP_PLUSE					"+"
ARITMETIC_OP_MINUS					"-"
ARITMETIC_OP_MUL					"*"
ARITMETIC_OP_DIV					"/"
ARITMETIC_OP_POWER					"**"

UNARY_OP_PLUSE_PLUSE				"++"
UNARY_OP_ADDRESS					"&"

SEPARATION_SIGN_COLON				":"
SEPARATION_SIGN_SEMICOLON			";"
SEPARATION_SIGN_OPEN_PARENTHESES	"("
SEPARATION_SIGN_CLOSE_PARENTHESES	")"
SEPARATION_SIGN_OPEN_BRACKETS		"["
SEPARATION_SIGN_CLOSE_BRACKETS		"]"

ASSIGNMENT_OP						"="
POINTER_IND							"^"



%%
\n					lineNumber++;

{COMMENT}			
{WHITE_SPACE}

{INT_NUM}							found_token(TOKEN_INT_NUM,yytext,lineNumber);
{REAL_NUM}							found_token(TOKEN_REAL_NUM,yytext,lineNumber);

{KEYWORD_BLOCK}						found_token(TOKEN_KEYWORD_BLOCK,yytext,lineNumber);
{KEYWORD_BEGIN}						found_token(TOKEN_KEYWORD_BEGIN,yytext,lineNumber);
{KEYWORD_END}						found_token(TOKEN_KEYWORD_END,yytext,lineNumber);
{KEYWORD_TYPE}						found_token(TOKEN_KEYWORD_TYPE,yytext,lineNumber);
{KEYWORD_IS}						found_token(TOKEN_KEYWORD_IS,yytext,lineNumber);
{KEYWORD_INTEGER}					found_token(TOKEN_KEYWORD_INTEGER,yytext,lineNumber);
{KEYWORD_REAL}						found_token(TOKEN_KEYWORD_REAL,yytext,lineNumber);
{KEYWORD_ARRAY}						found_token(TOKEN_KEYWORD_ARRAY,yytext,lineNumber);
{KEYWORD_OF}						found_token(TOKEN_KEYWORD_OF,yytext,lineNumber);
{KEYWORD_WHEN}						found_token(TOKEN_KEYWORD_WHEN,yytext,lineNumber);
{KEYWORD_DO}						found_token(TOKEN_KEYWORD_DO,yytext,lineNumber);
{KEYWORD_DEFAULT}					found_token(TOKEN_KEYWORD_DEFAULT,yytext,lineNumber);
{KEYWORD_END_WHEN}					found_token(TOKEN_KEYWORD_END_WHEN,yytext,lineNumber);
{KEYWORD_FOR}						found_token(TOKEN_KEYWORD_FOR,yytext,lineNumber);
{KEYWORD_END_FOR}					found_token(TOKEN_KEYWORD_END_FOR,yytext,lineNumber);
{KEYWORD_MALLOC}					found_token(TOKEN_KEYWORD_MALLOC,yytext,lineNumber);
{KEYWORD_SIZE_OF}					found_token(TOKEN_KEYWORD_SIZE_OF,yytext,lineNumber);
{KEYWORD_FREE}						found_token(TOKEN_KEYWORD_FREE,yytext,lineNumber);

{ID} 								found_token(TOKEN_ID,yytext,lineNumber);

{RELATION_OP_EQUAL}					found_token(TOKEN_RELATION_OP_EQUAL,yytext,lineNumber);
{RELATION_OP_NOT_EQUAL}				found_token(TOKEN_RELATION_OP_NOT_EQUAL,yytext,lineNumber);
{RELATION_OP_GREATER_EQUAL}			found_token(TOKEN_RELATION_OP_GREATER_EQUAL,yytext,lineNumber);
{RELATION_OP_GREATER}				found_token(TOKEN_RELATION_OP_GREATER,yytext,lineNumber);
{RELATION_OP_LESS_EQUAL}			found_token(TOKEN_RELATION_OP_LESS_EQUAL,yytext,lineNumber);
{RELATION_OP_LESS}					found_token(TOKEN_RELATION_OP_LESS,yytext,lineNumber);

{ARITMETIC_OP_PLUSE}				found_token(TOKEN_ARITMETIC_OP_PLUSE,yytext,lineNumber);
{ARITMETIC_OP_MINUS}				found_token(TOKEN_ARITMETIC_OP_MINUS,yytext,lineNumber);
{ARITMETIC_OP_MUL}					found_token(TOKEN_ARITMETIC_OP_MUL,yytext,lineNumber);
{ARITMETIC_OP_DIV}					found_token(TOKEN_ARITMETIC_OP_DIV,yytext,lineNumber);
{ARITMETIC_OP_POWER}				found_token(TOKEN_ARITMETIC_OP_POWER,yytext,lineNumber);

{UNARY_OP_PLUSE_PLUSE}				found_token(TOKEN_UNARY_OP_PLUSE_PLUSE,yytext,lineNumber);
{UNARY_OP_ADDRESS}					found_token(TOKEN_UNARY_OP_ADDRESS,yytext,lineNumber);

{SEPARATION_SIGN_COLON}				found_token(TOKEN_SEPARATION_SIGN_COLON,yytext,lineNumber);
{SEPARATION_SIGN_SEMICOLON}			found_token(TOKEN_SEPARATION_SIGN_SEMICOLON,yytext,lineNumber);
{SEPARATION_SIGN_OPEN_PARENTHESES}	found_token(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES,yytext,lineNumber);
{SEPARATION_SIGN_CLOSE_PARENTHESES} found_token(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES,yytext,lineNumber);
{SEPARATION_SIGN_OPEN_BRACKETS}		found_token(TOKEN_SEPARATION_SIGN_OPEN_BRACKETS,yytext,lineNumber);
{SEPARATION_SIGN_CLOSE_BRACKETS}	found_token(TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS,yytext,lineNumber);

{ASSIGNMENT_OP}						found_token(TOKEN_ASSIGNMENT_OP,yytext,lineNumber);

{POINTER_IND} 						found_token(TOKEN_POINTER_IND,yytext,lineNumber);

<<EOF>>								found_token(TOKEN_EOF,yytext,lineNumber); yyterminate();

.									print_token_error(yytext,lineNumber);
	
%%


int main()
{

	run_test("C:\\temp\\test1.txt","C:\\temp\\test1_305759292_316626631_lex.txt","C:\\temp\\test1_305759292_316626631_syntactic.txt");
	yyrestart(yyin);
	BEGIN(INITIAL);
	lineNumber = 1;

	run_test("C:\\temp\\test2.txt","C:\\temp\\test2_305759292_316626631_lex.txt","C:\\temp\\test2_305759292_316626631_syntactic.txt");
	
    return 1;
}