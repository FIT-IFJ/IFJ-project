#include <stdbool.h>

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
void child_arr_create(ast_node_t *node);

/**
 * @brief Pomocna funkce k AST, zvetsi velikost nafukovaciho pole
 * @param node Noda, ve ktere bude pole zvetseno
 */
void child_arr_expand(ast_node_t *node);

/**
 * @brief destruktor nafukovaciho pole
 * @param node Noda, ve ktere bude pole zruseno
 */
int child_arr_destroy(ast_node_t *node);

/**
 * @brief Inicializuje AST a vlozi do neho nodu program_id (root)
 * @param ast adresa ukazatele na koren (ast_node_t)
 * @return
 */
void AST_insert_root(ast_node_t **ast);

/**
 * @brief destruktor AST
 */
void AST_destroy(); //todo

/**
 * @brief vytvoří atribut se stringem.. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param stringg string na konverzi do atribudutu
 * @return atribut se stringem
 */
attribute_t string_a(char* stringg);

/**
 * @brief vytvoří atribut s nilem. Pro použití do parametru funkcí, co vyžadují attribute_t, jako například
 * AST_add_child()
 * @return atribut s nilem
 */
attribute_t nil_a();

/**
 * @brief vytvoří atribut s number. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param numberr number na konverzi do atribudutu
 * @return atribut s number
 */
attribute_t number_a(double numberr);

/**
 * @brief vytvoří atribut s integer. Pro použití do
 * parametru funkcí, co vyžadují attribute_t, jako například AST_add_child()
 * @param integerr integer na konverzi do atribudutu
 * @return atribut s integer
 */
attribute_t integer_a(int integerr);

/**
 *
 * @param parent ukazatel na rodice, kteremu mame vytvorit dite
 * @param id id ditete, ktere vkladame
 * @param attribute atribut nody, vutvoreny funkci integer_a, number_a, string_a nebo nil_a
 */
void AST_add_child(ast_node_t *parent, node_id_t id, attribute_t attribute);
/*

