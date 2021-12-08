/* ******************* ast.c ************************************************ */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Soubor s implementaci abstraktniho syntaktickeho stromu, ktery      */
/*        slouzi jako rozhrani mezi parserem a generatorem                    */
/*  Vytvoril: Tym 102 - David Novak, prosinec 2021                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "our_error.h"
#include "ast.h"
#define INIT_ARR_SIZE 4 //pocatecni velikost pole deti

/**
 * @brief Pomocna funkce k AST_add_child, je to vlastne kostruktor nafukovaciho pole
 * @param node Noda, ve ktere bude pole vytvoreno
 */
void child_arr_create(ast_node_t *node){
    node->child_arr = calloc(sizeof(ast_node_t), INIT_ARR_SIZE);
    if(!node->child_arr) {
        error(99, 0);
    }
    node->child_arr_size = INIT_ARR_SIZE;
}

/**
 * @brief Pomocna funkce k AST, zvetsi velikost nafukovaciho pole
 * @param node Noda, ve ktere bude pole zvetseno
 */
void child_arr_expand(ast_node_t *node){
    if(!node->child_arr_size){
        fprintf(stderr, "Muzes expandovat jen inicializovane child_arr\n");
        return;
    }
    node->child_arr = realloc(node->child_arr, sizeof(ast_node_t)* INIT_ARR_SIZE* node->no_children);
    if(!node->child_arr) {
        error(99, 0);
    }
    node->child_arr_size = node->child_arr_size*INIT_ARR_SIZE;
}

/**
 * @brief destruktor nafukovaciho pole
 * @param node Noda, ve ktere bude pole zruseno
 */
int child_arr_destroy(ast_node_t *node){
    free(node->child_arr);
    node->child_arr_size = 0;
    return 1;
}

/**
 * @brief Inicializuje AST a vlozi do neho nodu program_id (root)
 * @param ast adresa ukazatele na koren (ast_node_t)
 * @return 
 */
void AST_insert_root(ast_node_t **ast){
    (*ast) = malloc(sizeof(struct ASTNode));
    if(!(*ast)) {
        error(99, 0);
    }
    (*ast)->id = program_id;
    child_arr_create((*ast));
    (*ast)->no_children = 0;
}

/**
 * @brief destruktor AST
 */
void AST_destroy(){
    //todo
}

/**
 * @brief vytvoří atribut se stringem.. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param stringg string na konverzi do atribudutu
 * @return atribut se stringem
 */
attribute_t string_a(char* stringg){
    attribute_t atrib;
    atrib.saved_type = string;
    atrib.name = (char*)malloc(sizeof(char)*(strlen(stringg)+1));
    strcpy(atrib.name, stringg);
    return atrib;
}

/**
 * @brief vytvoří atribut s nilem. Pro použití do parametru funkcí, co vyžadují attribute_t, jako například
 * AST_add_child()
 * @return atribut s nilem
 */
attribute_t nil_a(){
    attribute_t atrib;
    atrib.saved_type = nil;
    atrib.nil = true;
    return atrib;
}

/**
 * @brief vytvoří atribut s number. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param numberr number na konverzi do atribudutu
 * @return atribut s number
 */
attribute_t number_a(double numberr){
    attribute_t atrib;
    atrib.saved_type = number;
    atrib.number = numberr;
    return atrib;
}

/**
 * @brief vytvoří atribut s integer. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param integerr integer na konverzi do atribudutu
 * @return atribut s integer
 */
attribute_t integer_a(int integerr){
    attribute_t atrib;
    atrib.saved_type = integer;
    atrib.integer = integerr;
    return atrib;
}

/**
 *
 * @param parent ukazatel na rodice, kteremu mame vytvorit dite
 * @param id id ditete, ktere vkladame
 * @param attribute atribut nody, vutvoreny funkci integer_a, number_a, string_a nebo nil_a
 */
void AST_add_child(ast_node_t *parent, node_id_t id, attribute_t attribute){
    if(parent->no_children == parent->child_arr_size) {
        child_arr_expand(parent);
    }
    child_arr_create(&parent->child_arr[parent->no_children]);
    parent->child_arr[parent->no_children].no_children = 0;
    parent->child_arr[parent->no_children].id = id;
    parent->child_arr[parent->no_children].attribute = attribute;

    parent->no_children++;
}

