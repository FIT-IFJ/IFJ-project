/* ******************* generator.c ****************************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Soubor s implementaci generatoru, ktery z abstraktniho stromu       */
/*        vygeneruje cilovy kod                                               */
/*  Vytvoril: Tym 102 - David Novak, prosinec 2021                            */
/* ************************************************************************** */

#include "generator.h"

int while_count = 0;
int if_count = 0;
int expr_count = 0;
int integrated_count = 0;



void g_integrated_functions(){
    printf("JUMP $jump_over_integrated\n");
    g_reads();
    g_readi();
    g_readn();
//    g_write(); nebude, je zabudovano ve g_func_call()
    g_tointeger();
    g_substr();
    g_ord();
    g_chr();
    printf("LABEL $jump_over_integrated\n");
}

void g_reads(){
    int curr_count = integrated_count;
    integrated_count++;
    printf("LABEL $reads\n");

    printf("DEFVAR LF@$integrated-%d\n", curr_count);
    printf("READ LF@$integrated-%d string\n", curr_count);
    printf("PUSHS LF@$integrated-%d\n", curr_count);

    printf("RETURN\n");
}

void g_readi(){
    int curr_count = integrated_count;
    integrated_count++;
    printf("LABEL $readi\n");

    printf("DEFVAR LF@$integrated-%d\n", curr_count);
    printf("READ LF@$integrated-%d int\n", curr_count);
    printf("PUSHS LF@$integrated-%d\n", curr_count);

    printf("RETURN\n");
}

void g_readn(){
    int curr_count = integrated_count;
    integrated_count++;
    printf("LABEL $readn\n");

    printf("DEFVAR LF@$integrated-%d\n", curr_count);
    printf("READ LF@$integrated-%d float\n", curr_count);
    printf("PUSHS LF@$integrated-%d\n", curr_count);

    printf("RETURN\n");
}

void g_tointeger(){
    int curr_count = integrated_count;
    integrated_count++;

    printf("LABEL $tointeger\n");

    //nacte argument
    printf("DEFVAR LF@$integrated-0-%d\n", curr_count);
    printf("POPS LF@$integrated-0-%d\n", curr_count);

    //kontrola argument neni nil
    printf("DEFVAR LF@$toi-nil-check-%d\n", curr_count);
    printf("DEFVAR LF@$toi-nil-check-bool-%d\n", curr_count);
    //stringu
    printf("TYPE LF@$toi-nil-check-%d LF@$integrated-0-%d\n", curr_count, curr_count);
    printf("EQ LF@$toi-nil-check-bool-%d LF@$toi-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $tointeger-ret-nil-%d LF@$toi-nil-check-bool-%d bool@true\n", curr_count, curr_count);

    //pokud je nil, skoci se dovnitr tehoto bloku
    printf("JUMP $tointeger-ret-nil-end-%d\n", curr_count);
    printf("LABEL $tointeger-ret-nil-%d\n", curr_count);
    printf("PUSHS nil@nil\n");
    printf("JUMP $tointeger-end-%d\n", curr_count);
    printf("LABEL $tointeger-ret-nil-end-%d\n", curr_count);


    //prevede a pushne
    printf("DEFVAR LF@$integrated-1-%d\n", curr_count);
    printf("FLOAT2INT LF@$integrated-1-%d LF@$integrated-0-%d\n", curr_count, curr_count);
    printf("PUSHS LF@$integrated-1-%d\n", curr_count);

    printf("LABEL $tointeger-end-%d\n", curr_count);
    printf("RETURN\n");
}

