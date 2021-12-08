/* ******************* our_error.c ****************************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Funkce pro error handling                                           */
/*  Vytvoril: Tym 102 - David Novak, listopad 2021                            */
/* ************************************************************************** */



#include "our_error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Funkce přijme kód chyby a řádek, na kterém se chyba stala, vypíše příslušnou chyovou hlášku na standartní
 * chyový výstup a ukončí program korespondující návratovou hodnotou dle zadání projektu:
 * [návratový-kód] - [chybová hláška]
 * 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
 * 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
 * 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.
 * 4 - sémantická chyba v příkazu přiřazení (typová nekompatibilita)
 * 5 - sémantická chyba v programu – špatný počet/typ parametrů či návratových hodnotu volání funkce či návratu z funkce
 * 6 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
 * 7 - ostatní sémantické chyby
 * 8 - běhová chyba při práci s neočekávanou hodnotou nil
 * 9 - běhová chyba celočíselného dělení nulovou konstantou
 * 99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.)
 * @param err_code Kód chyby
 * @param line Řádek, na kterém chyba nastala.
 */
void error(int err_code, int line) {
    if(err_code == 99){
        fprintf(stderr, "interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.)\n");
        exit(99);
    } else {
        fprintf(stderr, "Řádek %d:\n", line);
        switch (err_code) {
            case 1:
                fprintf(stderr, "chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)\n");
                exit(1);
            case 2:
                fprintf(stderr, "chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)\n");
                exit(2);
            case 3:
                fprintf(stderr, "sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici proměnné, atp.\n");
                exit(3);
            case 4:
                fprintf(stderr, "sémantická chyba v příkazu přiřazení (typová nekompatibilita)\n");
                exit(4);
            case 5:
                fprintf(stderr, "sémantická chyba v programu – špatný počet/typ parametrů či návratových hodnotu volání funkce či návratu z funkce\n");
                exit(5);
            case 6:
                fprintf(stderr, "sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech\n");
                exit(6);
            case 7:
                fprintf(stderr, "ostatní sémantická chyba\n");
                exit(7);
            case 8:
                fprintf(stderr, "běhová chyba při práci s neočekávanou hodnotou nil\n");
                exit(8);
            case 9:
                fprintf(stderr, "běhová chyba celočíselného dělení nulovou konstantou\n");
                exit(9);
            default:
                fprintf(stderr, "nedefinovaná návratová hodnota chyby, posíláš funkci error něco, co nemáš\n");
                exit(-1);
        }
    }
}



