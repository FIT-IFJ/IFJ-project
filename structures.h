/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast:    Abstraktne struktury pre parser                                  */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy:   Tym 102                                                         */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef SCANNER_H
#define SCANNER_H
#include "scanner.h"
#endif

#include "our_error.h"


typedef struct DLLElement {
	token_t *token;
    struct DLLElement *prev;
    struct DLLElement *next;
} DLLElement;

typedef struct {
	DLLElement* first;
	DLLElement* last;
	DLLElement* active;
} DLList;



void DLL_Init( DLList *list );
void DLL_GetFirst( DLList *list, token_t *token );
void DLL_GetLast( DLList *list, token_t *token );
void DLL_InsertFirst( DLList *list, token_t *token );
void DLL_InsertLast( DLList *list, token_t *token );
void DLL_DeleteFirst( DLList *list );
void DLL_DeleteLast( DLList *list );
void DLL_Dispose( DLList *list );
