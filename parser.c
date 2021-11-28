/* ******************************* parser.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a semanticky analyzator                                 */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"
// #include "precedence_parser.h" todo: resolve the issues while including PP

#define SUCCESS 1
#define FAILURE 0


int main(){
    token_t* new_token = malloc(sizeof(token_t));
    token_t* new_token_lookahead = malloc(sizeof(token_t));
    dynamic_string* string = string_init();
    printf("-- PARSING STARTED --\n");
    int result = program(new_token, new_token_lookahead, string);
    int syntax_control_result = result ? 0 : 1;
    printf("[STATUS CODE OF THE PARSING: %d]\n", syntax_control_result);
    if (!syntax_control_result) {
        printf("Syntax control successful.\n");
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
    result = prologue(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    return result && program_body(token, token_lookahead, string);

}
int prologue(token_t* token, token_t* token_lookahead, dynamic_string* string){
    get_token(token, string);
    if (!strcmp(token->attribute, "require"))
    {
        get_token(token_lookahead, string);
        if (!strcmp(token_lookahead->attribute, "\"ifj21\""))
        {
            return SUCCESS;
        }
    }
    return FAILURE;

}
int program_body(token_t* token, token_t* token_lookahead, dynamic_string* string){

    if (token_lookahead->type == TYPE_EOF)
    {
        // <program_body> -> e
        return SUCCESS;
    }
    moveAhead(token, token_lookahead, string);
    if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "global")))
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
    int result = 1;
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
        moveAhead(token, token_lookahead, string);
    }
    if (strcmp(token->attribute, ")"))
    {
        return FAILURE;
    }
    //moveAhead(token, token_lookahead, string);
//    if (strcmp(token->attribute, ":"))
//    {
//        return FAILURE;
//    }
//    //moveAhead(token, token_lookahead, string);
//    if (token_lookahead->type == TYPE_DATATYPE)
//    {
//        result = result && type_list(token, token_lookahead, string);
//        // do nothing for now
//    }
    if (!strcmp(token_lookahead->attribute, ":"))
    {
        // jestli je v lookaheadu dvojtecka, tak uzivatel specifikuje return values -> musim zkontrolovat type:list
        moveAhead(token, token_lookahead, string);
        moveAhead(token, token_lookahead, string);
        result = result && type_list(token, token_lookahead, string);
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

    //if (token_lookahead->type == TYPE_IDENTIFIER || !strcmp(token_lookahead->attribute, "global") || !strcmp(token_lookahead->attribute, "function") || !strcmp(token_lookahead->attribute, "end"))
    if (token_lookahead->type == TYPE_IDENTIFIER || token_lookahead->type == TYPE_KEYWORD || token_lookahead->type == TYPE_EOF)
    {
        // epsilon pravidlo pro konec vyctu typu mimo zavorky
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
        // epsilon pravidlo pro nula argumentu
        moveAhead(token, token_lookahead, string);
        return SUCCESS;
    }

    moveAhead(token, token_lookahead, string);
    // the current token is a constant - will need to work with that
    return constants(token, token_lookahead, string);
}

int constants(token_t* token, token_t* token_lookahead, dynamic_string* string) {
    if (!strcmp(token_lookahead->attribute, ")")){
        moveAhead(token, token_lookahead, string);
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
    //get_token(token, string);
    //get_token(token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, "(")) {
        return FAILURE;
    }
    result = param_list(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    if (!strcmp(token_lookahead->attribute, ":"))
    {
        // jestli je v lookaheadu dvojtecka, tak uzivatel specifikuje return values -> musim zkontrolovat type:list
        moveAhead(token, token_lookahead, string);
        moveAhead(token, token_lookahead, string);
        result = result && type_list(token, token_lookahead, string);
    }
    // zde bude kontrola func_body
    result = result && func_body(token, token_lookahead, string);
    if (strcmp(token->attribute, "end")){
        // both lines are hardcoded for testing
        return FAILURE;
    }
    return result;
}

int param_list(token_t* token, token_t* token_lookahead, dynamic_string* string){
    if (!strcmp(token_lookahead->attribute, ")"))
    {
        // epsilon pravidlo pro nula parametru
        return SUCCESS;
    }
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
    // now I am working with the var_ID
    moveAhead(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    return type(token, token_lookahead, string) && params(token, token_lookahead, string);
}

int func_body(token_t* token, token_t* token_lookahead, dynamic_string* string){
    if (token_lookahead->type == TYPE_EOF){
        return FAILURE;
    }
    if (!strcmp(token_lookahead->attribute, "end")){
        moveAhead(token, token_lookahead, string);
        return SUCCESS;
    }
    moveAhead(token, token_lookahead, string);
    if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "if"))){
        return if_element(token, token_lookahead, string) && func_body(token, token_lookahead, string);
    }
    //else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "while"))){
    //    return while_element(token, token_lookahead, string) && func_body(token, token_lookahead, string);
    //}
    return FAILURE;
}

int if_element(token_t* token, token_t* token_lookahead, dynamic_string* string){
    // the current token is "if"
    moveAhead(token, token_lookahead, string);
    if (strcmp(token->attribute, "EXPR"))
    {
        return FAILURE;
    }
    moveAhead(token, token_lookahead, string);
    if (token->spec != SPEC_THEN)
    {
        return FAILURE;
    }
    //moveAhead(token, token_lookahead, string);
    int result = func_body(token, token_lookahead, string);
    moveAhead(token, token_lookahead, string);
    if (token->spec != SPEC_ELSE)
    {
        return FAILURE;
    }
    result = result && func_body(token, token_lookahead, string);
    //moveAhead(token, token_lookahead, string);
    if (token->spec != SPEC_END)
    {
        return FAILURE;
    }
    return result;
    //int result = parse_expression()
}
