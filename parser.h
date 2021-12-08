/* ******************************* parser.h  ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a sematicky analyzator                                  */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ast.h"



int program(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* AST);
int prologue(token_t* token, token_t* token_lookahead, dynamic_string* string);
int program_body(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int func_decl(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int func_def(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int func_call(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int type_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int types(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int type(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int param_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int params(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int constant_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int constants(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int func_body(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int decl_element(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int if_element(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int while_element(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int return_element(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int func_element(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int item_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int items(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int item(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int arg_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int args(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int arg(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int decl_assign(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int L_assignment(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int R_assignment(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int assignment(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int ids(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);
int return_list(token_t* token, token_t* token_lookahead, dynamic_string* string, ast_node_t* parent_node);


#endif