node_id_t get_id(token_t* token){
    switch (token->type) {
        case TYPE_IDENTIFIER:
            return variable_id;
        case TYPE_OPERATOR:
            return operation_id;
        case TYPE_INTEGER:
        case TYPE_STRING:
        case TYPE_DECIMAL:
        case TYPE_NIL:
            return constant_id;
        default:
            fprintf(stderr, "konvertor nerozeznal id\n");
            return operation_id; //random
    }
}

attribute_t get_attribut(token_t* token){
    char *endptr;
    switch (token->type) {
        case TYPE_IDENTIFIER:
        case TYPE_OPERATOR:
        case TYPE_STRING:
            return string_a(token->attribute);
        case TYPE_INTEGER:
            return integer_a(strtol(token->attribute, &endptr, 10));
        case TYPE_DECIMAL:
            return number_a(strtod(token->attribute, &endptr));
        case TYPE_NIL:
            return nil_a();
        default:
            fprintf(stderr, "konvertor nerozeznal atribut\n");
            return string_a(token->attribute); //random
    }
}

void AST_add_child_rec_expr(DLList * dll, ast_node_t *parent){
    //add child - prida dite podle nasledujiciho tokenu
    DLL_DeleteLast(dll); //smaze posledni, tim padem posune posledni prvek o 1
    //////////////////////////// GET_LAST
    token_t *token = malloc(sizeof(token_t));
    if(!token) {
        error(99, 69);
    }
    DLL_GetLast(dll, token);
    char *new_attrib = malloc(sizeof(char)*(strlen(token->attribute)+1));
    if(!new_attrib) {
        error(99, 69);
    }
    strcpy(new_attrib, token->attribute);
    token->attribute = new_attrib;
    //////////////////////////////////
    AST_add_child(parent, get_id(token), get_attribut(token));
    //podiva se co je dite zac
    if (!(parent->child_arr[0].id == constant_id || parent->child_arr[0].id == variable_id)) { //pokud je operator
        //prida prvni dite - pravy
        AST_add_child_rec_expr(dll, &parent->child_arr[0]);
        if(!strcmp(parent->child_arr[0].attribute.name, "#")){ //pokud je to unarni operator (neprida uz dalsi dite)
            return;
        }

    }
    DLL_DeleteLast(dll);
    //////////////////////////// GET_LAST
    token = malloc(sizeof(token_t));
    if(!token) {
        error(99, 69);
    }
    DLL_GetLast(dll, token);
    char *new_attrib2 = malloc(sizeof(char)*(strlen(token->attribute)+1));
    if(!new_attrib2) {
        error(99, 69);
    }
    strcpy(new_attrib2, token->attribute);
    token->attribute = new_attrib2;
    //////////////////////////////////
    AST_add_child(parent, get_id(token), get_attribut(token));

    if (!(parent->child_arr[1].id == constant_id || parent->child_arr[1].id == variable_id)) {
        //prida prvni dite - pravy
        AST_add_child_rec_expr(dll, &parent->child_arr[1]);
    }
}


ast_node_t *AST_dll_to_tree(DLList *dll){
    ast_node_t *ast;
    ast = malloc(sizeof(struct ASTNode));
    if(!ast) {
        error(99, 0);
    }
    //////////////////////////// GET_LAST
    token_t *token = malloc(sizeof(token_t));
    if(!token) {
        error(99, 69);
    }
    DLL_GetLast(dll, token);
    char *new_attrib = malloc(sizeof(char)*(strlen(token->attribute)+1));
    if(!new_attrib) {
        error(99, 69);
    }
    strcpy(new_attrib, token->attribute);
    token->attribute = new_attrib;
    //////////////////////////////////
    ast->id = get_id(token);
    ast->attribute = get_attribut(token);
    child_arr_create(ast);
    ast->no_children = 0;

    if(ast->id == constant_id || ast->id == variable_id) {
        return ast;
    } else {
        AST_add_child_rec_expr(dll, ast);
    }
    return ast;
}

void AST_connect_DLL(ast_node_t *parent, DLList *dll) {
    if (parent->no_children == parent->child_arr_size) {
        child_arr_expand(parent);
    }
    child_arr_create(&parent->child_arr[parent->no_children]);
    parent->child_arr[parent->no_children] = *(AST_dll_to_tree(dll));

    parent->no_children++;
}

/*
//usage example

int main() {
    ast_node_t *ast;
    AST_insert_root(&ast);
    ast_node_t *stashed_node = ast;

    AST_add_child(stashed_node, func_def_id, string_a("foo"));

    return 1;
}
 */

