/* ******************************* parser.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a semanticky analyzator                                 */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#ifndef SCANNER_H
#define SCANNER_H
#include "scanner.h"
#endif

#include "symtab.h"
#include "parser.h"
#include "precedence_parser.h"


#define SUCCESS 1
#define FAILURE 0
bool podezrely_token = false;
bool from_func_def = false;
symtab_t* symtable;


int main(){
    token_t* new_token = malloc(sizeof(token_t));
    token_t* new_token_lookahead = malloc(sizeof(token_t));
    dynamic_string* dyn_string = string_init();
    ast_node_t* AST;
    symtable = symtab_create();
    start_block(symtable);
    AST_insert_root(&AST);
    printf("-- PARSING STARTED --\n");
    int result = program(new_token, new_token_lookahead, dyn_string, AST);
    int syntax_control_result = result ? 0 : 1;
    //printf("[STATUS CODE OF THE PARSING: %d]\n", syntax_control_result);
    if (!syntax_control_result) {
        printf("Syntax control successful.\n");
    }
    return 0;
}
void moveAhead(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string){
    *token = *token_lookahead;
    get_token(token_lookahead,dyn_string);
}
void report_error(char* msg, int line){
    printf("Syntax error: %s on line %d\n", msg, line);
}

void import_builtins(symtab_t* symtable){
    char* nazev_fce[] = {"write", "read"};
    for (int i = 0; i < 2; ++i) {

    }

}
int program(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* AST){
    int result;
    result = prologue(token, token_lookahead,dyn_string);
    moveAhead(token, token_lookahead,dyn_string);
    return result && program_body(token, token_lookahead, dyn_string, AST);

}
int prologue(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string){
    get_token(token,dyn_string);
    if (!strcmp(token->attribute, "require"))
    {
        get_token(token_lookahead,dyn_string);
        if (!strcmp(token_lookahead->attribute, "\"ifj21\""))
        {
            return SUCCESS;
        }
    }
    //error(2, token->line);
    error(2, token->line);
}
int program_body(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){

    if (token_lookahead->type == TYPE_EOF)
    {
        // <program_body> -> e
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "global")))
    {
        // <program_body> -> <func_decl><program_body>
        return func_decl(token, token_lookahead, dyn_string, parent_node) && program_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "function")))
    {
        // <program_body> -> <func_def><program_body>
        return func_def(token, token_lookahead, dyn_string, parent_node) && program_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if ((token->type == TYPE_IDENTIFIER) && (token_lookahead->type == TYPE_OPERATOR) && (!strcmp(token_lookahead->attribute, "(")))
    {
        // <program_body> -> <func_call><program_body>
        return func_call(token, token_lookahead, dyn_string, parent_node) && program_body(token, token_lookahead, dyn_string, parent_node);
    }
    error(2, token->line);
}
int func_decl(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node)
{
    // token_lookahead == FUNC_ID -> zpracovat se symboltable
    int result = 1;
    get_token(token,dyn_string);
    get_token(token_lookahead,dyn_string);
    if (strcmp(token->attribute, ":"))
    {
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, "function"))
    {
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, "("))
    {
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (token->type == TYPE_DATATYPE)
    {
        // PC
        result = type_list(token, token_lookahead, dyn_string, parent_node);
        moveAhead(token, token_lookahead,dyn_string);
    }
    if (strcmp(token->attribute, ")"))
    {
        error(2, token->line);
    }

    if (!strcmp(token_lookahead->attribute, ":"))
    {
        // jestli je v lookaheadu dvojtecka, tak uzivatel specifikuje return values -> musim zkontrolovat type:list
        moveAhead(token, token_lookahead,dyn_string);
        moveAhead(token, token_lookahead,dyn_string);
        // PC
        result = result && type_list(token, token_lookahead, dyn_string, parent_node);
    }
    return result;

}

int func_call(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // the current TOKEN is the ID of function
    AST_add_child(parent_node, func_call_id, string_a(token->attribute));
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, "("))
    {
        error(2, token->line);
    }
    //if (token_lookahead->type == TYPE_STRING) // THIS IS GONNA NEED TO CHECK FOR ALL THE CONSTANTS, now only accepts strings
    // PC
    return constant_list(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);

}