void g_substr(){
    int curr_count = integrated_count;
    integrated_count++;

    printf("LABEL $substr\n");

    //nacte argumenty
    printf("DEFVAR LF@$s-%d\n", curr_count);
    printf("POPS LF@$s-%d\n", curr_count);
    printf("DEFVAR LF@$i-%d\n", curr_count);
    printf("POPS LF@$i-%d\n", curr_count);
    printf("DEFVAR LF@$j-%d\n", curr_count);
    printf("POPS LF@$j-%d\n", curr_count);

    //kontrola zda argumenty nejsou nil
    printf("DEFVAR LF@$write-nil-check-%d\n", curr_count);
    printf("DEFVAR LF@$write-nil-check-bool-%d\n", curr_count);
    //stringu
    printf("TYPE LF@$write-nil-check-%d LF@$s-%d\n", curr_count, curr_count);
    printf("EQ LF@$write-nil-check-bool-%d LF@$write-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $write-exit-8%d LF@$write-nil-check-bool-%d bool@true\n", curr_count, curr_count);
    //icka
    printf("TYPE LF@$write-nil-check-%d LF@$i-%d\n", curr_count, curr_count);
    printf("EQ LF@$write-nil-check-bool-%d LF@$write-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $write-exit-8%d LF@$write-nil-check-bool-%d bool@true\n", curr_count, curr_count);
    //jcka
    printf("TYPE LF@$write-nil-check-%d LF@$j-%d\n", curr_count, curr_count);
    printf("EQ LF@$write-nil-check-bool-%d LF@$write-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $write-exit-8%d LF@$write-nil-check-bool-%d bool@true\n", curr_count, curr_count);


    //kontrola vstupu zda i > 1                             1 < i < j < #s
    printf("DEFVAR LF@$low_check-res-%d\n", curr_count);
    printf("LT LF@$low_check-res-%d LF@$i-%d int@1\n", curr_count, curr_count);
    printf("JUMPIFEQ $return-empty-%d LF@$low_check-res-%d bool@true\n", curr_count, curr_count);

    //kontrola vstupu zda j < #s
    printf("DEFVAR LF@$high_check-strlen-%d\n", curr_count);
    printf("STRLEN LF@$high_check-strlen-%d LF@$s-%d \n", curr_count, curr_count);
    printf("DEFVAR LF@$high_check-res2-%d\n", curr_count);
    printf("LT LF@$high_check-res2-%d LF@$high_check-strlen-%d LF@$j-%d\n", curr_count, curr_count, curr_count);
    printf("JUMPIFEQ $return-empty-%d LF@$high_check-res2-%d bool@true\n", curr_count, curr_count);


    //tvoreni substringu
    printf("DEFVAR LF@$substring-res-%d\n", curr_count);
    printf("MOVE LF@$substring-res-%d string@\n", curr_count); //da do res prazdny string
    printf("DEFVAR LF@$substring-char-%d\n", curr_count);

    //odecte dva, aby se dostal na -1. index na zacatku cyklu se pak pricte na 0. index (vse v relaci s i)
    printf("SUB LF@$i-%d LF@$i-%d int@2\n", curr_count, curr_count);
    printf("SUB LF@$j-%d LF@$j-%d int@1\n", curr_count, curr_count);

    printf("DEFVAR LF@$both_check-res3-%d\n", curr_count);
    //CYKLUS
    printf("LABEL $while-substr-%d\n", curr_count);
    printf("ADD LF@$i-%d LF@$i-%d int@1\n", curr_count, curr_count); //posune index i
    //kontrola zda i !< j if(i !< j)
    printf("LT LF@$both_check-res3-%d LF@$j-%d LF@$i-%d\n", curr_count, curr_count, curr_count);
    printf("JUMPIFEQ $end-while-substr-%d LF@$both_check-res3-%d bool@true\n", curr_count, curr_count);
    //ulozi znak
    printf("GETCHAR LF@$substring-char-%d LF@$s-%d LF@$i-%d\n", curr_count, curr_count, curr_count);
    //vlozi znak do vysledku
    printf("CONCAT LF@$substring-res-%d LF@$substring-res-%d LF@$substring-char-%d\n", curr_count, curr_count, curr_count);
    //konec cyklu
    printf("JUMP $while-substr-%d\n", curr_count);
    printf("LABEL $end-while-substr-%d\n", curr_count);

    //pushne vysledek
    printf("PUSHS LF@$substring-res-%d\n", curr_count);

    //return-empty
    printf("JUMP $write-hop-%d\n", curr_count);
    printf("LABEL $return-empty-%d\n", curr_count);
    printf("PUSHS string@\n");
    printf("LABEL $write-hop-%d\n", curr_count);


    //return chyba-nil
    printf("JUMP $write-hop2-%d\n", curr_count);
    printf("LABEL $write-exit-8%d\n", curr_count);
    printf("EXIT int@8\n");
    printf("LABEL $write-hop2-%d\n", curr_count);

    printf("RETURN\n");
}

