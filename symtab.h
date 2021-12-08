/* ******************* symtab.h  ******************************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Hlavickovy soubor k tabulce symbolu                                 */
/*  Vytvoril: Tym 102 - David Novak, listopad 2021                            */
/* ************************************************************************** */
#ifndef SYMTAB_HEADER
#define SYMTAB_HEADER

#include <stdbool.h>
#include "datatype_t.h"
#define HASH_TABLE_SIZE 1 //pocet indexu hash tabulky

struct symtab;    // neúplná deklarace struktury, obsah není zvenčí vidět
typedef struct symtab symtab_t;

/**
 *  @brief vytvoří a inicializuje tabulku symbolů
 *  @return vrací ukzatel na novou tabulku symbolu, při chybě alokace vrací NULL
 */
symtab_t *symtab_create();

/**
 * @brief Oznami tabulce symbolu zacatek noveho bloku
 * @return 1 pokud se podarilo, 0 pokud chyba alokace pameti, -1 neexistujici tabulka symbolu
 */
int start_block(symtab_t *symtab); //push
/**
 * @brief Oznami tabulce symbolu konec noveho bloku
 * @return 1 pokud ok, 0 pokud tabulka symbolu neexistuje
 */
int end_block(symtab_t *symtab); //pop

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů v současném bloku deklarovaná. Pokud je už deklarovaná,
 * vrátí -1. Pokud není, zavede ji do tabulky a vrati 1 (ok).
 * Chyba alokace pameti 0 (chyba)
 */
int define_var(symtab_t *symtab, const char *var_name,  datatype_t var_datatype);

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů deklarovaná.
 * @return Pokud promenna neni v tabulce, nebo pokud tabulka neexistuje, vrátí -1. Pokud je, vrátí: 0 pokud nil,
 * 1 pokud integer, 2 pokud number,
 * 3 pokud string.
 */
int get_var_datatype(symtab_t *symtab, char *var_name);

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů deklarovaná.
 * @return Pokud promenna neni v tabulce, nebo pokud tabulka neexistuje, vrátí NULL. Pokud je, vrátí string uid promenne
 */
char* get_var_uid(symtab_t *symtab, char *var_name);

/**
 * @brief Funkce pro deklaraci funkce. V tabulce vyhledá funkci odpovídající nazvu func_name a:
 *                  - pokud ji nalezne, vrátí -1 (funkce uz deklarovana je, nutno zkontrolovat jestli sedi parametry)
 *                  - pokud nenalezne, automaticky ji přidá a vrátí 1 (nutno jeste ulozit parametry)
 *                  - pokud nastane chyba alokace pameti, vrati 0
 *
 * Pro naši implementci dost možná nepoužitelná, k tomuto účelu jsou tu další fce, ale je elegantní tak ji tu zatím
 * nechám.
 * USAGE:
 * pro funkci:
 * global foo(a:integer, b:string, c:number ) : integer, string
 * declare_function2(symtab, foo, 2, 3, integer, string, a, integer, b, string, c, number);
 * tzn.
 * @param symtab ukazatel na tabulku symbolů
 * @param func_name název funkce
 * @param no_returns počet returnů
 * @param no_params počet parametrů
 * @param ... nejdřív datové typy všech returnů, a potom dvojice název_parametru, datovy_typ_parametru
 * @return
 */
int declare_function2(symtab_t *symtab, char *func_name, int no_returns, int no_params,...);
/**
 * @brief       V tabulce vyhledá funkci odpovídající nazvu func_name a
 *                  - pokud ji nalezne, vrátí -1 (funkce uz deklarovana je, nutno zkontrolovat jestli sedi parametry)
 *                  - pokud nenalezne, automaticky ji přidá a vrátí 1 (nutno jeste ulozit parametry)
 *                  - pokud nastane chyba alokace pameti, vrati 0
 * @param symtab     ukazatel na symtabulku
 * @param func_name   nazev_funkce
 * @return      viz @ brief
 */
int declare_function(symtab_t *symtab, char *func_name);

/**
 * @brief Nastavi priznak funkce is_defined na true. Spravne pouziti je nejdriv declare_function() a pak az toto
 * @param symtab     ukazatel na symtabulku
 * @param func_name   nazev_funkce
 * @return 1 pokud uspesne, 0 pokud neni deklarovana.
 */
int define_function(symtab_t *symtab, char *func_name);

/**
 * @brief vlozi parametr k funkci func_name, parametry vklada postupne, proto je nutne je vkladat ve spravnem poradi
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param datatype datovy typ parametru
 * @param param_name nazev parametru
 * @return 0 pokud chyba pameti, 1 pokud uspesne vlozeno, -1 pokud funkce neni deklarovana
 */
int set_param(symtab_t *symtab, char *func_name, datatype_t datatype, char *param_name);

/**
 * @brief Zjisti nazev parametru.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho parametru datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, nebo parametr na dane pozici neexistuje, vrátí NULL.
 * Jinak vrati ukazatel na retezec s nazvem funkce;
 */
char *get_param_name(symtab_t *symtab, char *func_name, int pos);

/**
 * @brief Zjisti datovy typ parametru.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho parametru datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, nebo parametr na dane pozici neexistuje, vrátí -1. Pokud je, vrátí: 0 pokud
 * nil, 1 pokud integer, 2 pokud number, 3 pokud string.
 */
int get_param_type(symtab_t *symtab, char *func_name, int pos);

/**
 * @brief Zjisti pocet parametru funkce.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return -1 pokud funkce neexistuje jinak pocet parametru
 */
int get_param_count(symtab_t *symtab, char *func_name);

/**
 * @brief vlozi return k funkci func_name, returny vklada postupne, proto je nutne je vkladat ve spravnem poradi
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param datatype datový typ returnu
 * @return 1 pokud ok, 0 pokud chyba paměti, -1 pokud funkce neni deklarovana
 */
int set_return_type(symtab_t *symtab, char *func_name, datatype_t datatype);

/**
 * @brief Zjisti pocet returnu funkce.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return -1 pokud funkce neexistuje jinak pocet returnu
 */
int get_return_count(symtab_t *symtab, char *func_name);

/**
 * @brief Zjisti datovy typ returnu.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho returnu datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, vrátí -1. Pokud je, vrátí: 0 pokud nil, 1 pokud integer, 2 pokud number,
 * 3 pokud string.
 */
int get_return_type(symtab_t *symtab, char *func_name, int pos);

/**
 * @brief Zjisti zda je funkce definovana (i pokud neni deklarovana, vrati false)
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return true pokud je definovana, false pokud neni
 */
bool is_defined(symtab_t *symtab, char *func_name);

/**
 * @brief Zjisti zda je funkce deklarovana
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return true pokud je deklarovana, false pokud neni
 */
bool is_declared(symtab_t *symtab, char *func_name);

#endif //SYMTAB_H
