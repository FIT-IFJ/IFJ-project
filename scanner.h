/* ******************************* scanner.h ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Lexikalni analyzator                                                */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamic_string.h"

typedef enum {
    // datove typy (aj ako navratove hodnoty z PP pri kontrole typu)
    TYPE_NIL,       // pre semantiku v PP
    TYPE_INTEGER,
    TYPE_DECIMAL,   
    TYPE_STRING,    
    TYPE_BOOL,      // pre semantiku v PP

    // ostatne
    TYPE_OPERATOR,
    TYPE_IDENTIFIER,
    TYPE_KEYWORD,
    TYPE_DATATYPE,
    TYPE_ASSIGNMENT, // '='
    TYPE_EOF, // konec souboru

} TokenType;

typedef enum {
    // symboly pre precedencny parser
    SPEC_PLUS,  // '+' 
    SPEC_MINU,  // '-' 
    SPEC_MULT,  // '*' 
    SPEC_DIVF,  // '/'  
    SPEC_DIVI,  // '//' 
    SPEC_LESS,  // '<'  
    SPEC_LEEQ,  // '<='
    SPEC_GREA,  // '>'  
    SPEC_GREQ,  // '>='
    SPEC_EQUA,  // '=='
    SPEC_NOEQ,  // '~='
    SPEC_CONC,  // '..' 
    SPEC_HASH,  // '#' 
    SPEC_IDOP,  // 'i' ako operand
    SPEC_OPEN,  // '(' 
    SPEC_CLOS,  // ')' 

    SPEC_DOLR,  // koniec zasobniku/vyrazu
    SPEC_MARK,  // znacenie zaciatku vyrazu '<' vramci zasobniku
    SPEC_NONT,  // 'E' ako nonterminal
    SPEC_EMPTY, // prazdny token->spec

    // KEYWORDS
    SPEC_DO,
    SPEC_ELSE,
    SPEC_END,
    SPEC_FUNCTION,
    SPEC_GLOBAL,
    SPEC_IF,   
    SPEC_LOCAL,
    SPEC_REQUIRE,  
    SPEC_RETURN, 
    SPEC_THEN, 
    SPEC_WHILE,

    // DATATYPE
    SPEC_NUMBER,
    SPEC_INTEGER,
    SPEC_NIL,
    SPEC_STRING,

    SPEC_OTHERS, // ostatni

} Specification;

// Token (obsahuje: typ tokenu, hodnotu, umístění v kódu)
typedef struct {
TokenType type;
Specification spec;
char* attribute;
int line;
} token_t;

void lex_error(token_t *new_token, dynamic_string *string);
void create_operator_token(token_t *new_token, dynamic_string *string);
void token_operator_sort(token_t *new_token, dynamic_string *string, char current);
bool is_valid_char(char c);
void detail_spec(token_t *new_token, int num);
void create_word_token(token_t *new_token, dynamic_string *string);
void general_sequence(token_t *new_token, dynamic_string *string, char c);
void string_token(token_t *new_token,dynamic_string *string);
void make_exponent_token(token_t *new_token, dynamic_string *string);
void make_exponent_num(token_t *new_token, dynamic_string *string);
void create_num_token(token_t *new_token, dynamic_string *string);
void detect_block_comment();
void detect_comment();
void get_token(token_t *new_token, dynamic_string *string);
