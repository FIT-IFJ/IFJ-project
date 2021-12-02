#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "our_error.h"
#include "ast.h"
#define INIT_ARR_SIZE 4 //pocatecni velikost pole deti

/**
 * Id/typ nody ast. Podle toho generato pozna, kterou konstrukci dany node reprezentuje, jaky ma ocekavat atribut a
 * jake ma ocekavat deti/kde
 */
typedef enum id {   //attribute //children
    program_id,     //          //0...n func_def; 0...n func_call
    func_def_id,    //func_name //1st returns; 2nd params, 3rd (func)body
    while_id,       //          //1st condition; 2nd (while)body
    branch_id,      //          //1st condition; 2nd (if)body, 3rd (else)body
    condition_id,   //operator  //1st (left)expr; 2nd (right)expr
    operation_id,   //operator  //1st (left)expr; 2nd (right)expr
    var_declare_id, //var_name  //
    assign_id,      //count     //count var_name; count expr or func_call
    func_call_id,   //func_name //n params
    params_id,      //          //n var
    returns_id,     //count     //
    return_id,      //          //n expr
    variable_id,    //var_name  //
    constant_id,    //value     //
    body_id         //          //n prikazu
} node_id_t;


typedef enum {nil, integer, number, string} datatype_t; // typ typu promnenne

/**
 * Typ atributu nody, je v nem ulozeny atribut na urcitem miste podle typu
 */
typedef struct attribute{
    datatype_t saved_type;
    char* name;
    double number;
    int integer;
    bool nil;
    datatype_t type;
} attribute_t;

/**
 * Typ nody, obsahuje id, atribut(data) a pole deti
 */
typedef struct ASTNode {
    node_id_t id;               //id
    attribute_t attribute;      //atribut
    int child_arr_size;         //pocet deti
    int no_children;            //pocet deti
    struct ASTNode* child_arr;  //děti, dynamické pole
} ast_node_t;

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
};

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
    child_arr_create((*ast)->child_arr);
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
    parent->no_children++;
    child_arr_create(parent->child_arr[parent->no_children].child_arr);
    parent->child_arr[parent->no_children].no_children = 0;
    parent->child_arr[parent->no_children].id = id;

    parent->child_arr[parent->no_children].attribute = attribute;
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

