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
    dynamic_string* string = string_init();
    int result = program(new_token, string);
    printf("[STATUS CODE OF THE PARSING: %d]\n", result);
    if (result == SUCCESS) {
        printf("Your program seems OK.\n");
    }
    return 0;
}

int program(token_t* token, dynamic_string* string){
    int result;
    result = prologue(token, string) && program_body(token, string);
    return result;
}
int prologue(token_t* token, dynamic_string* string){
    get_token(token, string);
    printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
    if (!strcmp(token->attribute, "require")) 
    {
        get_token(token, string);
        printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
        if (!strcmp(token->attribute, "̈́\"ifj21\"")) 
        {
            return SUCCESS;
        }
        else
        {
            return FAILURE;
        } 
    }
    
}
int program_body(token_t* token, dynamic_string* string){
    //int result = 0;
    get_token(token, string);
    printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
    return SUCCESS;
}