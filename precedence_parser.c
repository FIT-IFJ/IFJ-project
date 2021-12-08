/* ************************* precedence_parser.c **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */


#include "precedence_parser.h"

#define NUM_OF_SYMB 17


// struktury pre zasobnik
typedef struct pp_stack_element{
    token_t *token;
    struct pp_stack_element *next;
} pp_stack_element;


typedef struct pp_stack{
    pp_stack_element *top_element;
} pp_stack;


extern symtab_t* symtable;        // extern symtab_t* sym_table;  ??

void stack_init(pp_stack* stack)
{
    stack->top_element = NULL;
}



void stack_push(pp_stack* stack, token_t* token)     // pushne token_n, napr. pre '='
{
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    if (new_element == NULL)
        error(99, 0);
    new_element->next = stack->top_element;
    new_element->token = (token_t *) malloc(sizeof(token_t));
    if (new_element->token == NULL)
        error(99, 0);
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


pp_stack_element* stack_top(pp_stack* stack)
{
    return stack->top_element;
}



token_t* stack_top_term(pp_stack* stack) // zobrazi najvrchnejsi terminal
{
    pp_stack_element* stack_element = stack->top_element;
    
    while(stack_element->token->spec == SPEC_NONT)   // pozor musis vytvorit token->spec = SPEC_NONT
    {
        stack_element = stack_element->next;
    }
    return stack_element->token;
} 


void stack_mark(pp_stack* stack)   //oznaci zaciatok pravidla '<'
{
    pp_stack_element* stack_element = stack->top_element;
    pp_stack_element* new_element = (pp_stack_element*) malloc(sizeof(pp_stack_element));
    if (new_element == NULL)
        error(99, 0);
    new_element->token = (token_t *) malloc(sizeof(token_t));
    if (new_element->token == NULL)
        error(99, 0);

    while (stack_element != NULL)
        {
            if (stack_element == stack->top_element && stack_element->token->spec != SPEC_NONT)
            {
                new_element->next = stack_element;
                stack->top_element = new_element;
                new_element->token->attribute = "[<]"; // for debug
                new_element->token->spec = SPEC_MARK;
                return;
            }
            else if (stack_element->next->token->spec != SPEC_NONT)
            {
                new_element->next = stack_element->next;
                stack_element->next = new_element;
                new_element->token->attribute = "[<]";  // for debug
                new_element->token->spec = SPEC_MARK;
                return;
            }
            stack_element = stack_element->next;
        }

}


void stack_dispose(pp_stack* stack)
{
    while (stack->top_element != NULL)
      stack_pop(stack);
}




void push_nont(pp_stack* stack, token_t* nont_token) // vlozi nonterminal 'E'
{ 
    token_t* token = malloc(sizeof(token_t));
    if (token == NULL)
        error(99, 0);
    
    *token = *nont_token;       // gets line and type from source token
    token->spec = SPEC_NONT;
    stack_push(stack, token);
    free(token);

}



/**
 * @brief Funkcia je zavolana po vycitani znaku '>' z tabulky, to znamena, ze na vrchu zasobniku sa nachadza prava strana pravidla, ktore sa moze redukovat.
 * Funkcia zaroven spracuva semantiku typovania a delenia nulou.
 * 
 * @param stack Ukazatel na inicializovanu strukturu zasobniku, vyuzivanu pri precedencii
 * @param AST_list Ukazatel na inicializovanu DLL strukturu, cez ktoru parser preda generatoru rozparsovany a skontrolovany vyraz (postfix).
 */
void reduce(pp_stack* stack,DLList* AST_list) // prevedie redukciu '>' podla pravidla
{
    token_t* nont_token = malloc(sizeof(token_t));
    if ( nont_token == NULL )
        error(99, 0);
    pp_stack_element* stack_element;
    token_t *token[3];
    for (int i = 0; i < 3; i++)
    {
        token[i] = malloc(sizeof(token_t));
        if (token == NULL)
            error(99, 0);
        token[i]->spec = SPEC_EMPTY;
    }
    
    int count = 2;
    do
    {
        stack_element = stack_top(stack);        
        if (stack_element->token->spec == SPEC_MARK)
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
    if (token[0]->spec == SPEC_NONT && token[2]->spec == SPEC_NONT)
    {

        switch (token[1]->spec)
        {
            /* rule 1: [ E -> E + E ] */
            case SPEC_PLUS:
                
            /* rule 2: [ E -> E - E ] */
            case SPEC_MINU:

            /* rule 3: [ E -> E * E ] */
            case SPEC_MULT:
                // spravanie dat. typov pri '+','-','*': int + int = int, int + dec = dec, dec + dec = dec, else error
                *nont_token = *token[0];

                if (token[0]->type == TYPE_INTEGER && token[2]->type == TYPE_INTEGER)
                    nont_token->type = TYPE_INTEGER;
                else if ((token[0]->type == TYPE_INTEGER || token[0]->type == TYPE_DECIMAL) && (token[2]->type == TYPE_INTEGER || token[2]->type == TYPE_DECIMAL))
                    nont_token->type = TYPE_DECIMAL;
                else
                    error(6, token[0]->line);

                if (strcmp(token[0]->attribute, "0") == 0 && strcmp(token[2]->attribute, "0") == 0 )
                    nont_token->attribute = "0";
                else if (token[1]->spec == SPEC_MULT && ( strcmp(token[0]->attribute, "0") == 0 || strcmp(token[2]->attribute, "0") == 0 ))
                    nont_token->attribute = "0";
                else
                    nont_token->attribute = "E";

                break;


            /* rule 4: [ E -> E / E ] */
            case SPEC_DIVF:
                // spravanie dat. typov pri '/': int / int = dec, int / dec = dec, dec / dec = dec, else error
                *nont_token = *token[0];

                if ((token[0]->type == TYPE_INTEGER || token[0]->type == TYPE_DECIMAL) && (token[2]->type == TYPE_INTEGER || token[2]->type == TYPE_DECIMAL))
                    nont_token->type = TYPE_DECIMAL;
                else
                    error(6, token[0]->line);
                
                if (strcmp(token[0]->attribute, "0") != 0)
                    nont_token->attribute = "E";
                if (strcmp(token[2]->attribute, "0") == 0)
                    error(9, token[2]->line);

                break;


            /* rule 5: [ E -> E // E ] */
            case SPEC_DIVI:
                // spravanie dat. typov pri '//': int // int = int, else error
                *nont_token = *token[0];

                if (token[0]->type == TYPE_INTEGER && token[2]->type == TYPE_INTEGER)
                    nont_token->type = TYPE_INTEGER;
                else
                    error(6, token[0]->line);

                if (strcmp(token[0]->attribute, "0") != 0)
                    nont_token->attribute = "E";
                if (strcmp(token[2]->attribute, "0") == 0)
                    error(9, token[2]->line);

                break;


            /* rule 6: [ E -> E < E ] */
            case SPEC_LESS:
            
            /* rule 7: [ E -> E <= E ] */
            case SPEC_LEEQ:

            /* rule 8: [ E -> E > E ] */
            case SPEC_GREA:

            /* rule 9: [ E -> E >= E ] */
            case SPEC_GREQ:
                // spravanie dat. typov pri '<','<=','>','>=': int < int = bool, int < dec = bool, dec < dec = bool, string < string = bool, else error
                *nont_token = *token[0];

                if ((token[0]->type == TYPE_INTEGER || token[0]->type == TYPE_DECIMAL) && (token[2]->type == TYPE_INTEGER || token[2]->type == TYPE_DECIMAL))
                    nont_token->type = TYPE_BOOL;
                else if (token[0]->type == TYPE_INTEGER && token[2]->type == TYPE_STRING)
                    nont_token->type = TYPE_BOOL;
                else
                    error(6, token[0]->line);

                nont_token->attribute = "E";

                break;

            /* rule 10: [ E -> E == E ] */
            case SPEC_EQUA:

            /* rule 11: [ E -> E ~= E ] */
            case SPEC_NOEQ:
                // spravanie dat. typov pri '==','~=': int ~= int = bool, int ~= dec = bool, dec ~= dec = bool, string ~= string = bool (a zaroven miesto ktorehokolvek z dec, int, string moze byt nil), else error
                *nont_token = *token[0];

                if ((token[0]->type == TYPE_INTEGER || token[0]->type == TYPE_DECIMAL || token[0]->type == TYPE_NIL) && (token[2]->type == TYPE_INTEGER || token[2]->type == TYPE_DECIMAL || token[2]->type == TYPE_NIL))
                    nont_token->type = TYPE_BOOL;
                else if ((token[0]->type == TYPE_INTEGER || token[0]->type == TYPE_NIL) && (token[2]->type == TYPE_STRING || token[2]->type == TYPE_NIL))
                    nont_token->type = TYPE_BOOL;
                else
                    error(6, token[0]->line);

                nont_token->attribute = "E";

                break;

             /* rule 12: [ E -> E .. E ] */
            case SPEC_CONC:
                // spravanie dat. typov pri '..': string .. string = string, else error
                *nont_token = *token[0];
                if (token[0]->type == TYPE_STRING && token[2]->type == TYPE_STRING)
                    nont_token->type = TYPE_STRING;
                else
                    error(6, token[0]->line);

                nont_token->attribute = "E";

                break;

            default:
                error(2, token[0]->line);
                break;
        }
        
        // printf("( %i.)  E -> E %s E\n",token[1]->spec, token[1]->attribute);    ///
        DLL_InsertLast(AST_list,token[1]);
    }
    /* rule 13:  [ E -> # E ] */
    else if ( token[0]->spec ==  SPEC_EMPTY && token[1]->spec == SPEC_HASH && token[2]->spec == SPEC_NONT)
    {
        //printf("( %i.)  E -> %s E\n",token[1]->spec, token[1]->attribute);      ///
        DLL_InsertLast(AST_list,token[1]);
        *nont_token = *token[2];
        if (token[2]->type == TYPE_STRING)
            nont_token->type = TYPE_INTEGER;
        else
            error(6, token[1]->line);
    }
    /* rule 14:  [ E -> i ] */
    else if (token[0]->spec == SPEC_EMPTY && token[1]->spec == SPEC_EMPTY && token[2]->spec == SPEC_IDOP)
    {
        //printf("( %i.)  E -> '%s' \n",token[2]->spec, token[2]->attribute);     ///
        DLL_InsertLast(AST_list,token[2]);

        *nont_token = *token[2];
        if ( token[2]->type == TYPE_IDENTIFIER )
            {
                if (symtable != NULL){
                    nont_token->type = get_var_datatype(symtable, token[2]->attribute);
                }
                else
                    nont_token->type = TYPE_INTEGER;    //// !!!!!!!! symtable instead - do not forget to delete this
            }
        if (strcmp(token[2]->attribute, "0") != 0)    // if it is 0, it gets it as attribute from source token
            nont_token->attribute = "E";
    }
    /* rule 15:  [ E -> ( E ) ] */
    else if (token[0]->spec == SPEC_OPEN && token[1]->spec == SPEC_NONT && token[2]->spec == SPEC_CLOS)
    {
        //printf("( 15.)  E -> ( E )\n");     ///
        *nont_token = *token[1];
    }
    else
        error(2, token[2]->line);


    push_nont(stack, nont_token);
    free(nont_token);
    for (int i = 0; i < 3; i++)
    {
        free(token[i]);
    }
}



void print_stack(pp_stack* stack)   // for debug
{
    pp_stack_element* stack_element;
    DLLElement* DLL_element;
    DLList *list = (DLList *)malloc (sizeof(DLList));
    if (list == NULL)
        error(99, 0);
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
 * @brief Funkcia parsuje vyraz precedencnou analyzou zalozenej na precedencnej tabulke pp_table.
 * Je zavolana v DLL_parse po naplneni DLL zoznamu tokenmi jedneho vyrazu,
 * ukonceny tokenom s token->spec = SPEC_DOLR. Tento DLL tak reprezentuje vstupnu pasku vyrazu.
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu, naplnenu tokenmi jedneho vyrazu a ukoncenu s $ (vstupna paska s tokenmi)
 * @param AST_list Ukazatel na inicializovanu DLL strukturu, cez ktoru parser preda generatoru rozparsovany a skontrolovany vyraz (postfix).
 */
int parse_expression (DLList *list, DLList *AST_list)
{

    char pp_table[NUM_OF_SYMB][NUM_OF_SYMB]={ 
    /*            +     -     *     /     //    <    <=     >    >=    ==    ~=    ..     #     i     (     )     $   */
    /*  +  */   {'>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
    /*  -  */   {'>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
    /*  *  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
    /*  /  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
    /*  // */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
    /*  <  */   {'<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '<',  '<',  '<',  '>',  '>'  },
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
    /*  $  */   {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '_',  '#'  } 
    };


    int ret_code;
    pp_stack *stack = (pp_stack*) malloc(sizeof(pp_stack));
    token_t* token = malloc(sizeof(token_t));
    if (stack == NULL || token == NULL)
        error(99, 0);

    stack_init(stack);

    token->spec = SPEC_DOLR;
    token->attribute = "$"; // for debug
    stack_push(stack, token);

    DLLElement *element;
    element = list->first;
    *token = *element->token; // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
    int stack_n;            // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
   

    while (element != NULL)
    {
        stack_n = (stack_top_term(stack))->spec; //// stack_token = stack_top_term(stack); 

        //print_stack(stack); //for debug
        //printf("\n[stack: %d '%s']  vs  [token: %d '%s']\n\ntable symbol [%c]\n\n",stack_n,(stack_top_term(stack))->attribute,token->spec,token->attribute,pp_table[stack_n][token->spec]); // for debug

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

        case '>':
            reduce(stack, AST_list);  // vyberie tokeny zo zasobniku az po SPEC_MARK ('<'), porovna s pravidlami a redukuje
            break;

        case '#':
            ret_code = (stack_top(stack))->token->type;
            //printf("***** TYPE: %d *****\n",ret_code);
            stack_dispose(stack);
            free(token);
            free(stack);
            //printf("Parsing of expression was successfull!!\n");
            return ret_code;
            break;

        default:
            stack_dispose(stack);
            free(token);
            free(stack);
            error(2, token->line);
            break;
        }
    }

}



/**
 * @brief Funkcia je presunie token z konca zdrojoveho zoznamu na zaciatok cieloveho zoznamu.
 * Tento token sa zaroven po presunuti vymaze zo zdrojoveho zoznamu.
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
 * @brief Funkcia je zavolana z parseru po tokene "if", "while", "return" a "=".
 * Naplni pp_list a posunie ho do PSA parseru funkciou parse_expression(pp_list)
 * Ak narazi na klucove slovo alebo situaciu vylucujucu vyskyt vyrazu, tokeny nepatriace
 * do pp_listu uberie a prida do listu, z ktoreho si to vyberie hlavny parser.
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu, ktora sa plni pre parser
 * @param token Ukazatel na inicializovanu strukturu tokenu pre funkciu get_token(..)
 * @param string --||--
 */
int DLL_parse(DLList *list, token_t *token, dynamic_string *string, DLList *AST_list) 
{
    int ret_code;
    DLL_Dispose(list);      // presitotu premaze pri znovu-uzivani
    DLL_Dispose(AST_list);      // presitotu premaze pri znovu-uzivani
    DLList *pp_list = (DLList *) malloc (sizeof(DLList));   //pp_list pre parsovanie vyrazu
    if (pp_list == NULL)
        error(99, 0);
    DLL_Init(pp_list);
    while(token->type != TYPE_EOF)
    {
        if (token->spec == SPEC_NIL)
        {
            token->spec = SPEC_IDOP; // nil je v precedencnom parseri spracovavany ako operand
            token->type = TYPE_NIL;  // v semantike je treba pristup k jeho typu NIL
        }
        DLL_InsertLast(pp_list, token);
        
         //ak nasleduje keyword, tak vymaze posledny token a vlozi ho do listu pre hlavny parser
        if (pp_list->last->token->type == TYPE_KEYWORD || pp_list->last->token->type == TYPE_DATATYPE || strcmp(pp_list->last->token->attribute,",") == 0)     
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
        //printf("\n_____________________________PP_PARSER________________________________\n");
        token->spec = SPEC_DOLR;
        token->attribute = "$";
        token->line = -1;
        DLL_InsertLast( pp_list, token);
        ret_code = parse_expression(pp_list, AST_list);      //docasny vypis pre znazornenie prace PSA parseru
        //DLL_print(AST_list);            // for debug
        //printf("\n____________________________MAIN_PARSER_______________________________\n");
    }
    DLL_Dispose(pp_list);               // vymazanie pp_listu po tom ako su vyrazy spracovane
    free(pp_list);
    return ret_code;
}














// **************************************************************************

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
    int type_code;

    do
    {
        get_token(token,string);
        //printf("\n---> '%s',%d,%d\n",token->attribute,token->type,token->spec);   // cinnost parseru

        if (token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->spec == SPEC_WHILE || token->type == TYPE_ASSIGNMENT) // Tejd si sam triedi
        {
            again:
            get_token(token,string);
            type_code = DLL_parse(list,token,string,AST_list);   // volanie pre parsovanie pripadneho vyrazu
            if (list->last != NULL)
            {
                element = list->first;
                while(element != NULL)      // v liste sa nachadza omylom ukradnuty token pre parser
                {
                    //printf("\n---> '%s',%d,%d\n*TYPE %d*\n",element->token->attribute,element->token->type,element->token->spec,type_code);    // cinnost parseru
                    if (element->token->spec == SPEC_RETURN || element->token->spec == SPEC_IF || element->token->spec == SPEC_WHILE || strcmp(element->token->attribute,",") == 0) // moze obsahovat "keyword","datatype","identifier"
                        goto again;
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

