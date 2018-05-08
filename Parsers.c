#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Parsers.h"
#include "Token.h"


Token * tok;


void print_rule(char * from, char * to)
{
	fprintf(yyout,"%s -> %s\n", from, to);
}

void print_default_error(char * expected_kinds, Token * token,int is_only_one)
{
	if (is_only_one)
		fprintf(yyout,"Expected token ");
	else
		fprintf(yyout, "Expected one of tokens ");

	fprintf(yyout, "%s at line : %d,\nActual token %s, lexeme:%s\n", expected_kinds, token->lineNumber, getTokenNameByEnum(token->kind), token->lexeme);
}


void parse_PROGRAM()
{
	current_HashTable = NULL;
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_BLOCK:
		print_rule("PROGRAM", "BLOCK");
		back_token();
		parse_BLOCK();
		match(TOKEN_EOF);
		break;
	default:
		print_default_error("'block'", tok,1);
		eTOKENS follows[1] = { TOKEN_EOF };
		while (!is_one_of(follows, 1, next_token()->kind));
		back_token();
		match(TOKEN_EOF);
	}
}

void parse_BLOCK()
{
	hashtable_t * temp = Create_HashTable(100);

	if (current_HashTable != NULL)
	{
		temp->parent = current_HashTable;
	}

	current_HashTable = temp;

	print_rule("BLOCK","block DEFINITIONS; begin COMMANDS; end");
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_BLOCK:
		parse_DEFINITIONS();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_KEYWORD_BEGIN);
		parse_COMMANDS();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_KEYWORD_END);
		break;
	default:
		print_default_error("'block'", tok,1);
		eTOKENS follows[1] = { TOKEN_EOF };
		while (!is_one_of(follows, 1, next_token()->kind));
		back_token();

	}

	current_HashTable = current_HashTable->parent;
	free(temp);
}

void parse_DEFINITIONS()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_TYPE:
	case TOKEN_ID:
		print_rule("DEFINITIONS", "DEFINITION DEFINITION_HELPER");
		back_token();
		parse_DEFENITION();
		parse_DEFENITION_HELPER();
		break;
	default:
		print_default_error("'type','id'",tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_DEFENITION_HELPER()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_SEPARATION_SIGN_SEMICOLON:
		tok = next_token();
		if (tok->kind == TOKEN_KEYWORD_BEGIN)
		{
			print_rule("DEFENITION_HELPER", "epsilon");
			back_token(); back_token();
		}

		else
		{
			tok = back_token();
			print_rule("DEFENITION_HELPER", "; DEFINITION DEFINITION HELPER");
			parse_DEFENITION();
			parse_DEFENITION_HELPER();
		}
		break;
	default:
		print_default_error("';'", back_token(),1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_DEFENITION()
{

	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_ID:
		print_rule("DEFENITION","VAR_DEFINITION");
		back_token();
		parse_VAR_DEFENITION();
		break;

	case TOKEN_KEYWORD_TYPE:
		print_rule("DEFENITION", "TYPE_DEFINITION");
		back_token();
		parse_TYPE_DEFENITION();
		break;

	default:
		print_default_error("'id','type'", tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}

}

void parse_VAR_DEFENITION()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_ID:
		print_rule("VAR_DEFENITION","id:VAR_DEFENITION_HELPER");
		match(TOKEN_SEPARATION_SIGN_COLON);
		parse_VAR_DEFENITION_HELPER();
		break;
	default:
		print_default_error("'id'", tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}

}

void parse_VAR_DEFENITION_HELPER()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_INTEGER:
	case TOKEN_KEYWORD_REAL:
		print_rule("VAR_DEFINITION_HELPER", "BASIC_TYPE");
		back_token();
		parse_BASIC_TYPE();
		break;

	case TOKEN_ID:
		print_rule("VAR_DEFINITION_HELPER", tok->lexeme);
		break;

	default:
		print_default_error("'integer','real','type_name'", tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}
}

void parse_TYPE_DEFENITION()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_TYPE:
		print_rule("TYPE_DEFENITION", "type type_name is TYPE_INDICATOR");
		match(TOKEN_ID);
		match(TOKEN_KEYWORD_IS);
		parse_TYPE_INDICATOR();
		break;
	default:
		print_default_error("'type'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}

}