int type_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    // PC
    return type(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && types(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
}

int types(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }

    //if (token_lookahead->type == TYPE_IDENTIFIER || !strcmp(token_lookahead->attribute, "global") || !strcmp(token_lookahead->attribute, "function") || !strcmp(token_lookahead->attribute, "end"))
    if (token_lookahead->type == TYPE_IDENTIFIER || token_lookahead->type == TYPE_KEYWORD || token_lookahead->type == TYPE_EOF)
    {
        // epsilon pravidlo pro konec vyctu typu mimo zavorky
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ",")){
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    // PC
    return type(token, token_lookahead, dyn_string, parent_node) && types(token, token_lookahead, dyn_string, parent_node);

}

int type(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    // if ")" here should deal with the epsilon rule = zero types given
    if (token->type == TYPE_DATATYPE){
        return SUCCESS;
    }
    error(2, token->line);
}

int constant_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    //AST_add_child(parent_node, params_id, nil_a());
    if (!strcmp(token_lookahead->attribute, ")"))
    {
        // epsilon pravidlo pro nula argumentu
        moveAhead(token, token_lookahead,dyn_string);
        return SUCCESS;
    }

    moveAhead(token, token_lookahead,dyn_string);
    // the current token is a constant - will need to work with that
    if (token->type == TYPE_STRING){
        AST_add_child(parent_node, constant_id, string_a(token->attribute));
    }
    else if (token->type == TYPE_INTEGER){
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, integer_a(strtol(token->attribute, endptr, 10)));
    }
    else if (token->type == TYPE_DECIMAL){
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, number_a(strtod(token->attribute, endptr)));
    }
    // PC
    return constants(token, token_lookahead, dyn_string, parent_node);
}

int constants(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    if (!strcmp(token_lookahead->attribute, ")")){
        moveAhead(token, token_lookahead,dyn_string);
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ",")){
        report_error("expected comma", token->line);
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    // now I am working with the CONSTANT
    if (token->type == TYPE_STRING){
        AST_add_child(parent_node, constant_id, string_a(token->attribute));
    }
    else if (token->type == TYPE_INTEGER){
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, integer_a(strtol(token->attribute, endptr, 10)));
    }
    else if (token->type == TYPE_DECIMAL){
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, number_a(strtod(token->attribute, endptr)));
    }
    // PC
    return constants(token, token_lookahead, dyn_string, parent_node);

}

int func_def(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node) {
    int result = 0;
    // token_lookahead == FUNC_ID -> zpracovat se symboltable
    AST_add_child(parent_node, func_def_id, string_a(token_lookahead->attribute));
    //ast_node_t* new_parent = &parent_node->child_arr[parent_node->no_children - 1];
    //AST_add_child(new_parent, variable_id, string_a(token->attribute));

    moveAhead(token, token_lookahead,dyn_string);
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, "(")) {
        error(2, token->line);
    }
    // PC
    result = param_list(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    moveAhead(token, token_lookahead,dyn_string);
    if (!strcmp(token_lookahead->attribute, ":"))
    {
        // jestli je v lookaheadu dvojtecka, tak uzivatel specifikuje return values -> musim zkontrolovat type:list
        moveAhead(token, token_lookahead,dyn_string);
        moveAhead(token, token_lookahead,dyn_string);
        // PC
        result = result && type_list(token, token_lookahead, dyn_string, parent_node);
    }
    if (!strcmp(token_lookahead->attribute, "end"))
    {
            // epsilon rule for function body
            moveAhead(token, token_lookahead,dyn_string);
            return result;
    }
    from_func_def = true;
    result = result && func_body(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    if (strcmp(token->attribute, "end")){
        error(2, token->line);
    }
    return result;
}

int param_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    //AST_add_child(parent_node, params_id, nil_a());
    if (!strcmp(token_lookahead->attribute, ")"))
    {
        // epsilon pravidlo pro nula parametru
        return SUCCESS;
    }
    int result = 0;
    moveAhead(token, token_lookahead,dyn_string);
    // the current token is an ID
    /// add child
    AST_add_child(parent_node, variable_id, string_a(token->attribute));
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ":"))
    {
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    // PC
    result = type(token, token_lookahead, dyn_string, parent_node);
    // PC
    return  result && params(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);

}
int params(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ",")){
        report_error("expected comma", token->line);
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    // now I am working with the var_ID
    AST_add_child(parent_node, variable_id, string_a(token->attribute));
    moveAhead(token, token_lookahead,dyn_string);
    moveAhead(token, token_lookahead,dyn_string);
    // PC
    return type(token, token_lookahead, dyn_string, parent_node) && params(token, token_lookahead, dyn_string, parent_node);
}

