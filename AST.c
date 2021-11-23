#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


typedef enum {
    while_id,
    assign_id,
    if_id,
    variable_id,
    function_id
    // ...possibly other types
} type_id;

typedef struct ASTNode {
    struct ASTNode* children[1000];
    int children_count;
    type_id type;
    char* name; // name of var or func / const_string_value
    double number; // number of vars to assign to / value of constant

} ast_node;
void init_tree(ast_node **tree){
    ast_node* new_node = malloc(sizeof(struct ASTNode));
    if (!new_node)
    {
        // malloc has failed
        printf("malloc failed when initialising the AST\n");
        return;
    }
    new_node->children_count = 0;
    *tree = new_node;
}

void insert_child(ast_node *parent, type_id type, char *name, double number){
    ast_node* new_node = malloc(sizeof(struct ASTNode));
    if (!new_node)
    {
        // malloc has failed
        return;
    }
    new_node->children_count = 0;
    new_node->type = type;
    new_node->name = name;
    new_node->number = number;

    parent->children[(parent->children_count)++] = new_node;
}
void TEST_AST(ast_node *AST){
    insert_child(AST, if_id, "", 0);
    printf("%s %lf %d\n", AST->children[0]->name, AST->children[0]->number, AST->children[0]->type);
}
void main() {
    ast_node* AST;
    init_tree(&AST);
    TEST_AST(AST);
    return;
}