void g_ord(){
    int curr_count = integrated_count;
    integrated_count++;

    printf("LABEL $ord\n");

    //nacte argumenty
    printf("DEFVAR LF@$ord-s-%d\n", curr_count);
    printf("POPS LF@$ord-s-%d\n", curr_count);
    printf("DEFVAR LF@$ord-i-%d\n", curr_count);
    printf("POPS LF@$ord-i-%d\n", curr_count);

    //kontrola zda argumenty nejsou nil
    printf("DEFVAR LF@$ord-nil-check-%d\n", curr_count);
    printf("DEFVAR LF@$ord-nil-check-bool-%d\n", curr_count);
    //stringu
    printf("TYPE LF@$ord-nil-check-%d LF@$ord-s-%d\n", curr_count, curr_count);
    printf("EQ LF@$ord-nil-check-bool-%d LF@$ord-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $ord-exit-8%d LF@$ord-nil-check-bool-%d bool@true\n", curr_count, curr_count);
    //icka
    printf("TYPE LF@$ord-nil-check-%d LF@$ord-i-%d\n", curr_count, curr_count);
    printf("EQ LF@$ord-nil-check-bool-%d LF@$ord-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $ord-exit-8%d LF@$ord-nil-check-bool-%d bool@true\n", curr_count, curr_count);


    //kontrola jestli 1 <= i <= #s
    printf("DEFVAR LF@$ord-low_check-res-%d\n", curr_count); //vysledek mensiho
    printf("DEFVAR LF@$ord-high_check-strlen-%d\n", curr_count); //strlen
    printf("STRLEN LF@$ord-high_check-strlen-%d LF@$ord-s-%d \n", curr_count, curr_count);
    printf("DEFVAR LF@$ord-high_check-res2-%d\n", curr_count); //vysledek vetsiho
    //kontrola vstupu zda i <= #s => ptam se pokud i > #s
    //tady jeste indexuju od 1
    printf("GT LF@$ord-high_check-res2-%d LF@$ord-i-%d LF@$ord-high_check-strlen-%d \n", curr_count, curr_count, curr_count);
    printf("JUMPIFEQ $ord-retnil-%d LF@$ord-high_check-res2-%d bool@true\n", curr_count, curr_count);
    //kontrola 1 <= i => ptam se pokud i < 1
    //tady indexujem od 1
    printf("LT LF@$ord-low_check-res-%d LF@$ord-i-%d int@1\n", curr_count, curr_count);
    printf("JUMPIFEQ $ord-retnil-%d LF@$ord-low_check-res-%d bool@true\n", curr_count, curr_count);

    //HLAVNI TELO FUNKCE
    printf("DEFVAR LF@$ord-final-res-%d\n", curr_count); //vysledek funkce
    printf("SUB LF@$ord-i-%d LF@$ord-i-%d int@1\n", curr_count, curr_count);
    printf("STRI2INT LF@$ord-final-res-%d LF@$ord-s-%d LF@$ord-i-%d\n", curr_count, curr_count, curr_count); //vysledek mensiho
    printf("PUSHS LF@$ord-final-res-%d\n", curr_count); //pushnu vysledek funkce

    //vraceni nilu
    printf("JUMP $ord-hop-%d\n" ,curr_count);
    printf("LABEL $ord-retnil-%d\n" ,curr_count);
    printf("PUSHS nil@nil\n");
    printf("LABEL $ord-hop-%d\n" ,curr_count);

    //vraceni chyby 8
    printf("JUMP $ord-hop2-%d\n" ,curr_count);
    printf("LABEL $ord-exit-8%d\n" ,curr_count);
    printf("EXIT int@8\n");
    printf("LABEL $ord-hop2-%d\n" ,curr_count);

    printf("RETURN\n");

}

void g_chr(){
    int curr_count = integrated_count;
    integrated_count++;

    printf("LABEL $chr\n");

    //nacte argument
    printf("DEFVAR LF@$chr-i-%d\n", curr_count);
    printf("POPS LF@$chr-i-%d\n", curr_count);

    //kontrola zda argument neni nil
    printf("DEFVAR LF@$chr-nil-check-%d\n", curr_count);
    printf("DEFVAR LF@$chr-nil-check-bool-%d\n", curr_count);
    //icko
    printf("TYPE LF@$chr-nil-check-%d LF@$chr-i-%d\n", curr_count, curr_count);
    printf("EQ LF@$chr-nil-check-bool-%d LF@$chr-nil-check-%d string@nil\n", curr_count, curr_count);
    printf("JUMPIFEQ $chr-exit-8%d LF@$chr-nil-check-bool-%d bool@true\n", curr_count, curr_count);

    //return nil jestli i < 0 && i > 255
    printf("DEFVAR LF@$chr-low_check-res-%d\n", curr_count); //vysledek mensiho
    printf("DEFVAR LF@$chr-high_check-res2-%d\n", curr_count); //vysledek vetsiho
    // i > 255
    printf("GT LF@$chr-high_check-res2-%d LF@$chr-i-%d int@255\n", curr_count, curr_count);
    printf("JUMPIFEQ $chr-retnil-%d LF@$chr-high_check-res2-%d bool@true\n", curr_count, curr_count);
    // i < 0
    printf("LT LF@$chr-low_check-res-%d LF@$chr-i-%d int@0\n", curr_count, curr_count);
    printf("JUMPIFEQ $chr-retnil-%d LF@$chr-low_check-res-%d bool@true\n", curr_count, curr_count);

    //HLAVNI TELO FUNKCE
    printf("DEFVAR LF@$chr-final-res-%d\n", curr_count); //vysledek funkce
    printf("INT2CHAR LF@$chr-final-res-%d LF@$chr-i-%d\n", curr_count, curr_count); //prevedu int na znak
    printf("PUSHS LF@$chr-final-res-%d\n", curr_count); //pushnu vysledek funkce

    //vraceni nilu
    printf("JUMP $chr-hop-%d\n" ,curr_count);
    printf("LABEL $chr-retnil-%d\n" ,curr_count);
    printf("PUSHS nil@nil\n");
    printf("LABEL $chr-hop-%d\n" ,curr_count);

    //vraceni chyby 8
    printf("JUMP $chr-hop2-%d\n" ,curr_count);
    printf("LABEL $chr-exit-8%d\n" ,curr_count);
    printf("EXIT int@8\n");
    printf("LABEL $chr-hop2-%d\n" ,curr_count);

    printf("RETURN\n");
}

