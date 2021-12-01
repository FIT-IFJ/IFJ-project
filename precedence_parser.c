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



/*
typedef enum SYMBOLS {
    PLUS,  // '+'
    MINU,  // '-'
    MULT,  // '*'
    DIVF,  // '/'
    DIVI,  // '//'
    LESS,  // '<'
    LEEQ,  // '<='
    GREA,  // '>'
    GREQ,  // '>='
    EQUA,  // '=='
    NOEQ,  // '~='
    CONC,  // '..'
    HASH,  // '#'
    IDOP,  // 'i'
    OPEN,  // '('
    CLOS,  // ')'
    DOLR,  // '$' len pre precedencnu tabulku

    MARK,  // nahradza '<' v zasobniku
    NONT, // znaci nonterminal pre pravidla a zasobnik
} Symbols;

*/



// struktury pre zasobnik
typedef struct pp_stack_element{
    int token_n;
    token_t *token; // do I need it???
    struct pp_stack_element *next;
} pp_stack_element;


typedef struct pp_stack{
    pp_stack_element *top_element;
} pp_stack;





void stack_init(pp_stack* stack)
{
    stack->top_element = NULL;
}


void stack_push(pp_stack* stack, int token_n)     // pushne token_n, napr. pre '='
{
    printf("_____\n| %d |\n_____\n",token_n);
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    if (new_element != NULL)
    {
        new_element->next = stack->top_element;
        new_element->token_n = token_n;

        stack->top_element = new_element;
    }
}


void stack_pop(pp_stack* stack)
{
    if (stack->top_element != NULL)
    {
        pp_stack_element* stack_element = stack->top_element;
        stack->top_element = stack_element->next;
        free(stack_element);
    }
}


pp_stack_element* stack_top(pp_stack* stack) // ??
{

    return stack->top_element;
}



int stack_top_term(pp_stack* stack) // zobrazi najvrchnejsi terminal
{

    pp_stack_element* stack_element = stack->top_element;
    
    while(stack_element->token_n == NONT)
    {
        stack_element = stack_element->next;
    }
    return stack_element->token_n;
} 


void stack_mark(pp_stack* stack)   //oznaci zaciatok pravidla '<'
{
    pp_stack_element* stack_element = stack->top_element;
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    while (stack_element != NULL)
        {
            if (stack_element == stack->top_element && stack_element->token_n != NONT)
            {
                new_element->next = stack_element;
                stack->top_element = new_element;
                new_element->token_n = MARK;
                return;
            }
            else if (stack_element->next->token_n != NONT)
            {
                new_element->next = stack_element->next;
                stack_element->next = new_element;
                new_element->token_n = MARK;
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
    int token_n[3]= {-1,-1,-1};
    int count = 2;
    do
    {
        stack_element = stack_top(stack);
        if (stack_element->token_n == MARK)
        {
            stack_pop(stack);
            break;
        }
        else
        {
            token_n[count] = stack_element->token_n;
            stack_pop(stack);
            count--;
        }
    }while (stack_element != NULL || count <= 0);


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








void parse_expression (DLList *list)
{
    int stack_n;        // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
    int token_n;        // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
   
    pp_stack *stack = (pp_stack*) malloc(sizeof(pp_stack));
    //token_t* token = malloc(sizeof(token_t));
    if (stack == NULL )//|| token == NULL)
        error(2);

    stack_init(stack);
    stack_push(stack, DOLR);

    DLLElement *element;
    element = list->first;
    token_n = element->token->spec;


    while (true) //length of array
    {
        stack_n = stack_top_term(stack);

        printf("%d : %d\nsymbol ..%c\n",stack_n,token_n,pp_table[stack_n][token_n]);

        switch (pp_table[stack_n][token_n])
        {
        case '=':
            stack_push(stack, token_n);
            element = element->next;
            token_n = element->token->spec;
            break;
        
        case '<':
            stack_mark(stack);
            stack_push(stack, token_n);
            element = element->next;
            token_n = element->token->spec;
            break;

        case '>':
            if (! reduce(stack)) printf("**Syntax error**\n");  // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            stack_push(stack, NONT);
            break;

        case '#':
            stack_dispose(stack);
            printf("Successfully ended\n");
            return;
            break;

        default:
            printf("**ERROR**\n");
            break;
        }
    }
    if (!stack_empty)
        printf("**Stack is not empty**\n");
    return;

}




// ***********************************************************************************************************************************************************************





void DLL_fill(DLList *list, token_t *token, dynamic_string *string) // funkcia je zavolana po tokene "if", "while", "return" a "="
{
    do
    {
        get_token(token,string);
        DLL_InsertLast(list, token);
        printf("inserted[%s]\n", token->attribute);



        //if (token->type == TYPE_KEYWORD)        //      zakomentuj pre naplnenie DLL az do konca (nerobi to potom tolko chyb XD)
        //    return;                             //
        
    }while (token->type != TYPE_EOF);
    list->last->token->spec = DOLR;
}






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



//  ODKOMENTUJ PRE TESTOVANIE
///*
int main()
{
    return parser();
}
//*/

