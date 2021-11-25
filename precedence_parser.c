
/* ************************* precedence_parser.c **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listop_elementad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include "precedence_parser.h"

#include "structures.h"


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
            case PLUS:
                /* rule 1 */
                return true;
                break;

            case MINU:
                /* rule 2 */
                return true;

            case MULT:
                /* rule 3 */
                return true;
                break;

            case DIVF:
                /* rule 4 */
                return true;
                break;

            case DIVI:
                /* rule 5 */
                return true;
                break;

            case LESS:
                /* rule 6 */
                return true;
                break;
            
            case LEEQ:
                /* rule 7 */
                return true;
                break;

            case GREA:
                /* rule 8 */
                return true;
                break;

            case GREQ:
                /* rule 9 */
                return true;
                break;

            case EQUA:
                /* rule 10 */
                return true;
                break;

            case NOEQ:
                /* rule 11 */
                return true;
                break;
            
            case CONC:
                /* rule 12 */
                return true;
                break;

            default:
                return false;
                break;
        }
    }
    else if (token_n[0] == -1 && token_n[1] == HASH && token_n[2] == NONT)
    {
        /* rule 13 */
        printf("( 13.)  E -> # E \n");
        return true;
    }
    else if (token_n[0] == -1 && token_n[1] == -1 && token_n[2] == IDOP)
    {
        /* rule 14 */
        printf("( 14.)  E -> <i>\n");
        return true;
    }
    else if (token_n[0] == OPEN && token_n[1] == NONT && token_n[2] == CLOS)
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
        code_n = PLUS;
    else if(strcmp(attribute,"-") == 0)
        code_n = MINU;
    else if(strcmp(attribute,"*") == 0)
        code_n = MULT;
    else if(strcmp(attribute,"/") == 0)
        code_n = DIVF;
    else if(strcmp(attribute,"//") == 0)
        code_n = DIVI;
    else if(strcmp(attribute,"<") == 0)
        code_n = LESS;
    else if(strcmp(attribute,"<=") == 0)
        code_n = LEEQ;
    else if(strcmp(attribute,">") == 0)
        code_n = GREA;
    else if(strcmp(attribute,">=") == 0)
        code_n = GREQ;
    else if(strcmp(attribute,"==") == 0)
        code_n = EQUA;
    else if(strcmp(attribute,"~=") == 0)
        code_n = NOEQ;
    else if(strcmp(attribute,"..") == 0)
        code_n = CONC;
    else if(strcmp(attribute,"#") == 0)
        code_n = HASH;

    //else if(strcmp(attribute,"i") == 0)
    //    code_n = IDOP;
    
    else if(strcmp(attribute,"(") == 0)
        code_n = OPEN;
    else if(strcmp(attribute,")") == 0)
        code_n = CLOS;

    //else if(strcmp(attribute,"$") == 0)
    //    code_n = LAST;

    return code_n;
}







void error(int err_num)
{
    switch (err_num)
    {
    case 1:
        fprintf(stderr, "Syntax error\n"); //delenie 0, zly zapis vyrazu,.. '_' v tabulke
        exit(1);
        break;
    case 2:
        fprintf(stderr, "Memory allocation error\n");
        exit(2);
        break;
    case 99:
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
        break;

    }
    
}









void parse_expression (DLList *list)
{
    int stack_n;        // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
    int token_n;        // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
    int count = 0;
   
    pp_stack *stack = (pp_stack*) malloc(sizeof(pp_stack));
    //token_t* token = malloc(sizeof(token_t));
    if (stack == NULL )//|| token == NULL)
        return -1;

    stack_init(stack);
    stack_push(stack, DOLR);

    DLLElement *element;

    element = list->first;


    while (true) //length of array
    {
        token_n = element->token->spec;
        element = element->next;

        stack_n = stack_top_term(stack);

        printf("%d : %d\nsymbol ..%c\n",stack_n,token_n,pp_table[stack_n][token_n]);

        switch (pp_table[stack_n][token_n])
        {
        case '=':
            stack_push(stack, token_n);
            token_n = element->token->spec;
            element = element->next;
            break;
        
        case '<':
            stack_mark(stack);
            stack_push(stack, token_n);
            token_n = element->token->spec;
            element = element->next;
            break;

        case '>':
            if (! reduce(stack)) printf("**Syntax error**\n");  // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            stack_push(stack, NONT);
            break;

        case '#':
            stack_dispose(stack);
            printf("Successfully ended\n");
            return 0;
            break;

        default:
            printf("**ERROR**\n");
            break;
        }
    }
    if (!stack_empty)
        printf("**Stack is not empty**\n");
    return 0;

}






