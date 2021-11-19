/* ******************************* dynamic_string.h ************************* */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Dynamicky retezec                                                   */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* data;
    int size;
} dynamic_string;

void error_memory(dynamic_string *s);
// Vytvorim retezec
dynamic_string *string_init();
// Vymazu retezec
void string_free(dynamic_string *s);
// Pridam znak
void string_add_char(dynamic_string *s, char d);
// Dostanu obsah rezetce
char* string_get(dynamic_string* s);
// Vymazu obsah retezce
void string_delete(dynamic_string *s);