int func_body(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    if (from_func_def){
        AST_add_child(parent_node, body_id, nil_a());
        from_func_def = false;
    }
    if (token_lookahead->type == TYPE_EOF){
        error(2, token->line);
    }
    if (!strcmp(token_lookahead->attribute, "end")){
        moveAhead(token, token_lookahead,dyn_string);
        return SUCCESS;
    }
    if (token_lookahead->spec == SPEC_ELSE || token_lookahead->spec == SPEC_END){
        // epsilon rule for ending the function's body
        return SUCCESS;
    }
    // probably gonna have to add more keywords(?) checks to end while statements and others
    // todo not sure about this strat yet, might work
    if (podezrely_token){
        podezrely_token = false;
    }
    else{
        moveAhead(token, token_lookahead,dyn_string);
    }
    if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "if"))){
        return if_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "while"))){
        return while_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if ((token->type == TYPE_KEYWORD) && (!strcmp(token->attribute, "return"))){
        return return_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if (token->type == TYPE_IDENTIFIER && token_lookahead->spec == SPEC_OPEN){
        return func_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if (token->spec == SPEC_LOCAL){
        return decl_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    else if (!strcmp(token_lookahead->attribute, ",") || token_lookahead->type == TYPE_ASSIGNMENT)
    {
        return assignment(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && func_body(token, token_lookahead, dyn_string, parent_node);
    }
    error(2, token->line);
}

int if_element(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // the current token is "if"
    AST_add_child(parent_node, branch_id, nil_a());
    int result = 1;
    DLList* list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(list);
    DLList* AST_list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(AST_list);
    DLL_parse(list, token_lookahead, dyn_string, AST_list);
    DLLElement* vraceny_token = list->first;
    if (vraceny_token == NULL)
    {
        error(2, token->line);
    }
    AST_connect_DLL(&parent_node->child_arr[parent_node->no_children - 1], AST_list);
    //while (vraceny_token != NULL){
    //    // v liste se nachazi omylem ukradnuty token
    //    vraceny_token = vraceny_token->next;
    //}
    *token = *vraceny_token->token;
    DLL_Dispose(list);
    free(list);
    get_token(token_lookahead,dyn_string);
    if (token->spec != SPEC_THEN)
    {
        error(2, token->line);
    }
    if (token_lookahead->spec == SPEC_ELSE)
    {
        AST_add_child(&parent_node->child_arr[parent_node->no_children - 1], body_id, nil_a());
        // epsilon rule
    }
    else
    {
        from_func_def = true;
        result = func_body(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (token->spec != SPEC_ELSE)
    {
        error(2, token->line);
    }
    // now at token else
    if (token_lookahead->spec == SPEC_END){
        moveAhead(token, token_lookahead,dyn_string);
        // epsilon rule
        AST_add_child(&parent_node->child_arr[parent_node->no_children - 1], body_id, nil_a());
    }
    else
    {
        from_func_def = true;
        result = func_body(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && result;
    }
    //moveAhead(token, token_lookahead,dyn_string); // move_ahead musi byt tady nebo v epsilon vetvi predchoziho ifu
    if (token->spec != SPEC_END)
    {
        error(2, token->line);
    }
    //moveAhead(token, token_lookahead,dyn_string);
    return result;
}

int while_element(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // the current token is "while"
    AST_add_child(parent_node, while_id, nil_a());
    DLList* list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(list);
    DLList* AST_list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(AST_list);
    DLL_parse(list, token_lookahead, dyn_string, AST_list);
    DLLElement* vraceny_token = list->first;
    if (vraceny_token == NULL)
    {
        error(2, token->line);
    }
    AST_connect_DLL(&parent_node->child_arr[parent_node->no_children - 1], AST_list);
    printf("== vrácený token: %s ==\n", vraceny_token->token->attribute);
    *token = *vraceny_token->token;
    DLL_Dispose(list);
    free(list);
    printf("PROBEHLA USPESNA DEALOKACE\n");
    get_token(token_lookahead,dyn_string);
    if (token->spec != SPEC_DO)
    {
        error(2, token->line);
    }
    //moveAhead(token, token_lookahead,dyn_string);
    from_func_def = true;
    int result = func_body(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    //moveAhead(token, token_lookahead,dyn_string);
    if (token->spec != SPEC_END)
    {
        error(2, token->line);
    }
    return result;
}

int func_element(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    AST_add_child(parent_node, func_call_id, string_a(token->attribute));
    // the current token is FUNC_ID
    int result = 1;
    moveAhead(token, token_lookahead,dyn_string);
    // already checked opening bracket in lookahead, so I can skip that one
    if (token_lookahead->spec == SPEC_CLOS)
    {
        // epsilon rule for no given arguments
        AST_add_child(&parent_node->child_arr[parent_node->no_children - 1], body_id, nil_a());
    }
    else
    {
        // PC == possible change
        result = arg_list(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    }
    moveAhead(token, token_lookahead,dyn_string);
    return  result;
    //moveAhead(token, token_lookahead,dyn_string);
    // todo check for right closing bracket?
}

int arg_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // PC
    //AST_add_child(parent_node, params_id, nil_a());
    //return arg(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && args(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    return arg(token, token_lookahead, dyn_string, parent_node) && args(token, token_lookahead, dyn_string, parent_node);
}

int arg(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    moveAhead(token, token_lookahead,dyn_string);

    if (token->type == TYPE_STRING)
    {
        AST_add_child(parent_node, constant_id, string_a(token->attribute));
        return SUCCESS;
    }
    if (token->type == TYPE_INTEGER)
    {
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, integer_a(strtol(token->attribute, endptr, 10)));
        return SUCCESS;
    }
    if (token->type == TYPE_DECIMAL)
    {
        char** endptr = NULL;
        AST_add_child(parent_node, constant_id, number_a(strtod(token->attribute, endptr)));
        return SUCCESS;
    }
    if (token->type == TYPE_IDENTIFIER)
    {
        AST_add_child(parent_node, variable_id, string_a(token->attribute));
        return SUCCESS;
    }
    error(2, token->line);
}

int args(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    int result;
    if (!strcmp(token_lookahead->attribute, ")")){
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ",") != 0){
        report_error("expected comma", token->line);
        error(2, token->line);
    }
    // now I am working with the CONSTANT or ID
    // PC
    result = arg(token, token_lookahead, dyn_string, parent_node);
    return args(token, token_lookahead, dyn_string, parent_node) && result;

}

int return_element(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    AST_add_child(parent_node, return_id, nil_a());
    if (token_lookahead->type == TYPE_KEYWORD)
    {
        // epsilon rule for just purely return with no return data
        return SUCCESS;
    }
    if (token_lookahead->type == TYPE_IDENTIFIER || token_lookahead->type == TYPE_STRING || token_lookahead->type == TYPE_INTEGER || token_lookahead->type == TYPE_DECIMAL)
    {
        // PC
        return return_list(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    }
    //if (token_lookahead->type == TYPE_KEYWORD)
    //{
    //  // pokud je return nekde v tele funkce, tak je potreba skipnout vsechny tokeny az na konec deklarace????
    //    while (strcmp(token_lookahead->attribute, "end"))
    //    {
    //        moveAhead(token, token_lookahead,dyn_string);
    //    }
    //    return SUCCESS;
    //}
    error(2, token->line);
}

int return_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // PC
    return item(token, token_lookahead, dyn_string, parent_node) && items(token, token_lookahead, dyn_string, parent_node);

}

int item(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    DLList* list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(list);
    DLList* AST_list = (DLList *) malloc(sizeof(DLList));
    DLL_Init(AST_list);
    DLL_parse(list, token_lookahead, dyn_string, AST_list);
    DLLElement* vraceny_token = list->first;
    if (vraceny_token == NULL)
    {
        error(2, token->line);
    }
    //while (vraceny_token != NULL){
    //    // v liste se nachazi omylem ukradnuty token
    //    vraceny_token = vraceny_token->next;
    //}
    AST_connect_DLL(parent_node, AST_list);
    printf("== vrácený token: %s ==\n", vraceny_token->token->attribute);
    *token = *vraceny_token->token;
    DLL_Dispose(list);
    free(list);
    if (token->spec == SPEC_END){
        *token_lookahead = *token;
    }
    else{
        get_token(token_lookahead,dyn_string);
    }
    if (token->type == TYPE_IDENTIFIER && token_lookahead->spec == SPEC_OPEN){
        podezrely_token = true;
    }
    else if (token->spec == SPEC_WHILE || token->spec == SPEC_RETURN || token->spec == SPEC_IF || token->type == TYPE_IDENTIFIER || token->spec == SPEC_LOCAL){
        podezrely_token = true;
    }


    //moveAhead(token, token_lookahead,dyn_string);
    return SUCCESS;
    if (token->type == TYPE_STRING || token->type == TYPE_INTEGER || token->type == TYPE_DECIMAL)
    {
        return SUCCESS;
    }
    if (token->type == TYPE_IDENTIFIER)
    {
        // check if it is a variable or function call
        // jestli lookahead je leva zavorka, tak zavolej func_element
        if (token_lookahead->spec == SPEC_OPEN)
        {
            return  func_element(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
        }
        // currently working with ID
        return SUCCESS;
    }
    error(2, token->line);

}

int items(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    if (token->type == TYPE_KEYWORD || token->spec == SPEC_END || token_lookahead->spec == SPEC_OPEN) // todo changed from lookahead to token
    {
        //epsilon rule
        return SUCCESS;
    }
    //moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ","))
    {
        error(2, token->line);
    }
    // PC
    return item(token, token_lookahead, dyn_string, parent_node) && items(token, token_lookahead, dyn_string, parent_node);
}

int decl_element(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    moveAhead(token, token_lookahead,dyn_string);
    if (token->type != TYPE_IDENTIFIER)
    {
        error(2, token->line);
    }
    // now I am working with the ID of the variable
    AST_add_child(parent_node, var_declare_id, string_a(token->attribute));
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ":"))
    {
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (token_lookahead->type == TYPE_ASSIGNMENT)
    {
        //PC
        return type(token, token_lookahead, dyn_string, parent_node) && decl_assign(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    }
    else
    {
        // PC
        return type(token, token_lookahead, dyn_string, parent_node);
    }
}

int decl_assign(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    //AST_add_child(parent_node, assign_id, nil_a());
    moveAhead(token, token_lookahead,dyn_string);
    if (token->type != TYPE_ASSIGNMENT)
    {
        error(2, token->line);
    }
    // PC
    //return item(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    return item(token, token_lookahead, dyn_string, parent_node);
}

int R_assignment(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // PC
    return item_list(token, token_lookahead, dyn_string, parent_node);
}

int item_list(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // PC
    return item(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]) && items(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
}

int assignment(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    AST_add_child(parent_node, assign_id, nil_a());
    // PC
    int result = L_assignment(token, token_lookahead, dyn_string, &parent_node->child_arr[parent_node->no_children - 1]);
    moveAhead(token, token_lookahead,dyn_string);
    if (token->type != TYPE_ASSIGNMENT){
        error(2, token->line);
    }
    // PC
    return result && R_assignment(token, token_lookahead, dyn_string, parent_node);
}

int L_assignment(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    // now I am working with the ID
    if (token->type == TYPE_IDENTIFIER){
        AST_add_child(parent_node, variable_id, string_a(token->attribute));
    }
    else{
        error(2, token->line);
    }
    if (!strcmp(token_lookahead->attribute, ","))
    {
        // PC
        return ids(token, token_lookahead, dyn_string, parent_node);
    }
    return SUCCESS;
}

int ids(token_t* token, token_t* token_lookahead, dynamic_string* dyn_string, ast_node_t* parent_node){
    if (strcmp(token_lookahead->attribute, ","))
    {
        return SUCCESS;
    }
    moveAhead(token, token_lookahead,dyn_string);
    if (strcmp(token->attribute, ",")){
        error(2, token->line);
    }
    moveAhead(token, token_lookahead,dyn_string);
    // now I am working with the ID
    if (token->type == TYPE_IDENTIFIER){
        AST_add_child(parent_node, variable_id, string_a(token->attribute));
    }
    else{
        error(2, token->line);
    }
    // PC
    return ids(token, token_lookahead, dyn_string, parent_node);
}