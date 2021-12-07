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
    list->active = NULL;
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
        if (list->active == list->first)
            list->active = NULL;
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
        if (list->active == list->last)
            list->active = NULL;
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
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
}











void DLL_First( DLList *list )
{

    list->active = list->first;

}


void DLL_Last( DLList *list )
{

    list->active = list->last;

}


void DLL_DeleteAfter( DLList *list )
{

    if (list->active != NULL)
    {
        if (list->active->next != NULL)
        {
            DLLElement *del_element = list->active->next;
            list->active->next = del_element->next;
            if (del_element == list->last)
                list->last = list->active;
            else
                del_element->next->prev = list->active;
            free(del_element);
        }
    }
    
}


void DLL_DeleteBefore( DLList *list ) 
{

    if (list->active != NULL)
    {
        if (list->active->prev != NULL) 
        {
            DLLElement *del_element = list->active->prev;
            list->active->prev = del_element->prev;
            if (del_element == list->first)
                list->first = list->active;
            else
                del_element->prev->next = list->active;
            free(del_element);
        }
    }

}


void DLL_InsertAfter( DLList *list, token_t *token )
{

    if (list->active != NULL)    
    {
        DLLElement *new_element = (DLLElement *)malloc(sizeof(DLLElement));
        if (new_element == NULL)
            error(99, 0);
        new_element->token = (token_t *) malloc(sizeof(token_t));
        if (new_element->token == NULL)
            error(99, 0);
        * new_element->token = *token;
        new_element->next = list->active->next;
        new_element->prev = list->active;
        list->active->next = new_element;

        if (list->active == list->last)
            list->last = new_element;
        else
            new_element->next->prev = new_element;
    }

}

void DLL_InsertBefore( DLList *list, token_t *token )
{

    if (list->active != NULL)
    {
        DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
        if (new_element == NULL)
            error(99, 0);
        new_element->token = (token_t *) malloc(sizeof(token_t));
        if (new_element->token == NULL)
            error(99, 0);
        * new_element->token = *token;
        new_element->next = list->active;
        new_element->prev = list->active->prev;
        list->active->prev = new_element;

        if (list->active == list->first)
            list->first = new_element;
        else
            new_element->prev->next = new_element;
    }

}


void DLL_GetValue( DLList *list, token_t *token )
{

    if (list->active != NULL)
        *token = *list->active->token;
    
}

void DLL_SetValue( DLList *list, token_t *token )
{

    if (list->active != NULL)
        *list->active->token = *token;

}


void DLL_Next( DLList *list )
{

    if (list->active != NULL)
        list->active = list->active->next;

}


void DLL_Previous( DLList *list )
{

   if (list->active != NULL)
        list->active = list->active->prev;

}


int DLL_IsActive( DLList *list ) 
{

    return (list->active != NULL) ? true : false;

}


