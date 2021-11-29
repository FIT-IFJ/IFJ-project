
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INIT_ARR_SIZE 4 //pocatecni velikost pole deti

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

typedef union attribute{
    char* name;
    double number;
    int integer;
} attribute_t;

typedef struct ASTNode {
    node_id_t id;               //id
    attribute_t attribute;      //atribut
    int child_arr_size;         //pocet deti
    int no_children;            //pocet deti
    struct ASTNode* child_arr;  //děti, dynamické pole
} ast_node_t;

int child_arr_create(ast_node_t *node){
    node->child_arr = calloc(sizeof(ast_node_t), INIT_ARR_SIZE);
    if(!node->child_arr) {
        fprintf(stderr, "Chyba alokace paměti v child_arr_create\n");
        return 0;
    }
    node->child_arr_size = INIT_ARR_SIZE;
    return 1;
}

int child_arr_expand(ast_node_t *node){
    if(!node->child_arr_size){
        fprintf(stderr, "Muzes expandovat jen inicializovane child_arr\n");
        return -1;
    }
    node->child_arr = realloc(node->child_arr, sizeof(ast_node_t)* INIT_ARR_SIZE* node->no_children);
    if(!node->child_arr) {
        fprintf(stderr, "Chyba alokace paměti v child_arr_expand\n");
        return 0;
    }
    node->child_arr_size = node->child_arr_size*INIT_ARR_SIZE;
    return 1;
}

int child_arr_destroy(ast_node_t *node){
    free(node->child_arr);
    node->child_arr_size = 0;
    return 1;
};

void AST_init(ast_node_t **tree) {
    (*tree) = NULL;
}

int AST_insert_root(ast_node_t **ast){
    (*ast) = malloc(sizeof(struct ASTNode));
    if(!(*ast)) {
        fprintf(stderr, "Chyba alokace paměti v AST_create\n");
        return 0;
    }
    (*ast)->id = program_id;
    child_arr_create((*ast)->child_arr);
    (*ast)->no_children = 0;
    return 1;
}

void AST_destroy(){
    //todo
}

/**
 * @brief Makes String Attribute - vytvoří atribut se stringem, alokuje pro něho paměť. Pro použití přímo stringu do
 * atributu funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param string string na konverzi do atribudutu
 * @return atribut se stringem
 */
attribute_t msa(char* string){
    attribute_t atrib;
    atrib.name = (char*)malloc(sizeof(char)*(strlen(string)+1));
    strcpy(atrib.name, string);
    return atrib;
}


int AST_add_child(ast_node_t *parent, node_id_t id, attribute_t attribute){
    if(parent->no_children == parent->child_arr_size) {
        child_arr_expand(parent);
    }
    parent->no_children++;
    child_arr_create(parent->child_arr[parent->no_children].child_arr);
    parent->child_arr[parent->no_children].no_children = 0;
    parent->child_arr[parent->no_children].id = id;
    if(id == variable_id || id == func_def_id  || id == var_declare_id
    || id == func_call_id || id == condition_id || id == operation_id) {
        //retezcove attributy TODO
    } else { //double nebo int
        parent->child_arr[parent->no_children].attribute = attribute;
    }
    return 1;
}
///==================================================================
// GENERATOR
///==================================================================
int while_count = 0;

int digit_count(int number) {
    int counter = 0; // variable declaration
    while(number != 0) {
        number = number / 10;
        counter++;
    }
    return counter;
}

int g_while(ast_node_t *while_node);
int g_body(ast_node_t *body_node);
int g_branch(ast_node_t *body_node);
int g_condition(ast_node_t *body_node);
//int g_expression(ast_node_t *body_node);
int g_func_call(ast_node_t *body_node);
int g_func_def(ast_node_t *body_node);


int g_expression(ast_node_t *body_node, char* result_var){

    return 1;
}


int g_while_condition(ast_node_t *condition_node, int current_while_no){
    printf("# while - podminka\n");
    char* while_cond_left = calloc((strlen("$while-cond-left-%d")+ digit_count(current_while_no)+1)*sizeof(char));
    if (!while_cond_left) {
        fprintf(stderr, "Chyba alokace paměti v g_while_condition");
        return 0;
    }
    char* while_cond_right = calloc((strlen("$while-cond-right-%d")+ digit_count(current_while_no)+1)*sizeof(char));
    if (!while_cond_right) {
        fprintf(stderr, "Chyba alokace paměti v g_while_condition");
        free(while_cond_left);
        return 0;
    }
    char* while_cond_result = calloc((strlen("$while-cond-result-%d")+ digit_count(current_while_no)+1)*sizeof(char));
    if (!while_cond_result) {
        fprintf(stderr, "Chyba alokace paměti v g_while_condition");
        free(while_cond_left);
        free(while_cond_right);
        return 0;
    }
    sprintf(while_cond_result, "$while-cond-result-%d", current_while_no);
    sprintf(while_cond_left, "$while-cond-left-%d", current_while_no);
    sprintf(while_cond_right, "$while-cond-right-%d", current_while_no);

    g_expression(&condition_node->child_arr[0], while_cond_left);
    g_expression(&condition_node->child_arr[1], while_cond_right);

    if (!strcmp(condition_node->attribute.name, "<")){

    } else if (!strcmp(condition_node->attribute.name, "<=")) {

    } else if (!strcmp(condition_node->attribute.name, ">")) {

    } else if (!strcmp(condition_node->attribute.name, ">=")) {

    } else if (!strcmp(condition_node->attribute.name, "==")) {

    } else if (!strcmp(condition_node->attribute.name, "~=")) {

    }
        return 1;
}

int g_body(ast_node_t *body_node){
    return 1;
}

int g_while(ast_node_t *while_node){
    const int curr_while_no = while_count;
    printf("# while - zacatek\n");
    printf("LABEL $while-cond%d\n", while_count);
    g_while_condition(&while_node->child_arr[0], curr_while_no);
    printf("JUMP $while-end");
    g_body(&while_node->child_arr[1]);
    printf("JUMP $while-cond");
    printf("LABEL $while-end%d\n", while_count);


    return 1;
}





int main() {
    ast_node_t *ast;
    AST_insert_root(&ast);
    ast_node_t *stashed_node = ast;

    AST_add_child(stashed_node, func_def_id, msa("foo"));

    return 1;
}
