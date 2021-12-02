/* ******************* our_error.h ****************************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Hlavickovy soubor pro funkci pro error handling                     */
/*  Vytvoril: Tym 102 - David Novak, listopad 2021                            */
/* ************************************************************************** */


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

void error(int err_code, int line);



