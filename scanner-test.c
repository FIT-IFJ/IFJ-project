#include <assert.h>
#include "scanner.h"


// id/data/keyword
void test_general(token_t *new_token, dynamic_string *string) {
    get_token(new_token, string);
    char *typ;

    switch (new_token->type)
    {
    case TYPE_OPERATOR:
        typ = "operator";
        break;
    case TYPE_STRING:
        typ = "string";
        break;
    case TYPE_INTEGER:
        typ = "integer";
        break;
    case TYPE_DECIMAL:
        typ = "decimal";
        break;
    case TYPE_IDENTIFIER:
        typ = "identifier";
        break;
    case TYPE_KEYWORD:
        typ = "keword";
        break;
    case TYPE_DATATYPE:
        typ = "datatype";
        break;
    case TYPE_ASSIGNMENT:
        typ = "assignment";
        break;
    case TYPE_EOF:
        typ = "eof";
        break;
    default:
        typ = "chyba";
        break;
    }

    printf("%s  ", typ);
    printf("|  %s  ", new_token->attribute);
    printf("|  %i  \n", new_token->line);
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

    printf("ZACATEK PROGRAMU\n");

    while(new_token->type != TYPE_EOF) {
        test_general(new_token, string);
    }

    printf("KONEC PROGRAMU\n");

    // po pouziti potreba uvolnit
    string_free(string);
    free(new_token);
    
    return 0;
}
