/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "structures.h"
#include "symtab.h"



int DLL_parse(DLList *list, token_t *token, dynamic_string *string, DLList *AST_list);