void parse_TYPE_INDICATOR()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_INTEGER:
	case TOKEN_KEYWORD_REAL:
		print_rule("TYPE_INDICATOR", "BASIC_TYPE");
		back_token();
		parse_BASIC_TYPE();
		break;

	case TOKEN_KEYWORD_ARRAY:
		print_rule("TYPE_INDICATOR", "ARRAY_TYPE");
		back_token();
		parse_ARRAY_TYPE();
		break;

	case TOKEN_POINTER_IND:
		print_rule("TYPE_INDICATOR", "POINTER_TYPE");
		back_token();
		parse_POINTER_TYPE();
		break;
	default:
		print_default_error("'integer','real','array','^'",tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_ARRAY_TYPE()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_ARRAY:
		print_rule("ARRAY_TYPE","array [SIZE] of BASIC_TYPE");
		match(TOKEN_SEPARATION_SIGN_OPEN_BRACKETS);
		parse_SIZE();
		match(TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS);
		match(TOKEN_KEYWORD_OF);
		parse_BASIC_TYPE();
		break;

	default:
		print_default_error("'array'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_SIZE() 
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_INT_NUM:
		print_rule("SIZE","int_num");
		break;
	default:
		print_default_error("'int_num'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_BASIC_TYPE()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_INTEGER:
		print_rule("BASIC_TYPE", "integer");
		break;

	case TOKEN_KEYWORD_REAL:
		print_rule("BASIC_TYPE", "real");
		break;

	default:
		print_default_error("'integer','real'",tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_POINTER_TYPE()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_POINTER_IND:
		print_rule("POINTER_TYPE", "^POINTER_TYPE_HELPER");
		parse_POINTER_TYPE_HELPER();
		break;
	default:
		print_default_error("'^'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}
}

void parse_POINTER_TYPE_HELPER()
{
	tok = next_token();

	switch (tok->kind)
	{
	case TOKEN_KEYWORD_INTEGER:
	case TOKEN_KEYWORD_REAL:
		print_rule("POINTER_TYPE_HELPER", "BASIC_TYPE");
		back_token();
		parse_BASIC_TYPE();
		break;

	case TOKEN_ID:
		print_rule("POINTER_TYPE_HELPER", "type_name");
		break;

	default:
		print_default_error("'type_name','integer','real'",tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}

}





void parse_COMMANDS()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_WHEN:
	case TOKEN_KEYWORD_FOR:
	case TOKEN_ID:
	case TOKEN_KEYWORD_FREE:
	case TOKEN_KEYWORD_BLOCK:
		print_rule("COMMANDS", "COMMAND COMMANDS_HELPER");
		back_token();
		parse_COMMAND();
		parse_COMMANDS_HELPER();
		break;

	default:
		print_default_error("'when','for','id','free','block'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_COMMAND()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_WHEN:
		print_rule("COMMAND", "when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when");
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		parse_EXPRESSION();
		match_rel_op();
		parse_EXPRESSION();
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		match(TOKEN_KEYWORD_DO);
		parse_COMMANDS();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_KEYWORD_DEFAULT);
		parse_COMMANDS();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_KEYWORD_END_WHEN);
		break;
	case TOKEN_KEYWORD_FOR:
		print_rule("COMMAND","for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for " );
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		match(TOKEN_ID);
		match(TOKEN_ASSIGNMENT_OP);
		parse_EXPRESSION();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_ID);
		match_rel_op();
		parse_EXPRESSION();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_ID);
		match(TOKEN_UNARY_OP_PLUSE_PLUSE);
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		parse_COMMANDS();
		match(TOKEN_SEPARATION_SIGN_SEMICOLON);
		match(TOKEN_KEYWORD_END_FOR);
		break;
	case TOKEN_ID:
		print_rule("COMMAND", "id RECIVER_HELPER");
		parse_RECEIVER_HELPER();
		break;
	case TOKEN_KEYWORD_FREE:
		print_rule("COMMAND", "free(id)");
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		match(TOKEN_ID);
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		break;
	case TOKEN_KEYWORD_BLOCK:
		print_rule("COMMAND", "BLOCK");
		back_token();
		parse_BLOCK();
		break;
	default:
		print_default_error("'when','for','id','free','block'", tok, 1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();
	}
}

void parse_COMMANDS_HELPER()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_SEPARATION_SIGN_SEMICOLON:
		tok = next_token();

		if (tok->kind == TOKEN_KEYWORD_END		||
			tok->kind == TOKEN_KEYWORD_END_FOR ||
			tok->kind == TOKEN_KEYWORD_DEFAULT ||
			tok->kind == TOKEN_KEYWORD_END_WHEN)
		{
			print_rule("COMMANDS_HELPER", "epsilon");
			back_token(); back_token();
		}

		else
		{
			tok = back_token();
			print_rule("COMMANDS_HELPER", "; COMMANDS COMMANDS_HELPER");
			parse_COMMAND();
			parse_COMMANDS_HELPER();
		}
		break;
	default:
		print_default_error("';'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}
}

void parse_RECEIVER_HELPER()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_ASSIGNMENT_OP:
		print_rule("RECEIVER_HELPER", "= RECIVER_HELPER_AFTER_EQUAL");
		parse_RECIVER_HELPER_AFTER_EQUAL();
		break;
	case TOKEN_POINTER_IND:
		print_rule("RECEIVER_HELPER", "^ = EXPRESSION");
		match(TOKEN_ASSIGNMENT_OP);
		parse_EXPRESSION();
		break;

	case TOKEN_SEPARATION_SIGN_OPEN_BRACKETS:
		print_rule("RECEIVER_HELPER", "[EXPRESSION] = EXPRESSION");
		parse_EXPRESSION();
		match(TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS);
		match(TOKEN_ASSIGNMENT_OP);
		parse_EXPRESSION();
		break;

	default:
		print_default_error("'=','^','['", tok,0);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}
}

