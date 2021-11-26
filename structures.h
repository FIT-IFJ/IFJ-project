
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"

typedef struct DLLElement {
	token_t *token;
    struct DLLElement *prev;
    struct DLLElement *next;
} DLLElement;

typedef struct {
	DLLElement* first;
	DLLElement* active;
	DLLElement* last;
} DLList;


void error(int err_num);


void DLL_Init( DLList *list );
void DLL_GetFirst( DLList *list, token_t *token );
void DLL_GetLast( DLList *list, token_t *token );
void DLL_InsertFirst( DLList *list, token_t *token );
void DLL_InsertLast( DLList *list, token_t *token );
void DLL_DeleteFirst( DLList *list );
void DLL_DeleteLast( DLList *list );
void DLL_Dispose( DLList *list );
void DLL_parse(DLList *list, token_t *token, dynamic_string *string);