int main()
{
    DLList *list = (DLList *)malloc (sizeof(DLList));
    token_t* token = malloc(sizeof(token_t));
    if (list == NULL || token == NULL)
        return -1;

    dynamic_string *string = string_init();

    get_token(token,string);
    DLL_fill(list,token,string);



    parse_expression(list);

    DLL_Dispose(list);
    free (list);
    free (token);

    return 0;
}




/*

// NEW VERSION



int main()
{

    int stack_n;        // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
    int token_n;        // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
    int count = 0;

    pp_stack *stack = (pp_stack*) malloc(sizeof(pp_stack));
    if (stack == NULL)
        return -1;

    stack_init(stack);
    stack_push(stack, DOLR);


    int tokens[20] = { OPEN, OPEN, HASH, OPEN, OPEN, IDOP, CONC, IDOP, CLOS, CONC, IDOP,CLOS,CLOS, LEEQ, IDOP, CLOS, DOLR};

    token_n = tokens[count];
    count++;

    while (true) //length of array
    {
        stack_n = stack_top_term(stack);

        printf("%d : %d\nsymbol ..%c\n",stack_n,token_n,pp_table[stack_n][token_n]);

        switch (pp_table[stack_n][token_n])
        {
        case '=':
            stack_push(stack, token_n);
            token_n = tokens[count];
            count++; // zjednot to
            break;
        
        case '<':
            stack_mark(stack);
            stack_push(stack, token_n);
            token_n = tokens[count];
            count++;
            break;

        case '>':
            if (! reduce(stack)) printf("**Syntax error**\n");  // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            stack_push(stack, NONT);
            break;

        case '#':

            stack_dispose(stack);

            printf("Successfully ended\n");
            return 0;
            break;

        default:
            printf("**ERROR**\n");
            break;
        }
    }
    if (!stack_empty) // ***
        printf("**Stack is not empty**\n");
    return 0;
}




// OLD VERSION

int parse_expression(token_t *token, dynamic_string *string, token_t *tmp_token)
{
    int stack_n;        // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
    int token_n;        // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)
    int count = 0;

    pp_stack *stack;
    stack_init(stack);
    stack_push(stack, DOLR);

// **************************************************** vyber tokenu pre expression *******************************************************
    get_token(token, string);

    if (token->type == TYPE_KEYWORD)
        return 0;

    while (token->type != TYPE_EOF)
    {
        count++;
        *tmp_token = *token;
        get_token(token, string);

        // zistuje ci nema vratit len priradenu funkciu alebo priradenie po returne
        if (count == 1 && tmp_token->type == TYPE_IDENTIFIER && (strcmp(token->attribute,"(") == 0 || strcmp(token->attribute,"=") == 0))
        {
            return 1;
        }

        // naplni vyraz
        if (tmp_token->type == TYPE_IDENTIFIER ||  tmp_token->type == TYPE_STRING || tmp_token->type == TYPE_DECIMAL || tmp_token->type == TYPE_INTEGER)  // token moze byt operand !!pozor zatvorky
        {   
            token_n = IDOP;
        }
        else if (tmp_token->type == TYPE_OPERATOR)
        {
            token_n = convert(tmp_token->attribute);    //token_n = token->type_code
        }
    // ********************************************************* aplikacia tokenu ******************************************************************************

        
        stack_n = stack_top_term(stack);



 switch (pp_table[stack_n][token_n])
        {
        case '=':
            stack_push(stack, token_n);
            //token_n = tokens[count];
            //count++; // zjednot to
            break;
        
        case '<':
            stack_mark(stack);
            stack_push(stack, token_n);
            //token_n = tokens[count];
            //count++;
            break;

        case '>':
            if (! reduce(stack)) error(1); // vyberie tokeny zo zasobniku az po MARK ('<'), porovna s pravidlami a redukuje
            stack_push(stack, NONT);
            break;

        case '#':
            stack_dispose(stack);
            return 0;
            break;

        default:
            error(1);
            break;
        }

        // tahanie znaku z tabulky table[stack_n][token_n]

        printf("expression   '%s'    (code %d)\n",tmp_token->attribute ,token_n);   //tu spracovavam token







        // zistuje ci uz nema koncit vyraz
        if(token->type == TYPE_KEYWORD || (token_n == IDOP && token->type == TYPE_IDENTIFIER) )
        {
            return 0;
        }

    }

}
*/


