/* ******************************* parser.h  ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a sematicky analyzator                                  */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



int program(token_t* token, token_t* token_lookahead, dynamic_string* string);
int prologue(token_t* token, dynamic_string* string);
int program_body(token_t* token, token_t* token_lookahead, dynamic_string* string);
int func_decl(token_t* token, token_t* token_lookahead, dynamic_string* string);
int func_def(token_t* token, token_t* token_lookahead, dynamic_string* string);
int func_call(token_t* token, token_t* token_lookahead, dynamic_string* string);
int type_list(token_t* token, dynamic_string* string);
int types(token_t* token, dynamic_string* string);
int type(token_t* token, dynamic_string* string);
int param_list(token_t* token, dynamic_string* string);
int params(token_t* token, dynamic_string* string);
int constant_list(token_t* token, dynamic_string* string);
int constants(token_t* token, dynamic_string* string);
int func_body(token_t* token, dynamic_string* string);
int element(token_t* token, dynamic_string* string);
int decl_element(token_t* token, dynamic_string* string);
int if_element(token_t* token, dynamic_string* string);
int while_element(token_t* token, dynamic_string* string);
int return_element(token_t* token, dynamic_string* string);
int func_element(token_t* token, dynamic_string* string);
int item_list(token_t* token, dynamic_string* string);
int items(token_t* token, dynamic_string* string);
int item(token_t* token, dynamic_string* string);
int arg_list(token_t* token, dynamic_string* string);
int args(token_t* token, dynamic_string* string);
int arg(token_t* token, dynamic_string* string);
int decl_assign(token_t* token, dynamic_string* string);
int L_assignment(token_t* token, dynamic_string* string);
int R_assignment(token_t* token, dynamic_string* string);
int ids(token_t* token, dynamic_string* string);


