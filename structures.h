
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "precedence_parser.h"


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

void DLL_Dispose( DLList *list );
void DLL_parse(DLList *list, token_t *token, dynamic_string *string);