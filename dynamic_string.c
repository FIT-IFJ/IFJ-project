/* ******************************* dynamic_string.h ************************* */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Dynamicky retezec                                                   */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include "dynamic_string.h"

void error_memory(dynamic_string *s) {
    string_free(s);
    fprintf(stderr, "Memory allocation error\n");
    exit(99);
}

dynamic_string *string_init() {

    // alokuji pamet
    dynamic_string *s = (dynamic_string*) malloc(sizeof(dynamic_string));
    if (s == NULL) {
        error_memory(s);
    }

    // inicializuji dynamicky retezec
    s->size = 1;
    s->data = NULL;

    return s;
}

void string_free(dynamic_string *s) {
    // vymaze retezec a uvolni pamet
    free(s->data);
    free(s);
}

void string_add_char(dynamic_string *s, char d) {

    // alokuji pamet
    if (s->size != 1) {
        s->data = (char*) realloc(s->data, ((sizeof(char)) * s->size + 1));
        if (s->data == NULL) {
            error_memory(s);
        }
        // vlozim znak do retezce
        strncat(s->data, &d, 1);
    }
    else {
        s->data = (char*) malloc(sizeof(char)*2);
        if (s->data == NULL) {
            error_memory(s);
        }
        char pole[] = {d, '\0'};
        // vlozim znak do retezce
        strcpy(s->data, pole);
    }

    s->size++;
}

char* string_get(dynamic_string *s) {
    // vrati retezec
    return s->data;
}

void string_delete(dynamic_string *s) {
    s->size = 1;
}
