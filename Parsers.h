#ifndef THEPARSERS_H
#define THEPARSERS_H
#include "Token.h"

void parse_PROGRAM();
void parse_BLOCK();

//-----------------DEFENITIONS-----------------
void parse_DEFINITIONS();
void parse_DEFENITION();
void parse_DEFENITION_HELPER();

void parse_VAR_DEFENITION();
void parse_VAR_DEFENITION_HELPER();

void parse_TYPE_DEFENITION();
void parse_TYPE_INDICATOR();
void parse_ARRAY_TYPE();
void parse_BASIC_TYPE();
void parse_SIZE();
void parse_POINTER_TYPE();
void parse_POINTER_TYPE_HELPER();

//-----------------COMMANDS-----------------
void parse_COMMANDS();
void parse_COMMAND();
void parse_COMMANDS_HELPER();
void parse_RECEIVER_HELPER();
void parse_RECIVER_HELPER_AFTER_EQUAL();
void parse_EXPRESSION();
void parse_EXPRESSION_HELPER();

void print_rule(char *, char *);
void print_default_error(char *, Token *, int);

int is_one_of(eTOKENS * , int , eTOKENS );


#endif