void g_error_labels(){
    printf("JUMP $$$hop\n" );

    // prace s neocekavanou hodnotou nil = 8
    printf("LABEL $$$exit8\n" );
    printf("EXIT int@8\n");

    // chyba typove kompatibility v aritm, retezc, a relacnich vyrazech = 6
    printf("LABEL $$$exit6\n" );
    printf("EXIT int@6\n");

    // chyba deleni nulou = 9
    printf("LABEL $$$exit9\n" );
    printf("EXIT int@9\n");

    printf("LABEL $$$hop\n" );
}

void g_program(ast_node_t *program_node){
    printf(".IFJcode21\n");
    g_error_labels();
    g_integrated_functions();
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    for (int i = 0; i < program_node->no_children; i++) {
        if (program_node->child_arr[i].id == func_def_id) {
            g_func_def(&program_node->child_arr[i]);
        } else if (program_node->child_arr[i].id == func_call_id) {
            g_func_call(&program_node->child_arr[i]);
        }
    }
}



void g_body(ast_node_t *body_node){
    for (int i = 0; i < body_node->no_children; i++) { //projde deti
        if (body_node->child_arr[i].id == while_id) { //pokud bude dite tento typ nody, vygeneruje ho
            g_while(&body_node->child_arr[i]);
        } else if (body_node->child_arr[i].id == branch_id) {
            g_branch(&body_node->child_arr[i]);
        } else if (body_node->child_arr[i].id == return_id) {
            g_return(&body_node->child_arr[i]);
        } else if (body_node->child_arr[i].id == var_declare_id) {
            g_decl_var(&body_node->child_arr[i]);
        } else if (body_node->child_arr[i].id == assign_id) {
            g_assign(&body_node->child_arr[i]);
        } else if (body_node->child_arr[i].id == func_call_id) {
            g_func_call(&body_node->child_arr[i]);
        }
    }
}



void g_return(ast_node_t *return_node){
    for (int i = 0; i < return_node->no_children; i++) {
        int expr_no = g_expression(&return_node->child_arr[i]);
        printf("PUSHS LF@$expr-res-%d \n", expr_no);
    }
    printf("RETURN\n");
}

void g_decl_var(ast_node_t *var_decl_node){
    printf("DEFVAR LF@%s\n", var_decl_node->attribute.name);
    if(var_decl_node->no_children > 0){
        if(var_decl_node->child_arr[0].id == func_call_id){
            g_func_call(&var_decl_node->child_arr[0]);
            printf("POPS LF@%s\n", var_decl_node->attribute.name);
        } else {
            int expr_no = g_expression(&var_decl_node->child_arr[0]);
            printf("MOVE LF@%s LF@$expr-res-%d\n", var_decl_node->attribute.name, expr_no);
        }
    }
}

