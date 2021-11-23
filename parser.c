/* ******************************* parser.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Syntakticky a sematicky analyzator                                  */
/*  Vytvoril: Tym 102 - Kozub Tadeas, listopad 2021                           */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "scanner.h"


int read_prolog(token_t* token, dynamic_string* string);
int read_require(token_t* token, dynamic_string* string);

int main(){
    token_t* new_token = malloc(sizeof(token_t));
    dynamic_string* string = string_init();
    int result = read_prolog(new_token, string);
    printf("[STATUS CODE OF THE PARSING: %d]\n", result);
    return 0;
}

int read_prolog(token_t* token, dynamic_string* string){
    get_token(token, string);
    printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
    if (!strcmp(token->attribute, "require")) 
    {
        get_token(token, string);
        printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
        if (!strcmp(token->attribute, "\"ifj21\"")) 
        {
            printf("supr, vas program ma require :) \n");
            get_token(token, string);
            printf("THE CURRENT TOKEN IS: %s on line %d\n\n", token->attribute, token->line);
            return 0;        
        }

    }
    printf("Syntax error, missing mandatory \"require\" line \n");
    return 1;  
}