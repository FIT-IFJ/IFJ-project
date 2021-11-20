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
    TYPE_OPERATOR,
    TYPE_STRING,
    TYPE_INTEGER,
    TYPE_DECIMAL,
    TYPE_IDENTIFIER,
    TYPE_KEYWORD,
    TYPE_DATATYPE,
    TYPE_ASSIGNMENT, // '='
    TYPE_EOF, // konec souboru

} TokenType;

// Token (obsahuje: typ tokenu, hodnotu, umístění v kódu)
typedef struct {
TokenType type;
char* attribute;
int line;
} token_t;

void lex_error(token_t *new_token, dynamic_string *string);
void create_operator_token(token_t *new_token, dynamic_string *string);
void token_operator_sort(token_t *new_token, dynamic_string *string, char current);
bool is_valid_char(char c);
void create_word_token(token_t *new_token, dynamic_string *string);
void general_sequence(token_t *new_token, dynamic_string *string, char c);
void string_token(token_t *new_token,dynamic_string *string);
void make_exponent_token(token_t *new_token, dynamic_string *string);
void make_exponent_num(token_t *new_token, dynamic_string *string);
void create_num_token(token_t *new_token, dynamic_string *string);
void detect_block_comment();
void detect_comment();
void get_token(token_t *new_token, dynamic_string *string);