int g_expression(ast_node_t *expr_node){ //todo typove kontroly
    int curr_expr_no = expr_count;
    expr_count++;
    //vytvorim promennou pro vysledek
    printf("DEFVAR LF@$expr-res-%d\n" , curr_expr_no);

    //vlozim vysledek
    if (expr_node->id == constant_id) {
        if (expr_node->attribute.saved_type == string) {
            printf("MOVE LF@$expr-res-%d string@%s\n", curr_expr_no, expr_node->attribute.name);
        } else if (expr_node->attribute.saved_type == integer) {
            printf("MOVE LF@$expr-res-%d int@%d\n", curr_expr_no, expr_node->attribute.integer);
        } else if (expr_node->attribute.saved_type == number) {
            printf("MOVE LF@$expr-res-%d float@%a\n", curr_expr_no, expr_node->attribute.number);
        } else if (expr_node->attribute.saved_type == nil) {
            printf("MOVE LF@$expr-res-%d nil@nil\n", curr_expr_no);
        }
    } else if (expr_node->id == variable_id) {
        printf("MOVE LF@$expr-res-%d LF@%s\n", curr_expr_no, expr_node->attribute.name);
    } else if (expr_node->id == operation_id){
        if(strcmp(expr_node->attribute.name,"#") == 0){ //jednooperandovy operator
            int expr1_no = g_expression(&expr_node->child_arr[0]);
            // todo kontrola ze nejsou nil

            //kontrola, zda je operand string
            printf("DEFVAR LF@$expr-str-check-%d\n", curr_expr_no);
            printf("DEFVAR LF@$expr-str-check-bool-%d\n", curr_expr_no);

            printf("TYPE LF@$expr-str-check-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
            printf("EQ LF@$expr-str-check-bool-%d LF@$expr-res-%d string@string\n", curr_expr_no, expr1_no);
            printf("JUMPIFNEQ $$$exit6 LF@$expr-str-check-bool-%d bool@true\n", curr_expr_no); //chyba pokud cokoliv jineho nez string

            printf("STRLEN LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);

        } else { //
            int expr1_no = g_expression(&expr_node->child_arr[1]);
            int expr2_no = g_expression(&expr_node->child_arr[0]);
            // todo kontrola ze nejsou nil
            if (strcmp(expr_node->attribute.name, "+") == 0) {
                //kontrola typu operandu, pokud je jeden z nich float, pretypuju oba na floaty, jinak necham tak
                printf("DEFVAR LF@$expr-type-check-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check2-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check-bool-%d\n", curr_expr_no);

                printf("TYPE LF@$expr-type-check-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("TYPE LF@$expr-type-check2-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);

                //pokud stejne, skoci na ok
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d LF@$expr-type-check2-%d\n", curr_expr_no, curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $ok-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);

                //pokud prvni int, prevedu ho na float, jinak prevedu druhy
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d string@int\n", curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $convert-first-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);
                //prevedu druhy
                printf("DEFVAR LF@$second-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);
                printf("ADD LF@$expr-res-%d LF@$expr-res-%d LF@$second-converted-%d\n", curr_expr_no, expr1_no, curr_expr_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //prevedu prvni
                printf("LABEL $convert-first-%d\n", curr_expr_no);
                printf("DEFVAR LF@$first-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$first-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("ADD LF@$expr-res-%d LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, curr_expr_no, expr1_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //vypocitam bez prevodu
                printf("LABEL $ok-%d\n", curr_expr_no);
                printf("ADD LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
                printf("LABEL $ookk-%d\n", curr_expr_no);
            } else if (strcmp(expr_node->attribute.name, "-") == 0) {
                //kontrola typu operandu, pokud je jeden z nich float, pretypuju oba na floaty, jinak necham tak
                printf("DEFVAR LF@$expr-type-check-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check2-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check-bool-%d\n", curr_expr_no);

                printf("TYPE LF@$expr-type-check-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("TYPE LF@$expr-type-check2-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);

                //pokud stejne, skoci na ok
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d LF@$expr-type-check2-%d\n", curr_expr_no, curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $ok-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);

                //pokud prvni int, prevedu ho na float, jinak prevedu druhy
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d string@int\n", curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $convert-first-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);
                //prevedu druhy
                printf("DEFVAR LF@$second-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);
                printf("SUB LF@$expr-res-%d LF@$expr-res-%d LF@$second-converted-%d\n", curr_expr_no, expr1_no, curr_expr_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //prevedu prvni
                printf("LABEL $convert-first-%d\n", curr_expr_no);
                printf("DEFVAR LF@$first-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$first-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("SUB LF@$expr-res-%d LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, curr_expr_no, expr1_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //vypocitam bez prevodu
                printf("LABEL $ok-%d\n", curr_expr_no);
                printf("SUB LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
                printf("LABEL $ookk-%d\n", curr_expr_no);
            } else if (strcmp(expr_node->attribute.name, "*") == 0) {
                //kontrola typu operandu, pokud je jeden z nich float, pretypuju oba na floaty, jinak necham tak
                printf("DEFVAR LF@$expr-type-check-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check2-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-type-check-bool-%d\n", curr_expr_no);

                printf("TYPE LF@$expr-type-check-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("TYPE LF@$expr-type-check2-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);

                //pokud stejne, skoci na ok
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d LF@$expr-type-check2-%d\n", curr_expr_no, curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $ok-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);

                //pokud prvni int, prevedu ho na float, jinak prevedu druhy
                printf("EQ LF@$expr-type-check-bool-%d LF@$expr-type-check-%d string@int\n", curr_expr_no, curr_expr_no);
                printf("JUMPIFEQ $convert-first-%d LF@$expr-type-check-bool-%d bool@true\n", curr_expr_no, curr_expr_no);
                //prevedu druhy
                printf("DEFVAR LF@$second-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);
                printf("MUL LF@$expr-res-%d LF@$expr-res-%d LF@$second-converted-%d\n", curr_expr_no, expr1_no, curr_expr_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //prevedu prvni
                printf("LABEL $convert-first-%d\n", curr_expr_no);
                printf("DEFVAR LF@$first-converted-%d\n", curr_expr_no);
                printf("INT2FLOAT LF@$first-converted-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("MUL LF@$expr-res-%d LF@$second-converted-%d LF@$expr-res-%d\n", curr_expr_no, curr_expr_no, expr1_no);
                printf("JUMP $ookk-%d\n", curr_expr_no);

                //vypocitam bez prevodu
                printf("LABEL $ok-%d\n", curr_expr_no);
                printf("MUL LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
                printf("LABEL $ookk-%d\n", curr_expr_no);
            } else if (strcmp(expr_node->attribute.name, "/") == 0) { //todo kontroly deleni + kontrola 0
                printf("DIV LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
            } else if (strcmp(expr_node->attribute.name, "//") == 0) {
                printf("IDIV LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
            } else if (strcmp(expr_node->attribute.name, "..") == 0) {
                //kontrola, zda jsou operandy stringy
                printf("DEFVAR LF@$expr-str-check-%d\n", curr_expr_no);
                printf("DEFVAR LF@$expr-str-check-bool-%d\n", curr_expr_no);
                //kontrola prvniho
                printf("TYPE LF@$expr-str-check-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no);
                printf("EQ LF@$expr-str-check-bool-%d LF@$expr-res-%d string@string\n", curr_expr_no, expr1_no);
                printf("JUMPIFNEQ $$$exit6 LF@$expr-str-check-bool-%d bool@true\n", curr_expr_no); //chyba pokud cokoliv jineho nez string
                //kontrola druheho
                printf("TYPE LF@$expr-str-check-%d LF@$expr-res-%d\n", curr_expr_no, expr2_no);
                printf("EQ LF@$expr-str-check-bool-%d LF@$expr-res-%d string@string\n", curr_expr_no, expr2_no);
                printf("JUMPIFNEQ $$$exit6 LF@$expr-str-check-bool-%d bool@true\n", curr_expr_no); //chyba pokud cokoliv jineho nez string

                printf("CONCAT LF@$expr-res-%d LF@$expr-res-%d LF@$expr-res-%d\n", curr_expr_no, expr1_no, expr2_no);
            } else {
                fprintf(stderr, "generator nerozpoznal operac v g_wxpression()\n");
            }
        }
    }
    return curr_expr_no;
}

void g_func_call(ast_node_t *call_node){
    //pokud je funkce write, musime to resit takto, protoze nemame jak zjistit pocet predanych argumentu
    if (!strcmp(call_node->attribute.name, "write")) { //pokud fce je write
        for (int i = 0; i < call_node->no_children; i++) {
            //pokud je dite variable
            if (call_node->child_arr[i].id == variable_id) {
                printf("WRITE LF@%s\n", call_node->child_arr[i].attribute.name);
            } else { //pokud je dite konstanta
                if (call_node->child_arr[i].attribute.saved_type == string) {
                    printf("WRITE string@%s\n", call_node->child_arr[i].attribute.name);
                } else if (call_node->child_arr[i].attribute.saved_type == integer) {
                    printf("WRITE int@%d\n", call_node->child_arr[i].attribute.integer);
                } else if (call_node->child_arr[i].attribute.saved_type == number) {
                    printf("WRITE float@%a\n", call_node->child_arr[i].attribute.number);
                } else if (call_node->child_arr[i].attribute.saved_type == nil) {
                    printf("WRITE nil@nil\n");
                }
            }
        }
    } else {
        //pushnu na datovy zasobnik argumenty
        for (int i = 0; i < call_node->no_children - 1; i++) { //-1 bo indexace je od nuly
            if (call_node->child_arr[i].id == variable_id) {
                printf("PUSHS LF@%s\n", call_node->attribute.name);
            } else { //konstantu
                if (call_node->attribute.saved_type == string) {
                    printf("PUSHS string@%s\n", call_node->attribute.name);
                } else if (call_node->attribute.saved_type == integer) {
                    printf("PUSHS int@%d\n", call_node->attribute.integer);
                } else if (call_node->attribute.saved_type == number) {
                    printf("PUSHS float@%a\n", call_node->attribute.number);
                } else if (call_node->attribute.saved_type == nil) {
                    printf("PUSHS nil@nil\n");
                }
            }
        }
        printf("CREATEFRAME\n");
        printf("PUSHFRAME\n");
        printf("CALL $%s\n", call_node->attribute.name);
        printf("POPFRAME\n");
    }
}

void g_assign(ast_node_t *assign_node) {
    int no_assigns;

    //pokud je posledni dite func_call
    if (assign_node->child_arr[assign_node->no_children - 1].id == func_call_id) {
        no_assigns = (assign_node->no_children - 1) / 2;

        g_func_call(&assign_node->child_arr[assign_node->no_children - 1]);

        //popnu a ulozim navratove hodnoty
        for (int i = 0; i < no_assigns; i++) {
            printf("POPS LF@%s\n", assign_node->child_arr[i].attribute.name);
        }
    } else {
        no_assigns = assign_node->no_children / 2;
        for (int i = 0; i < no_assigns; i++) {
            int expr_no = g_expression(&assign_node->child_arr[no_assigns + i]);
            printf("MOVE LF@%s LF@$expr-res-%d \n", assign_node->child_arr[i].attribute.name, expr_no);
        }
    }
}

void g_func_def(ast_node_t *func_def_node){
    printf("JUMP $jump_over%s\n", func_def_node->attribute.name);

    printf("LABEL $%s\n", func_def_node->attribute.name);

    //definuje promenne z argumentu
    for(int i = 0; i < func_def_node->no_children-2; i++) { //-2 bo posledni je body_id a indexace je od nuly
        printf("DEFVAR LF@%s\n", func_def_node->child_arr[i].attribute.name);
        printf("POPS LF@%s\n", func_def_node->child_arr[i].attribute.name);
    }

    g_body(&func_def_node->child_arr[func_def_node->no_children-1]);
    printf("RETURN\n");

    printf("LABEL $jump_over%s\n", func_def_node->attribute.name);
}



void g_if_condition(ast_node_t *if_condition_node, int current_if_no){
    printf("\n # if - podminka\n");
//pokud je relacni operator
    if(!strcmp(if_condition_node->attribute.name, "<") || !strcmp(if_condition_node->attribute.name, "<=") ||
       !strcmp(if_condition_node->attribute.name, ">") || !strcmp(if_condition_node->attribute.name, ">=") ||
       !strcmp(if_condition_node->attribute.name, "==") || !strcmp(if_condition_node->attribute.name, "~=")) {

        int lres = g_expression(&if_condition_node->child_arr[1]);
        int rres = g_expression(&if_condition_node->child_arr[0]);

        printf("DEFVAR LF@$if-cond-res-%d\n", current_if_no);

        if (!strcmp(if_condition_node->attribute.name, "<")) {
            //pokud je mensi nez
            printf("LT LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
        } else if (!strcmp(if_condition_node->attribute.name, "<=")) {
            //pokud se rovna
            printf("EQ LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
            //pokud je mensi nez
            printf("LT LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);

        } else if (!strcmp(if_condition_node->attribute.name, ">")) {
            //pokud je vetsi nez
            printf("GT LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
        } else if (!strcmp(if_condition_node->attribute.name, ">=")) {
            //pokud se rovna
            printf("EQ LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
            //pokud je vetsi nez
            printf("GT LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);

        } else if (!strcmp(if_condition_node->attribute.name, "==")) {
            //pokud se rovna
            printf("EQ LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
        } else if (!strcmp(if_condition_node->attribute.name, "~=")) {
            //pokud se nerovna
            printf("EQ LF@$if-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_if_no, lres, rres);
            printf("JUMPIFNEQ $if-body%d LF@$if-cond-res-%d 1\n", current_if_no, current_if_no);
        }
    } else { //pokud neni relacni operator
        int res = g_expression(if_condition_node);
        //kontrola zda argumenty nejsou nil
        printf("DEFVAR LF@$if-nil-check-%d\n", current_if_no);
        printf("DEFVAR LF@$if-nil-check-bool-%d\n", current_if_no);

        printf("TYPE LF@$if-nil-check-%d LF@$if-expr-res-%d\n", current_if_no, res);
        printf("EQ LF@$if-nil-check-bool-%d LF@$if-nil-check-%d string@nil\n", current_if_no, current_if_no);
        //pokud se nerovna nil, je true a skocim na if body
        printf("JUMPIFNEQ $if-body%d LF@$if-nil-check-bool-%d bool@true\n", current_if_no, current_if_no);
    }
}

void g_branch(ast_node_t *if_node){
    const int curr_if_no = if_count;
    if_count++;

    printf("# if - zacatek\n");
    g_if_condition(&if_node->child_arr[0], curr_if_no);
    //pokud bude podminka ok, SKOCI na if body. pokud ne, POKRACUJE na else body

    printf("LABEL $else-body%d\n", curr_if_no);
    g_body(&if_node->child_arr[2]);

    printf("JUMP $if-end%d\n", curr_if_no); //po vykonani else body preskoci if vetev


    printf("LABEL $if-body%d\n", curr_if_no);
    g_body(&if_node->child_arr[1]);

    printf("LABEL $if-end%d\n", curr_if_no);

    //pokud condition true, vyskoci na na while-body

}



void g_while_condition(ast_node_t *condition_node, int current_while_no){
    printf("\n # while - podminka\n");

    //pokud je relacni operator
    if(!strcmp(condition_node->attribute.name, "<") || !strcmp(condition_node->attribute.name, "<=") ||
       !strcmp(condition_node->attribute.name, ">") || !strcmp(condition_node->attribute.name, ">=") ||
       !strcmp(condition_node->attribute.name, "==") || !strcmp(condition_node->attribute.name, "~=")) {

        int lres = g_expression(&condition_node->child_arr[1]);
        int rres = g_expression(&condition_node->child_arr[0]);

        printf("DEFVAR LF@$while-cond-res-%d\n", current_while_no);

        if (!strcmp(condition_node->attribute.name, "<")) {
            //pokud je mensi nez
            printf("LT LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
        } else if (!strcmp(condition_node->attribute.name, "<=")) {
            //pokud se rovna
            printf("EQ LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ LF@$while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
            //pokud je mensi nez
            printf("LT LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);

        } else if (!strcmp(condition_node->attribute.name, ">")) {
            //pokud je vetsi nez
            printf("GT LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
        } else if (!strcmp(condition_node->attribute.name, ">=")) {
            //pokud se rovna
            printf("EQ LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
            //pokud je vetsi nez
            printf("GT LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);

        } else if (!strcmp(condition_node->attribute.name, "==")) {
            //pokud se rovna
            printf("EQ LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
        } else if (!strcmp(condition_node->attribute.name, "~=")) {
            //pokud se nerovna
            printf("EQ LF@$while-cond-res-%d LF@$expr-res-%d LF@$expr-res-%d\n",
                   current_while_no, lres, rres);
            printf("JUMPIFNEQ $while-body%d LF@$while-cond-res-%d 1\n", current_while_no, current_while_no);
        }
    } else { //pokud neni relacni operator
        int res = g_expression(condition_node);
        //kontrola zda argumenty nejsou nil
        printf("DEFVAR LF@$while-nil-check-%d\n", current_while_no);
        printf("DEFVAR LF@$while-nil-check-bool-%d\n", current_while_no);

        printf("TYPE LF@$while-nil-check-%d LF@$while-expr-res-%d\n", current_while_no, res);
        printf("EQ LF@$while-nil-check-bool-%d LF@$while-nil-check-%d string@nil\n", current_while_no, current_while_no);
        //pokud se nerovna nil, je true a skocim na zacatek cyklu
        printf("JUMPIFNEQ $while-body%d LF@$write-nil-check-bool-%d bool@true\n", current_while_no, current_while_no);

    }
}


void g_while(ast_node_t *while_node){
    const int curr_while_no = while_count;
    while_count++;
    printf("# while - zacatek\n");
    printf("JUMP $while-cond"); //zkontroluje podminku

    printf("LABEL $while-body%d\n", curr_while_no);
    g_body(&while_node->child_arr[1]);


    printf("LABEL $while-cond%d\n", curr_while_no);
    g_while_condition(&while_node->child_arr[0], curr_while_no);
    //pokud condition true, vyskoci na na while-body
}
