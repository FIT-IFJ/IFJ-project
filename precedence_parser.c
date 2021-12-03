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



bool reduce(pp_stack* stack) // prevedie redukciu '>'
{
    pp_stack_element* stack_element;
    int token_n[3]= {-1,-1,-1};     ////prerobit na token
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
            token_n[count] = stack_element->token->spec;
            stack_pop(stack);
            count--;
        }
    }while (stack_element != NULL); // && count >= 0);


    // porovnavanie obsahu token_n[] podla rules

    if (token_n[0] == NONT && token_n[2] == NONT)
    {
        printf("( %i.)  E -> E [?] E\n",token_n[1]);
        switch (token_n[1])
        {
            case SPEC_PLUS:
                /* rule 1 */
                return true;
                break;

            case SPEC_MINU:
                /* rule 2 */
                return true;
                break;

            case SPEC_MULT:
                /* rule 3 */
                return true;
                break;

            case SPEC_DIVF:
                /* rule 4 */
                return true;
                break;

            case SPEC_DIVI:
                /* rule 5 */
                return true;
                break;

            case SPEC_LESS:
                /* rule 6 */
                return true;
                break;
            
            case SPEC_LEEQ:
                /* rule 7 */
                return true;
                break;

            case SPEC_GREA:
                /* rule 8 */
                return true;
                break;

            case SPEC_GREQ:
                /* rule 9 */
                return true;
                break;

            case SPEC_EQUA:
                /* rule 10 */
                return true;
                break;

            case SPEC_NOEQ:
                /* rule 11 */
                return true;
                break;
            
            case SPEC_CONC:
                /* rule 12 */
                return true;
                break;

            default:
                return false;
                break;
        }
    }
    else if (token_n[0] == -1 && token_n[1] == SPEC_HASH && token_n[2] == NONT)
    {
        /* rule 13 */
        printf("( 13.)  E -> # E \n");
        return true;
    }
    else if (token_n[0] == -1 && token_n[1] == -1 && token_n[2] == SPEC_IDOP)
    {
        /* rule 14 */
        printf("( 14.)  E -> <i>\n");
        return true;
    }
    else if (token_n[0] == SPEC_OPEN && token_n[1] == NONT && token_n[2] == SPEC_CLOS)
    {
        /* rule 15 */

        printf("( 15.)  E -> ( E )\n");
        return true;
    }
    else 
        return false; // syntax error

}





/*

int convert(char* attribute)
{
    int code_n;
    if(strcmp(attribute,"+") == 0)
        code_n = SPEC_PLUS;
    else if(strcmp(attribute,"-") == 0)
        code_n = SPEC_MINU;
    else if(strcmp(attribute,"*") == 0)
        code_n = SPEC_MULT;
    else if(strcmp(attribute,"/") == 0)
        code_n = SPEC_DIVF;
    else if(strcmp(attribute,"//") == 0)
        code_n = SPEC_DIVI;
    else if(strcmp(attribute,"<") == 0)
        code_n = SPEC_LESS;
    else if(strcmp(attribute,"<=") == 0)
        code_n = SPEC_LEEQ;
    else if(strcmp(attribute,">") == 0)
        code_n = SPEC_GREA;
    else if(strcmp(attribute,">=") == 0)
        code_n = SPEC_GREQ;
    else if(strcmp(attribute,"==") == 0)
        code_n = SPEC_EQUA;
    else if(strcmp(attribute,"~=") == 0)
        code_n = SPEC_NOEQ;
    else if(strcmp(attribute,"..") == 0)
        code_n = SPEC_CONC;
    else if(strcmp(attribute,"#") == 0)
        code_n = SPEC_HASH;

    //else if(strcmp(attribute,"i") == 0)
    //    code_n = IDOP;
    
    else if(strcmp(attribute,"(") == 0)
        code_n = SPEC_OPEN;
    else if(strcmp(attribute,")") == 0)
        code_n = SPEC_CLOS;

    //else if(strcmp(attribute,"$") == 0)
    //    code_n = LAST;

    return code_n;
}

*/


void push_nont(pp_stack* stack)
{ 
    token_t* token = malloc(sizeof(token_t));
    if (token == NULL)
        error(2);
    token->spec = NONT;
    token->attribute = "E"; // debug 
    stack_push(stack, token); ////
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


void parse_expression (DLList *list)
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
        printf("stack >>  %d : %d  << token\ntable symbol [%c]\n",stack_n,token->spec,pp_table[stack_n][token->spec]); // for debug

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
            if (! reduce(stack)) printf("**Syntax error**\n");  // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            push_nont(stack);
            break;

        case '#':
            stack_dispose(stack);
            free(stack);
            printf("Successfully ended\n");
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





void DLL_fill(DLList *list, token_t *token, dynamic_string *string) // testovacia funkcia - !! VYMAZAT !!
{
    do
    {
        get_token(token,string);
        DLL_InsertLast(list, token);
        printf("inserted[%s]\n", token->attribute);

    }while (token->type != TYPE_EOF);
    list->last->token->spec = DOLR;
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
void DLL_parse(DLList *list, token_t *token, dynamic_string *string) 
{
    DLL_Dispose(list);      // presitotu premaze pri znovu-uzivani
    DLList *pp_list = (DLList *) malloc (sizeof(DLList));   //pp_list pre parsovanie vyrazu
    if (pp_list == NULL)
        error(2);
    DLL_Init(pp_list);
    while(token->type != TYPE_EOF)
    {
        get_token(token, string);
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
        //get_token(token, string);
    }
    if (pp_list->last != NULL)
    {
        token->spec = DOLR;
        DLL_InsertLast( pp_list, token);
        parse_expression(pp_list);      //docasny vypis pre znazornenie prace PSA parseru
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
                    if (element->token->spec == SPEC_RETURN || element->token->spec == SPEC_IF || element->token->spec == SPEC_WHILE)
                    {
                        *token = *element->token;
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



//  ODKOMENTUJ PRE TESTOVANIE
///*
int main()
{
    return parser();
}
//*/

