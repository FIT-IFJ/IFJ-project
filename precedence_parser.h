/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Precededencny syntaktický analyzator                                */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include "scanner.h"


 #define DOLR 16  // '$' len pre precedencnu tabulku

 #define MARK 17  // nahradza '<' v zasobniku
 #define NONT 18



void error(int err_num);

//#include "structures.h"
//#include "scanner.h"

void parse_expression (DLList *list);
