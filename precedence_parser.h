/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#ifndef PRECEDENCE_PARSER_HEADER
#define PRECEDENCE_PARSER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structures.h"
#include "symtab.h"


/**
 * @brief Funkcia je zavolana z parseru po tokene "if", "while", "return" a "=".
 * Naplni pp_list a posunie ho do PSA parseru funkciou parse_expression(pp_list)
 * Ak narazi na klucove slovo alebo situaciu vylucujucu vyskyt vyrazu, tokeny nepatriace
 * do pp_listu uberie a prida do listu, z ktoreho si to vyberie hlavny parser.
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu, ktora sa plni pre parser
 * @param token Ukazatel na inicializovanu strukturu tokenu pre funkciu get_token(..)
 * @param string --||--
 */
int DLL_parse(DLList *list, token_t *token, dynamic_string *string, DLList *AST_list);

#endif
