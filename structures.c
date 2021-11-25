
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




void error(int err_num)
{
    switch (err_num)
    {
    case 1:
        fprintf(stderr, "Syntax error\n"); //delenie 0, zly zapis vyrazu,.. '_' v tabulke
        exit(1);
        break;
    case 2:
        fprintf(stderr, "Memory allocation error\n"); //malloc
        exit(2);
        break;
    case 99:
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
        break;

    }
    
}



// *********************** currently used DLL functions **************************


void DLL_Init( DLList *list ) 
{
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
}


void DLL_InsertLast( DLList *list, token_t *token )
{

    DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
    if (new_element == NULL)
    {
        error(2);
        return;
    }
    new_element->token = (token_t *) malloc(sizeof(token_t));
    * new_element->token = *token;
    printf("**%s**\n", new_element->token->attribute);
    new_element->next = NULL;
    new_element->prev = list->last;
    if(list->last != NULL)
        list->last->next = new_element;
    else
        list->first = new_element;

    list->last = new_element;

}


void DLL_GetLast( DLList *list, token_t *token )
{

    if (list->last == NULL)
        error(2);
    else
        token = list->last->token;
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
        free(del_element);
    }
}



void DLL_Dispose( DLList *list )
{
    list->active = list->first;
    while (list->active != NULL) 
    {
        free(list->active->token);
        free(list->active);
        list->active = list->active->next;
    }
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
}



// *********************** currently unused DLL functions **************************

void DLL_InsertFirst( DLList *list, token_t *token )
{

    DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
    if (new_element == NULL)
    {
        error(2);
        return;
    }
    new_element->token = token;
    new_element->next = list->first;
    new_element->prev = NULL;
    if(list->first != NULL)
        list->first->prev = new_element;
    else
        list->last = new_element;

    list->first = new_element;
}


void DLL_GetFirst( DLList *list, token_t *token )
{

    if (list->first == NULL)
        error(2);
    else
        token = list->first->token;

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
        free(del_element);
    }


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


void DLL_InsertAfter( DLList *list, token_t *token )     // pre parser na posunutie tokenu
{

    if (list->active != NULL)    
    {
        DLLElement *new_element = (DLLElement *)malloc(sizeof(DLLElement));
        new_element->token = token;
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
        new_element->token = token;
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

    if (list->active == NULL)
        error(2);
    else
        token = list->active->token;
    
}

void DLL_SetValue( DLList *list, token_t *token )
{

    if (list->active != NULL)
        list->active->token = token;

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

// ***************************************************************************






void DLL_to_DLL(DLList *pp_list, DLList *list, token_t* token)
{
    DLL_GetLast(pp_list, token);
    DLL_DeleteLast(pp_list);
    DLL_InsertLast(list, token);
}


/*
// vopred initializuj list
//DLList *list = (DLList *)malloc (sizeof(DLList));
//DLL_Init(list);
void DLL_parse(DLList *list, token_t *token, dynamic_string *string) // funkcia je zavolana po tokene "if", "while", "return" a "="
{
    DLL_Dispose(list);
    DLList *pp_list = (DLList *) malloc (sizeof(DLList));
    DLL_Init(pp_list);


    while(token != NULL)
    {
        get_token(token, string);
        DLL_InsertLast(pp_list, token);
        if (token->type == TYPE_KEYWORD || token->type == TYPE_DATATYPE)
        {
            DLL_to_DLL(pp_list, list, token);
            break;
        }
        else if( list->last->prev != NULL )
        {
            if (token->type == TYPE_IDENTIFIER && list->last->prev->token->type == TYPE_IDENTIFIER)
            {
                DLL_to_DLL(pp_list, list, token);
                break;
            }
            else if (token->spec == SPEC_OPEN && list->last->prev->token->type == TYPE_IDENTIFIER)
            {
                DLL_to_DLL(pp_list, list, token);
                DLL_to_DLL(pp_list, list, token);
                break;
            }
        }
    
    }
    if (pp_list->last != NULL)
        parse_expression(pp_list);
    DLL_Dispose(pp_list);



}
*/






int main()
{
    DLList *list = (DLList *)malloc (sizeof(DLList));
    token_t* token = malloc(sizeof(token_t));
    if (list == NULL || token == NULL)
        return -1;

    dynamic_string *string = string_init();
    printf("here\n");
    DLL_Init(list);
    printf("and here\n");
    for (int i = 0; i < 10; i++)
    {
        get_token(token,string);
        DLL_InsertLast(list, token);
        printf("[%s]\n", token->attribute);
    }
    

    DLLElement *element = list->first;
    printf(">>>%s<<<\n", element->token->attribute);
    while(element != NULL)
    {
        printf("%s\n", element->token->attribute);
        element = element->next;

    }
    DLL_Dispose(list);
    free (list);
    free (token);

    return 0;
}