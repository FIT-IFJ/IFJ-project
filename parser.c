#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include "scanner.h"
#include "precedence_parser.h"



// docasna nahrada hlavneho parseru pre debugovanie PSA
int main()
{
    // inicializace tokenu
    token_t* new_token = malloc(sizeof(token_t));
    token_t* tmp_token = malloc(sizeof(token_t));
    if(new_token == NULL || tmp_token == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
    }
    dynamic_string *string = string_init();

    get_token(new_token, string);
     //parser spracuje prvy token   #############################
    printf("parser processing      '%s'\n", new_token->attribute);


    while(new_token->type != TYPE_EOF)
    {

        //spracuva "=", "if", "while", "return"
        if (new_token->type == TYPE_ASSIGNMENT || strcmp(new_token->attribute,"if") == 0  || \
                strcmp(new_token->attribute,"while") == 0  ||  strcmp(new_token->attribute,"return") == 0)
        {

            printf("\n\n******* expression *******\n");
       //new_token obsahuje podozrivy token pre zaciatok vyrazu a tmp_token je lubovolny, psa ho plni predoslym tokenom
            int res = parse_expression(new_token, string, tmp_token);
            printf("\n\n******* parser *******\n");

            if(res == 0);   // pokracuje s new_token po uspesnom spracovani vyrazu
            else if (res == 1)
            {
                //parser spracuje omylom vybraty token  (nazov funckie) #############################
                printf("parser processing      '%s'\n", tmp_token->attribute);
            }
            else
                exit(res);
        
        }
        else
        {
            get_token(new_token, string);
        }

        //parser spracuje new_token                                    #############################
        printf("parser processing      '%s'\n", new_token->attribute);


    }

    // po pouziti potreba uvolnit
    string_free(string);
    free(new_token);
    free(tmp_token);

    return 0;
}