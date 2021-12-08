/* ******************* generator.h ****************************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Hlavickovy soubor ke generatoru v generator.c                       */
/*  Vytvoril: Tym 102 - David Novak, prosinec 2021                            */
/* ************************************************************************** */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "ast.h"

void g_while(ast_node_t *while_node);
void g_body(ast_node_t *body_node);
void g_assign(ast_node_t *assign_node);
void g_branch(ast_node_t *if_node);
void g_if_condition(ast_node_t *if_condition_node, int current_if_no);
void g_while_condition(ast_node_t *condition_node, int current_while_no);
void g_return(ast_node_t *return_node);
void g_decl_var(ast_node_t *var_decl_node);
int g_expression(ast_node_t *expr_node);
void g_func_call(ast_node_t *call_node);
void g_func_def(ast_node_t *func_def_node);
void g_program(ast_node_t *program_node);
void g_error_labels();
void g_integrated_functions();
void g_reads();
void g_readi();
void g_readn();
//void g_write();
void g_tointeger();
void g_substr();
void g_ord();
void g_chr();

#endif
