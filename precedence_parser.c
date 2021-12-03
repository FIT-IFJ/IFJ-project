/* ************************* precedence_parser.c **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include "precedence_parser.h"




#define NUM_OF_SYMB 17

char pp_table[NUM_OF_SYMB][NUM_OF_SYMB]={ 
  /*            +     -     *     /     //    <    <=     >    >=    ==    ~=    ..     #     i     (     )     $   */
  /*  +  */   {'>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },  // +,-
  /*  -  */   {'>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  *  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },  // *,/,//
  /*  /  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  // */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  <  */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },   // <,..,~=
  /*  <= */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  >  */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  >= */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  == */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  ~= */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
  /*  .. */   {'_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '<',  '_',  '<',  '<',  '>',  '>'  },
  /*  #  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '<',  '_',  '<',  '<',  '>',  '>'  }, // !!!???????
  /*  i  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '_',  '_',  '>',  '>'  },
  /*  (  */   {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '=',  '_'  },
  /*  )  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '_',  '_',  '>',  '>'  },
  /*  $  */   {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '_',  '#'  } // # .. exp ok
};


 #define DOLR 16  // '$' len pre precedencnu tabulku
 #define MARK 17  // nahradza '<' v zasobniku
 #define NONT 18



// struktury pre zasobnik
typedef struct pp_stack_element{
    //int token_n;
    token_t *token; //// I do need it!!!
    struct pp_stack_element *next;
} pp_stack_element;


typedef struct pp_stack{
    pp_stack_element *top_element;
} pp_stack;





void stack_init(pp_stack* stack)
{
    stack->top_element = NULL;
}

////
void stack_push(pp_stack* stack, token_t* token)     // pushne token_n, napr. pre '='
{
   // printf("_____\n| '%s' |\n_____\n",token->attribute);
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    if (new_element == NULL)
        error(2);
    new_element->next = stack->top_element;
    new_element->token = (token_t *) malloc(sizeof(token_t));
    if (new_element->token == NULL)
        error(2);
    *new_element->token = *token;

    stack->top_element = new_element;
    
}


void stack_pop(pp_stack* stack)
{
    if (stack->top_element != NULL)
    {
        pp_stack_element* stack_element = stack->top_element;
        stack->top_element = stack_element->next;
        free(stack_element->token);
        free(stack_element);
    }
}


pp_stack_element* stack_top(pp_stack* stack) // ??
{

    return stack->top_element;
}



token_t* stack_top_term(pp_stack* stack) // zobrazi najvrchnejsi terminal
{

    pp_stack_element* stack_element = stack->top_element;
    
    while(stack_element->token->spec == NONT)   // pozor musis vytvorit token->spec = NONT
    {
        stack_element = stack_element->next;
    }
    return stack_element->token;
} 


void stack_mark(pp_stack* stack)   //oznaci zaciatok pravidla '<'
{
    pp_stack_element* stack_element = stack->top_element;
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    new_element->token = (token_t *) malloc(sizeof(token_t));

    while (stack_element != NULL)
        {
            if (stack_element == stack->top_element && stack_element->token->spec != NONT)
            {
                new_element->next = stack_element;
                stack->top_element = new_element;
                new_element->token->attribute = "[<]"; // for debug
                new_element->token->spec = MARK;
                return;
            }
            else if (stack_element->next->token->spec != NONT)
            {
                new_element->next = stack_element->next;
                stack_element->next = new_element;
                new_element->token->attribute = "[<]";  // for debug
                new_element->token->spec = MARK;
                return;
            }
            stack_element = stack_element->next;
        }

}



bool stack_empty(pp_stack* stack)
{
    return (stack->top_element == NULL);
}

void stack_dispose(pp_stack* stack)
{
    while (!stack_empty(stack))
      stack_pop(stack);
}



void reduce(pp_stack* stack,DLList* AST_list) // prevedie redukciu '>'
{
    pp_stack_element* stack_element;
    
    token_t *token[3];
    for (int i = 0; i < 3; i++)
    {
        token[i] = malloc(sizeof(token_t));
        if (token == NULL)
            error(2);
    }
    
    int count = 2;
    do
    {
        stack_element = stack_top(stack);        
        if (stack_element->token->spec == MARK)
        {        
            stack_pop(stack);
            break;
        }
        else
        {
            *token[count] = *stack_element->token;
            stack_pop(stack);
            count--;
        }
    }while (stack_element != NULL); // && count >= 0);


    // porovnavanie obsahu token[]->spec podla rules

    if (token[0]->spec == NONT && token[2]->spec == NONT)
    {
        switch (token[1]->spec)
        {
            case SPEC_PLUS:
                /* rule 1 */
                break;

            case SPEC_MINU:
                /* rule 2 */
                break;

            case SPEC_MULT:
                /* rule 3 */
                break;

            case SPEC_DIVF:
                /* rule 4 */
                break;

            case SPEC_DIVI:
                /* rule 5 */
                break;

            case SPEC_LESS:
                /* rule 6 */
                break;
            
            case SPEC_LEEQ:
                /* rule 7 */
                break;

            case SPEC_GREA:
                /* rule 8 */
                break;

            case SPEC_GREQ:
                /* rule 9 */
                break;

            case SPEC_EQUA:
                /* rule 10 */
                break;

            case SPEC_NOEQ:
                /* rule 11 */
                break;
            
            case SPEC_CONC:
                /* rule 12 */
                break;

            default:
                error(2);
                break;
        }

        printf("( %i.)  E -> E %s E\n",token[1]->spec, token[1]->attribute);
        if (AST_list != NULL)
            DLL_InsertLast(AST_list,token[1]);

    }
    else if ( token[0]->spec == '\0' && token[1]->spec == SPEC_HASH && token[2]->spec == NONT)
    {
        /* rule 13 */
        printf("( %i.)  E -> %s E\n",token[1]->spec, token[1]->attribute);
        if (AST_list != NULL)
            DLL_InsertLast(AST_list,token[1]);
    }
    else if (token[0]->spec == '\0' && token[1]->spec == '\0' && token[2]->spec == SPEC_IDOP)
    {
        /* rule 14 */
        printf("( %i.)  E -> '%s' \n",token[2]->spec, token[2]->attribute);
        if (AST_list != NULL)
            DLL_InsertLast(AST_list,token[2]);
    }
    else if (token[0]->spec == SPEC_OPEN && token[1]->spec == NONT && token[2]->spec == SPEC_CLOS)
    {
        /* rule 15 */
        printf("( 15.)  E -> ( E )\n");
    }
    else
        error(2); // syntax error

}


void push_nont(pp_stack* stack)
{ 
    token_t* token = malloc(sizeof(token_t));
    if (token == NULL)
        error(2);
    token->spec = NONT;
    token->attribute = "E"; // for debug 
    stack_push(stack, token);
    free(token);

}


void print_stack(pp_stack* stack)   // function for debug
{
    pp_stack_element* stack_element;
    DLLElement* DLL_element;
    DLList *list = (DLList *)malloc (sizeof(DLList));
    if (list == NULL)
        error(2);
    DLL_Init(list);

    printf("\n");
    stack_element = stack->top_element;
    while (stack_element!=NULL)
    {
        DLL_InsertFirst(list,stack_element->token);
        stack_element = stack_element->next;
    }

    DLL_element = list->first;
    while(DLL_element != NULL)
    {
        printf("| '%s' ",DLL_element->token->attribute);
        DLL_element = DLL_element->next;
    }
    printf("\n\n");

    DLL_Dispose(list);
    free(list);
}


void parse_expression (DLList *list, DLList *AST_list)
{
    pp_stack *stack = (pp_stack*) malloc(sizeof(pp_stack));
    token_t* token = malloc(sizeof(token_t));
    if (stack == NULL || token == NULL)
        error(2);

    stack_init(stack);

    token->spec = DOLR;
    token->attribute = "$"; // for debug
    stack_push(stack, token);

    DLLElement *element;
    element = list->first;
    token = element->token; // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
    int stack_n;            // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
   

    while (true) //length of array
    {
        stack_n = (stack_top_term(stack))->spec; //// stack_token = stack_top_term(stack); 

        print_stack(stack); //for debug
        printf("\n[stack: %d '%s']  vs  [token: %d '%s']\n\ntable symbol [%c]\n\n",stack_n,(stack_top_term(stack))->attribute,token->spec,token->attribute,pp_table[stack_n][token->spec]); // for debug

        switch (pp_table[stack_n][token->spec])
        {
        case '=':
            stack_push(stack, token);
            element = element->next;
            *token = *element->token;
            break;
        
        case '<':
            stack_mark(stack);
            stack_push(stack, token);
            element = element->next;
            *token = *element->token;
            break;

        case '>':   // prerobit na void !!
            reduce(stack, AST_list);  // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            push_nont(stack);
            break;

        case '#':
            stack_dispose(stack);
            free(stack);
            printf("Parsing of expression was successfull!!\n");
            return;
            break;

        default:
            stack_dispose(stack);
            error(1);
            break;
        }
    }

}




// ***********************************************************************************************************************************************************************





void DLL_fill(DLList *list, token_t *token, dynamic_string *string) // for debug - not used already
{
    do
    {
        get_token(token,string);
        DLL_InsertLast(list, token);
        printf("inserted[%s]\n", token->attribute);

    }while (token->type != TYPE_EOF);
    list->last->token->spec = DOLR;
}

void DLL_print(DLList *AST_list)    // for debug
{
    printf("\n\nAST_LIST:\n");
    DLLElement *element = AST_list->first;
    while(element != NULL)      // v liste sa nachadza omylom ukradnuty token pre parser
    {
        printf(" [ %s ]", element->token->attribute);
        element = element->next;
    }
    printf("\n");
}


/**
 * Funkcia je presunie token z konca zdrojoveho zoznamu na zaciatok cieloveho zoznamu.
 * Tento token zaroven po presunuti zo zdrojoveho tokenu vymaze.
 * Vyuziva sa pri vrateni omylom vziateho tokenu pre hlavny parser.
 * 
 * @param pp_list zdrojovy zoznam (zoznam posuvany precedencnemu parseru)
 * @param list cielovy zoznam (zoznam posuvany hlavnemu parseru)
 * @param token pomocny token na presuvanie
 */
void DLL_to_DLL(DLList *pp_list, DLList *list, token_t* token)
{
    DLL_GetLast(pp_list, token);
    DLL_InsertFirst(list, token);
    DLL_DeleteLast(pp_list);
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
void DLL_parse(DLList *list, token_t *token, dynamic_string *string, DLList *AST_list) 
{
    printf("actual_token: %s\n", token->attribute);
    DLL_Dispose(list);      // presitotu premaze pri znovu-uzivani
    if (AST_list != NULL)
        DLL_Dispose(AST_list);      // presitotu premaze pri znovu-uzivani
    DLList *pp_list = (DLList *) malloc (sizeof(DLList));   //pp_list pre parsovanie vyrazu
    if (pp_list == NULL)
        error(2);
    DLL_Init(pp_list);
    while(token->type != TYPE_EOF)
    {
        if (token->spec == SPEC_NIL)
            token->spec = SPEC_IDOP; // nil je v precedencnom parseri spracovavany ako operand
        DLL_InsertLast(pp_list, token);
        
         //ak nasleduje keyword, tak vymaze posledny token a vlozi ho do listu pre hlavny parser
        if (pp_list->last->token->type == TYPE_KEYWORD || (pp_list->last->token->type == TYPE_DATATYPE && pp_list->last->token->spec != SPEC_IDOP) || strcmp(pp_list->last->token->attribute,",") == 0)     
        {
            DLL_to_DLL(pp_list, list, token);
            break;
        }
        else if( pp_list->last->prev != NULL )
        {
             //ak nasleduje dalsi identifier bez znamienka medzi nimi (t.j. koniec vyrazu), tak vymaze posledny token a vlozi ho do listu pre hlavny parser
            if (pp_list->last->token->type == TYPE_IDENTIFIER && (pp_list->last->prev->token->spec == SPEC_IDOP || pp_list->last->prev->token->spec == SPEC_CLOS))
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
        get_token(token, string);
    }
    if (pp_list->last != NULL)
    {
        printf("\n_____________________________PP_PARSER________________________________\n");
        token->spec = DOLR;
        DLL_InsertLast( pp_list, token);
        parse_expression(pp_list, AST_list);      //docasny vypis pre znazornenie prace PSA parseru
        if (AST_list != NULL)
            DLL_print(AST_list);            // for debug
        printf("\n____________________________MAIN_PARSER_______________________________\n");
    }
    DLL_Dispose(pp_list);               // vymazanie pp_listu po tom ako su vyrazy spracovane
    free(pp_list);
}








// **************************************************************************

//                       TOTO VLOZ DO HLAVNEHO PARSERU


// **************************************************************************

int parser()  // tento main uz znazornuje pracu parseru a mal by robit (ale nerobi) to co chcem 
{
    DLList *list = (DLList *)malloc (sizeof(DLList));
    DLList *AST_list = (DLList *)malloc (sizeof(DLList));
    token_t* token = malloc(sizeof(token_t));
    if (list == NULL || AST_list == NULL || token == NULL)
        return -1;

    dynamic_string *string = string_init();
    DLLElement *element;
    DLL_Init(list);
    DLL_Init(AST_list);

    do
    {
        get_token(token,string);
        printf("\n---> %s,%d,%d\n",token->attribute,token->type,token->spec);   // cinnost parseru

        if (token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->spec == SPEC_WHILE || token->type == TYPE_ASSIGNMENT) // Tejd si sam triedi
        {
            get_token(token,string);
            DLL_parse(list,token,string,AST_list);   // volanie pre parsovanie pripadneho vyrazu
            if (list->last != NULL)
            {
                element = list->first;
                while(element != NULL)      // v liste sa nachadza omylom ukradnuty token pre parser
                {
                    printf("\n---> %s,%d,%d\n",element->token->attribute,element->token->type,element->token->spec);    // cinnost parseru -> moze obsahovat "keyword","datatype","identifier"
                    if (element->token->spec == SPEC_RETURN || element->token->spec == SPEC_IF || element->token->spec == SPEC_WHILE || strcmp(element->token->attribute,",") == 0)
                    {
                        get_token(token,string);
                        //*token = *element->token; // ak get_token je na zaciatku while vo funkcii
                        DLL_parse(list,token,string, AST_list);
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


    

    DLL_Dispose(AST_list);
    DLL_Dispose(list);
    free (AST_list);
    free (list);
    free (token);

    return 0;
}



//  ODKOMENTUJ PRE TESTOVANIE
/*
int main()
{
    return parser();
}
*/

