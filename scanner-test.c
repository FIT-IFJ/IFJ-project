#include <assert.h>
#include "scanner.h"


// id/data/keyword
void test_id(token_t *new_token, dynamic_string *string) {
    get_token(new_token, string);
    printf("LINE: %i\n",new_token->line);
    printf("%s\n",new_token->type);
    printf("%s\n",new_token->attribute);

    get_token(new_token, string);
    printf("LINE: %i\n",new_token->line);
    printf("%s\n",new_token->type);
    printf("%s\n",new_token->attribute);

    get_token(new_token, string);
    printf("LINE: %i\n",new_token->line);
    printf("%s\n",new_token->type);
    printf("%s\n",new_token->attribute);

    get_token(new_token, string);
    printf("LINE: %i\n",new_token->line);
    printf("%s\n",new_token->type);
    printf("%s\n",new_token->attribute);
    return;
}

int main()
{
    // inicializace
    token_t* new_token = malloc(sizeof(token_t));
    if(new_token == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
    }
    dynamic_string *string = string_init();

    // testy - posloupnost dle zadaneho vstupu
    test_id(new_token, string);

    // po pouziti potreba uvolnit
    string_free(string);
    free(new_token);
    
    return 0;
}
