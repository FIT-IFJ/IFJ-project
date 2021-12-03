/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast:    Abstraktne struktury pre parser                                  */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy:   Tym 102                                                         */
/* ************************************************************************** */


#include "structures.h"


void DLL_Init( DLList *list ) 
{
    list->first = NULL;
    list->last = NULL;
}


void DLL_InsertFirst( DLList *list, token_t *token )
{

    DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
    if (new_element == NULL)
        error(99, 0);
    new_element->token = (token_t *) malloc(sizeof(token_t));
    * new_element->token = *token;
    new_element->next = list->first;
    new_element->prev = NULL;
    if(list->first != NULL)
        list->first->prev = new_element;
    else
        list->last = new_element;

    list->first = new_element;
}


void DLL_InsertLast( DLList *list, token_t *token )
{

    DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
    if (new_element == NULL)
        error(99, 0);
    new_element->token = (token_t *) malloc(sizeof(token_t));
    if (new_element->token == NULL)
        error(99, 0);
    * new_element->token = *token;
    new_element->next = NULL;
    new_element->prev = list->last;
    if(list->last != NULL)
        list->last->next = new_element;
    else
        list->first = new_element;

    list->last = new_element;

}


void DLL_GetFirst( DLList *list, token_t *token )
{

    if (list->first == NULL)
        error(99, 0);
    else
        token = list->first->token;

}

void DLL_GetLast( DLList *list, token_t *token )
{

    if (list->last == NULL)
        error(99, 0);
    else
        *token = *list->last->token;

}


void DLL_DeleteFirst( DLList *list ) 
{
    DLLElement *del_element;
    if (list->first != NULL)
    {
        del_element = list->first;        
        if (list->first == list->last)
        {
            list->first = NULL;
            list->last = NULL;
        }
        else
        {
            list->first = list->first->next;
            list->first->prev = NULL;
        }
        free(del_element->token);
        free(del_element);
    }
}


void DLL_DeleteLast( DLList *list )
{
    DLLElement *del_element;
    if (list->last != NULL) {
        del_element = list->last;        
        if (list->first == list->last)
        {
            list->first = NULL;
            list->last = NULL;
        }
        else
        {
            list->last = list->last->prev;
            list->last->next = NULL;
        }
        free(del_element->token);
        free(del_element);
    }
}


void DLL_Dispose( DLList *list )
{
    DLLElement *element = list->first;
    DLLElement *del_element;
    while (element != NULL) 
    {
        del_element = element;
        element = element->next;
        free(del_element->token);
        free(del_element);
    }
    list->first = NULL;
    list->last = NULL;
}


