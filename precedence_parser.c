
/* ************************* precedence_parser.c **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listop_elementad 2021                       */
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
  /*  #  */   {'_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '_',  '<',  '_',  '=',  '<',  '_',  '>'  }, // !!!???????
  /*  i  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '_',  '_',  '>',  '>'  },
  /*  (  */   {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '_',  '<',  '<',  '<',  '=',  '_'  },
  /*  )  */   {'>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '>',  '_',  '_',  '_',  '_',  '>',  '>'  },
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
    NLEN,  // '#'
    IDOP,  // 'i'
    OPEN,  // '('
    CLOS,  // ')'
    LAST,  // '$' len pre precedencnu tabulku

    MARK,  // nahradza '<' v zasobniku
    NONT, // znaci nonterminal pre pravidla a zasobnik
} Symbols;


// struktury pre zasobnik
typedef struct pp_stack_element{
    int token_n;
    token_t *token;
    struct pp_stack_element *next;
} pp_stack_element;


typedef struct pp_stack{
    pp_stack_element *top_element;
} pp_stack;





void stack_init(pp_stack* stack)
{
    stack->top_element = NULL;
}

void stack_push(pp_stack* stack, int token_n)     // pushne pre '='
{
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


pp_stack_element* stack_top(pp_stack* stack)
{

    return stack->top_element;
}



pp_stack_element* stack_top_term(pp_stack* stack) // zobrazi najvrchnejsi terminal
{

    pp_stack_element* stack_element = stack->top_element;
    while (stack_element != NULL)
    {
        if (stack_element->token_n != MARK && stack_element->token_n != NONT)
        {
            return stack_element;
        }

        stack_element = stack_element->next;
    }
    return NULL;
}

void stack_mark(pp_stack* stack, int token_n)   //oznaci zaciatok pravidla '<'
{
    pp_stack_element* left = NULL;
    pp_stack_element* stack_element = stack->top_element;

    while (stack_element != NULL)
    {
        if (stack_element->token_n != MARK && stack_element->token_n != NONT)
        {

            pp_stack_element* new_element = (pp_stack_element*)malloc(sizeof(pp_stack_element));
            new_element->token_n = token_n;

            if (left == NULL)
            {
                new_element->next = stack->top_element;
                stack->top_element = new_element;
            }
            else
            {
                new_element->next = left->next;
                left->next = new_element;
            }
            return;

        }

        left = stack_element;
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



void reduce(pp_stack* stack) // prevedie redukciu '>'
{
    //init stack
    pp_stack_element* stack_element;
    int token_n[3];
    int count = 0;
    while (stack_element != NULL)
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
            count++;
        }
    }
    // porovnavanie obsahu token_n[] podla rules

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
        code_n = NLEN;

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
        exit(99);
        break;
    case 2:
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
        break;
    case 99:
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
        break;

    }
    
}





int parse_expression(token_t *token, dynamic_string *string, token_t *tmp_token)
{
    int stack_n;        // hodnota na vrcholu zasobnika (lava strana suradnice vramci tabulky)
    int token_n;        // hodnota na aktualneho tokenu (prava strana suradnice vramci tabulky)

    int op_count = 0;   // pocitadlo operandov
    int count = 0;

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
           // op_count++;
        }
        else if (tmp_token->type == TYPE_OPERATOR)
        {
            token_n = convert(tmp_token->attribute);    //token_n = token->type_code
            //op_count = 0;
        }







        printf("expression   '%s'    (code %d)\n",tmp_token->attribute ,token_n);   //tu spracovavam token









        // zistuje ci uz nema koncit vyraz
        if(token->type == TYPE_KEYWORD || (token_n == IDOP && token->type == TYPE_IDENTIFIER) )
        {
            return 0;
        }

    }

}


