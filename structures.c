
#include "structures.h"



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




void DLL_InsertFirst( DLList *list, token_t *token )
{

    DLLElement *new_element = (DLLElement *) malloc(sizeof(DLLElement));
    if (new_element == NULL)
    {
        error(2);
        return;
    }
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



// *********************** currently unused DLL functions **************************


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
        new_element->token = (token_t *) malloc(sizeof(token_t));
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
        new_element->token = (token_t *) malloc(sizeof(token_t));
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


void parse_expression(DLList *pp_list)
{
    DLLElement *element = pp_list->first;
    printf("\n******************\n\n");
    while(element != NULL)
    {
        printf("processed[%s]\n", element->token->attribute);
        element = element->next;

    }
}

void DLL_fill(DLList *list, token_t *token, dynamic_string *string) // funkcia je zavolana po tokene "if", "while", "return" a "="
{
    DLL_Dispose(list);
    while (token->type != TYPE_EOF)
    {
        get_token(token,string);
        DLL_InsertLast(list, token);
        printf("inserted[%s]\n", token->attribute);



        if (token->type == TYPE_KEYWORD)        //      zakomentuj pre naplnenie DLL az do konca (nerobi to potom tolko chyb XD)
            return;                             //
        
    }
}




// ***********************************************************************************************************************************************************************

//                                                       HERE IT BEGINS

// ***********************************************************************************************************************************************************************







void DLL_to_DLL(DLList *pp_list, DLList *list, token_t* token)
{
    DLL_GetLast(pp_list, token);
    DLL_DeleteLast(pp_list);
    DLL_InsertFirst(list, token);
}




/**
 * Funkcia je zavolana z parseru po tokene "if", "while", "return" a "=".
 * Naplni pp_list a posunie ho do PSA parseru funkciou parse_expression(pp_list)
 * Ak narazi na klucove slovo alebo situaciu vylucujucu vyskyt vyrazu, tokeny nepatriace
 * do pp_listu uberie a prida do listu, z ktoreho si to vyberie hlavny parser.
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu, ktora sa plni pre parser
 * @param token Ukazatel na inicializovanu strukturu tokenu pre funkciu get_token(..)
 * @param string --||--
 */
void DLL_parse(DLList *list, token_t *token, dynamic_string *string) 
{
    DLL_Dispose(list);      // nwm ci tu musi byt, ale presitotu premazat pri znovu-uzivani
    DLList *pp_list = (DLList *) malloc (sizeof(DLList));   //pp_list pre parsovanie vyrazu
    DLL_Init(pp_list);

    while(token->type != TYPE_EOF)
    {
        get_token(token, string);

        DLL_InsertLast(pp_list, token);

         //ak nasleduje keyword, tak vymaze posledny token a vlozi ho do listu pre hlavny parser
        if (pp_list->last->token->type == TYPE_KEYWORD || pp_list->last->token->type == TYPE_DATATYPE)     
        {
            DLL_to_DLL(pp_list, list, token);
            break;
        }
        else if( pp_list->last->prev != NULL )
        {
             //ak nasleduje dalsi identifier bez znamienka medzi nimi, tak vymaze posledny token a vlozi ho do listu pre hlavny parser
            if (pp_list->last->token->type == TYPE_IDENTIFIER && (pp_list->last->prev->token->type == TYPE_IDENTIFIER || pp_list->last->prev->token->spec == SPEC_CLOS))
            {
                DLL_to_DLL(pp_list, list, token);
                break;
            }
             //ak nasleduje funkcia, tak vymaze posledne dva tokeny a vlozi ho do listu pre hlavny parser
            else if (pp_list->last->token->spec == SPEC_OPEN && pp_list->last->prev->token->type == TYPE_IDENTIFIER)
            {
                DLL_to_DLL(pp_list, list, token);
                DLL_to_DLL(pp_list, list, token);
                break;
            }
        }
    }

    if (list->last != NULL)
    {
        token->spec = DOLR;
        DLL_InsertLast( pp_list, token);
        parse_expression(pp_list);      //docasny vypis pre znazornenie prace PSA parseru
    }
    DLL_Dispose(pp_list);               // vymazanie pp_listu po tom ako su vyrazy spracovane
    free(pp_list);
}

// ***************************************************************************
/*

int main()  // testovaci main
{
    DLList *list = (DLList *)malloc (sizeof(DLList));
    token_t* token = malloc(sizeof(token_t));
    if (list == NULL || token == NULL)
        return -1;

    dynamic_string *string = string_init();
    
    DLL_Init(list);
    
    do
    {
        get_token(token,string);

        printf("\n---> %s,%d,%d\n",token->attribute,token->type,token->spec);

        if(token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->spec == SPEC_WHILE || token->type == TYPE_ASSIGNMENT)
        {
            //DLL_fill(list, token, string);      // testovacie naplnenie DLL
            //parse_expression(list);             // testovaci vypis DLL
            DLL_parse(list, token, string);

        }
    }while (token->type != TYPE_EOF);


    printf("\n\nKONIEC\n");
    DLL_Dispose(list);
    free (list);
    free (token);

    return 0;
}


*/





int main()  // tento main uz znazornuje pracu parseru a mal by robit (ale nerobi) to co chcem 
{
    DLList *list = (DLList *)malloc (sizeof(DLList));
    token_t* token = malloc(sizeof(token_t));
    if (list == NULL || token == NULL)
        return -1;

    dynamic_string *string = string_init();
    DLLElement *element;
    DLL_Init(list);

    do
    {
        get_token(token,string);
        printf("\n---> %s,%d,%d\n",token->attribute,token->type,token->spec);   // cinnost parseru

        if (token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->spec == SPEC_WHILE || token->type == TYPE_ASSIGNMENT) // Tejd si sam triedi
        {


            DLL_parse(list,token,string);   // volanie pre parsovanie pripadneho vyrazu
            if (list->last != NULL)
            {
                element = list->first;
                while(element != NULL)      // v liste sa nachadza omylom ukradnuty token pre parser
                {
                    printf("\n---> %s,%d,%d\n",element->token->attribute,element->token->type,element->token->spec);    // cinnost parseru -> moze obsahovat "keyword","datatype","identifier"
                    if (token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->spec == SPEC_WHILE)
                    {
                        DLL_parse(list,token,string);
                        if (list->last != NULL)//
                        {
                            element = list->first;
                            printf("\n---> %s,%d,%d\n",element->token->attribute,element->token->type,element->token->spec);//
                        }
                    }
                    element = element->next;//
                }
            }
        }
    }while (token->type != TYPE_EOF);

    
    DLL_Dispose(list);
    free (list);
    free (token);

    return 0;
}
