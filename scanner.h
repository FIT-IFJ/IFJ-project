/* ******************************* scanner.h ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Lexikalni analyzator                                                */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamic_string.c"

// Token (obsahuje: typ tokenu, hodnotu, umístění v kódu)
typedef struct {
char* type;
char* attribute;
int line;
} token_t;
