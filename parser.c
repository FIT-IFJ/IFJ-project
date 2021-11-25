/* ******************************* parser.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a semanticky analyzator                                  */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"

#define SUCCESS 1
#define FAILURE 0


int main(){
    token_t* new_token = malloc(sizeof(token_t));
    token_t* new_token_lookahead = malloc(sizeof(token_t));
    dynamic_string* string = string_init();
    int result = program(new_token, new_token_lookahead, string);
    printf("[STATUS CODE OF THE PARSING: %d]\n", result);
    if (result == SUCCESS) {
        printf("Your program seems OK.\n");
    }
    return 0;
}
void moveAhead(token_t* token, token_t* token_lookahead, dynamic_string* string){
    *token = *token_lookahead;
    get_token(token_lookahead, string);
}
void report_error(char* msg, int line){
    printf("Syntax error: %s on line %d\n", msg, line);
}
int program(token_t* token, token_t* token_lookahead, dynamic_string* string){
    int result;
    result = prologue(token, string) && program_body(token, token_lookahead, string);
    return result;
}
int prologue(token_t* token, dynamic_string* string){
    get_token(token, string);
    printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
    if (!strcmp(token->attribute, "require"))
    {
        get_token(token, string);
        printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
        if (!strcmp(token->attribute, "\"ifj21\""))
        {
            return SUCCESS;
        }
    }
    return FAILURE;

}
int program_body(token_t* token, token_t* token_lookahead, dynamic_string* string){
    //int result = 0;
    get_token(token, string);
    printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
    get_token(token_lookahead, string);
    printf("THE CURRENT LOOKAHEAD IS: %s on line %d\n\n", token_lookahead->attribute, token_lookahead->line);
    // ---
    // <program_body> -> <func_decl><program_body>
    // <program_body> -> <func_def><program_body>
    // <program_body> -> <func_call><program_body>
    // <program_body> -> e
    // ---
    if (token->type == TYPE_EOF)
    {
        // <program_body> -> e
        return SUCCESS;
    }
    else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "global")))
    {
        // <program_body> -> <func_decl><program_body>
        return func_decl(token, token_lookahead, string) && program_body(token, token_lookahead, string);
    }
    else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "function")))
    {
        // <program_body> -> <func_def><program_body>
        return func_def(token, token_lookahead, string) && program_body(token, token_lookahead, string);
    }
    else if ((token->type == TYPE_IDENTIFIER) && (token_lookahead->type == TYPE_OPERATOR) && (!strcmp(token_lookahead->attribute, "(")))
    {
        // <program_body> -> <func_call><program_body>
        return func_call(token, token_lookahead, string) && program_body(token, token_lookahead, string);
    }
    return FAILURE;
}
int func_decl(token_t* token, token_t* token_lookahead, dynamic_string* string)
{
    // token_lookahead == FUNC_ID -> zpracovat se symboltable
    int result = 0;
    get_token(token, string);
    get_token(token_lookahead, string);
    if (strcmp(token->attribute, ":"))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, "function"))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, "("))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    if (token->type == TYPE_DATATYPE)
    {
        result = type_list(token, token_lookahead, string);
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ")"))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ":"))
    {
        return FAILURE;
    }
    //moveAhead(token, token_lookahead, string);
    if (token_lookahead->type == TYPE_DATATYPE)
    {
        //result = type_list();
        // do nothing for now
    }
    //result = 1; // FOR DEBUGGING PURPOSES
    return result;

}

int func_call(token_t* token, token_t* token_lookahead, dynamic_string* string){
    // the current TOKEN is the ID of function
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, "("))
    {
        return FAILURE;
    }
    //if (token_lookahead->type == TYPE_STRING) // THIS IS GONNA NEED TO CHECK FOR ALL THE CONSTANTS, now only accepts strings
    return constant_list(token, token_lookahead, string);

}

int type_list(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    return type(token, token_lookahead, string) && types(token, token_lookahead, string);
}

int types(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ",")){
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    return type(token, token_lookahead, string) && types(token, token_lookahead, string);

}

int type(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    // if ")" here should deal with the epsilon rule = zero types given
    if (token->type == TYPE_DATATYPE){
        return SUCCESS;
    }
    return FAILURE;
}

int constant_list(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    if (!strcmp(token_lookahead->attribute, ")"))
    {
        // epsilon pravidlo
        return SUCCESS;
    }

    moveAhead(token, token_lookahead, string);
    // the current token is a constant - will need to work with that
    return constants(token, token_lookahead, string);
}

int constants(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ",")){
        report_error("expected comma", token->line);
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    // now I am working with the CONSTANT
    return constants(token, token_lookahead, string);

}
// aktualni TODO - vyresit epsilon pravidla pro argumenty deklaraci, definici a volani fci

int func_def(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    int result = 0;
    // token_lookahead == FUNC_ID -> zpracovat se symboltable
    get_token(token, string);
    get_token(token_lookahead, string);
    if (strcmp(token->attribute, "(")) {
        return FAILURE;
    }
    result = param_list(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    if (!strcmp(token_lookahead->attribute, ":"))
    {
        // jestli je v lookaheadu dvojtecka, tak uzivatel specifikuje return values -> musim zkontrolovat type:list
        result = result && type_list(token, token_lookahead, string);
    }
    if (!strcmp(token_lookahead->attribute, "end")){
        return SUCCESS;
    }
    return result;
}

int param_list(token_t* token, token_t* token_lookahead, dynamic_string* string){
    int result = 0;
    moveAhead(token, token_lookahead, string);
    // the current token is an ID
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ":"))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    result = type(token, token_lookahead, string);
    return  result && params(token, token_lookahead, string);

}
int params(token_t* token, token_t* token_lookahead, dynamic_string* string){
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, ",")){
        report_error("expected comma", token->line);
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    // now I am working with the CONSTANT
    return constants(token, token_lookahead, string);
}