void parse_RECIVER_HELPER_AFTER_EQUAL()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_ID:
	case TOKEN_INT_NUM:
	case TOKEN_REAL_NUM:
	case TOKEN_UNARY_OP_ADDRESS:
	case TOKEN_KEYWORD_SIZE_OF:
		print_rule("RECIVER_HELPER_AFTER_EQUAL", "EXPRESSION ");
		back_token();
		parse_EXPRESSION();
		break;
	case TOKEN_KEYWORD_MALLOC:
		print_rule("RECIVER_HELPER_AFTER_EQUAL", "malloc(size_of(type_name))");
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		match(TOKEN_KEYWORD_SIZE_OF);
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		match(TOKEN_ID);
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		break;

	default:
		print_default_error("'id','int_num','real_num','&','size_of','malloc'", tok,1);
		eTOKENS follows[2] = { TOKEN_EOF,TOKEN_SEPARATION_SIGN_SEMICOLON };
		while (!is_one_of(follows, 2, next_token()->kind));
		back_token();

	}
}





void parse_EXPRESSION()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_ID:
		print_rule("EXPRESSION", "id EXPRESSION_HELPER ");
		parse_EXPRESSION_HELPER();
		break;
	case TOKEN_INT_NUM:
		print_rule("EXPRESSION", "int_num");
		break;
	case TOKEN_REAL_NUM:
		print_rule("EXPRESSION", "real_num");
		break;
	case TOKEN_UNARY_OP_ADDRESS:
		print_rule("EXPRESSION", "&id");
		match(TOKEN_ID);
		break;
	case TOKEN_KEYWORD_SIZE_OF:
		print_rule("EXPRESSION", "size_of(type_name)");
		match(TOKEN_SEPARATION_SIGN_OPEN_PARENTHESES);
		match(TOKEN_ID);
		match(TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES);
		break;

	default:
		print_default_error("'id','int_num','real_num','&','size_of'", tok,1);
		eTOKENS follows[10] = {	TOKEN_EOF,
								TOKEN_SEPARATION_SIGN_SEMICOLON,
								TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS,
								TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES,
								TOKEN_RELATION_OP_EQUAL,
								TOKEN_RELATION_OP_NOT_EQUAL,
								TOKEN_RELATION_OP_GREATER_EQUAL,
								TOKEN_RELATION_OP_GREATER,
								TOKEN_RELATION_OP_LESS_EQUAL,
								TOKEN_RELATION_OP_LESS };
		while (!is_one_of(follows, 10, next_token()->kind));
		back_token();

	}
}

void parse_EXPRESSION_HELPER()
{

	tok = next_token();
	switch (tok->kind)
	{

	case TOKEN_SEPARATION_SIGN_OPEN_BRACKETS:
		print_rule("EXPRESSION_HELPER", "[EXPRESSION]");
		parse_EXPRESSION();
		match(TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS);
		break;
	case TOKEN_POINTER_IND:
		print_rule("EXPRESSION_HELPER", "^");
		break;

	case TOKEN_ARITMETIC_OP_DIV:
	case TOKEN_ARITMETIC_OP_MINUS:
	case TOKEN_ARITMETIC_OP_MUL:
	case TOKEN_ARITMETIC_OP_PLUSE:
	case TOKEN_ARITMETIC_OP_POWER:
		print_rule("EXPRESSION_HELPER", "ar_op EXPRESSION");
		parse_EXPRESSION();
		break;
	
	default:
		if (tok->kind == TOKEN_SEPARATION_SIGN_SEMICOLON ||
			tok->kind == TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS ||
			tok->kind == TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES ||
			tok->kind == TOKEN_RELATION_OP_EQUAL ||
			tok->kind == TOKEN_RELATION_OP_NOT_EQUAL ||
			tok->kind == TOKEN_RELATION_OP_GREATER ||
			tok->kind == TOKEN_RELATION_OP_GREATER_EQUAL ||
			tok->kind == TOKEN_RELATION_OP_LESS ||
			tok->kind == TOKEN_RELATION_OP_LESS_EQUAL)
		{
			print_rule("EXPRESSION_HELPER", "epsilon");
			back_token();
		}
		else
		{
			tok = back_token();
			print_default_error("'[','^','ar_op'", tok,0);
			eTOKENS follows[10] = { TOKEN_EOF,
				TOKEN_SEPARATION_SIGN_SEMICOLON,
				TOKEN_SEPARATION_SIGN_CLOSE_BRACKETS,
				TOKEN_SEPARATION_SIGN_CLOSE_PARENTHESES,
				TOKEN_RELATION_OP_EQUAL,
				TOKEN_RELATION_OP_NOT_EQUAL,
				TOKEN_RELATION_OP_GREATER_EQUAL,
				TOKEN_RELATION_OP_GREATER,
				TOKEN_RELATION_OP_LESS_EQUAL,
				TOKEN_RELATION_OP_LESS };
			while (!is_one_of(follows, 10, next_token()->kind));
			back_token();
		}
	}
}

int is_one_of(eTOKENS * one_of,int size, eTOKENS to_check)
{
	int result = 0;
	int i;

	for (i = 0; !result && i < size; i++)
		if (one_of[i] == to_check)
			result = 1;

	return result;
}