// symtab.c by David Novák, IFJ 2021


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// htab.h
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "our_error.h"

// Typy:
typedef const char * htab_key_t;        // typ klíče
typedef char * htab_value_t;              // ukazatel na hodnotu promenne


///linked list dvojic datatype - name

typedef struct linked_list_item {
    datatype_t datatype;
    char* name;
    struct linked_list_item* next;
} LL_item;

typedef struct linked_list_head {
    int lenght;
    struct linked_list_item* next;
} LL;

/**
 * @brief Vytvori novy prazdny linked_list pro data typu [datatype, name, next]
 * @return vraci ukazatel na novy seznam, NULL pokud chyba alokace pameti
 */
LL* LL_create();

/**
 * @brief Vloží nový item na konec LL
 * @param ll ukazatel na LL, do kterého vkládám
 * @param datatype datový typ vkládaného prvku
 * @param name datový typ prvku
 * @return 1 úspěch, 0 chyba alokace paměti, -1 spatne zadane parametry
 */
int LL_insert(LL* ll, datatype_t datatype, char* name);

/**
 * @brief Smaže a dealokuje celý LL
 * @param ll ukazatel na LL
 */
void LL_delete(LL* ll);

/**
 * @brief Zjistí počet prvků seznamu,
 * @param ll ukaztel na LL
 * @return Počet prvků v seznamu.
 */
int LL_lenght(LL* ll);

/**
 * @brief Zjistí datový typ prvku na pozici pos.
 * @param ll ukazatel na LL
 * @param pos pozice prvku
 * @return datový typ prvku na pozici pos
 */
datatype_t LL_get_item_type(LL* ll, int pos);

/**
 * @brief Zjistí název proměnné prvku na pozici pos.
 * @param ll ukazatel na LL
 * @param pos pozice prvku
 * @return ukazatel na řetězec s názvem prvku na pozici pos
 */
char* LL_get_item_name(LL* ll, int pos);

/// end of linked list declarations


// Struktura dat v tabulce promněnných:
typedef struct var_data {
    htab_key_t key;              // ukazatel na klic (nazev promenne)
    char* uid;
    datatype_t type;             // datovy typ
} var_data_t;

// Struktura dat v tabulce funkcí:
typedef struct func_data {
    htab_key_t key;           // ukazatel na klic (nazev funkce)
    datatype_t type;               // datovy typ
    LL *params;
    LL *returns;
    bool is_defined;
} func_data_t;

//struktura bunky var table
typedef struct var_item{
    struct var_item *next;
    var_data_t data;
}var_item_t;

//struktura bunky func table
typedef struct func_item{
    struct func_item *next;
    func_data_t data;
}func_item_t;

typedef func_item_t* func_list_head;
typedef var_item_t* var_list_head;

// Tabulka:
typedef struct var_htab {
    size_t size; //pocet itemu v tabulce
    size_t arr_size; //pocet indexu v tabulce
    var_list_head *item_arr;
} var_htab_t;

typedef struct func_htab {
    size_t size; //pocet itemu v tabulce
    size_t arr_size; //pocet indexu v tabulce
    func_list_head *item_arr;
} func_htab_t;

// Rozptylovací (hash) funkce
size_t htab_hash_function(htab_key_t str);

// Funkce pro práci s tabulkou promennych:
var_htab_t *var_htab_init(size_t n);                    // konstruktor tabulky
void var_htab_free(var_htab_t *t);
int var_htab_lookup_add(var_htab_t *t, htab_key_t key, datatype_t datatype);
void var_htab_clear(var_htab_t * t);
var_data_t * var_htab_find(var_htab_t * t, htab_key_t key);  // hledání
bool var_htab_erase(var_htab_t * t, htab_key_t key);

// Funkce pro práci s tabulkou funkci:
func_htab_t *func_htab_init(size_t n);                    // konstruktor tabulky
void func_htab_free(func_htab_t *t);
int func_htab_lookup_add(func_htab_t * t, htab_key_t key);
void func_htab_clear(func_htab_t * t);
func_data_t * func_htab_find(func_htab_t * t, htab_key_t key);  // hledání
bool func_htab_erase(func_htab_t * t, htab_key_t key);

///var_symtab - zasobnik
typedef struct stack_item stack_item_t;
struct stack_item {
    stack_item_t *below;  // ukazatel na prvek pod prvkem v zasobniku
    var_htab_t *var_ht;             // hodnota (ukazatel na hash tabulku)
};

typedef struct {
    stack_item_t *top;
    stack_item_t *bottom;
}stack_t;

typedef struct symtab {
    stack_t *var_symtab;
    func_htab_t *func_ht;
} symtab_t;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// htab.c
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Vytvori novy prazdny linked_list pro data typu [datatype, name, next]
 * @return vraci ukazatel na novy seznam, NULL pokud chyba alokace pameti
 */
LL* LL_create(){
    //vytvori novy head
    LL* head = (LL*)malloc(sizeof (struct linked_list_head));
    if(!head){
        error(99, 69);
        //fprintf( stderr, "malloc - chyba alokace paměti ve funkci LL_create\n");
        //return NULL;
    }
    //priradi novemu head hodnoty a vrati ho
    head->next = NULL;
    head->lenght = 0;
    return head;
}

/**
 * @brief Vloží nový item na konec LL
 * @param ll ukazatel na LL, do kterého vkládám
 * @param datatype datový typ vkládaného prvku
 * @param name datový typ prvku
 * @return 1 úspěch, 0 chyba alokace paměti, -1 spatne zadane parametry
 */
int LL_insert(LL* ll, datatype_t datatype, char* name){
    //kontrola parametrů
    if(!ll){
        fprintf( stderr, "předán NULL parametrem funkci LL_insert, hrozí siesegv\n");
        return -1;
    }

    //alokuje novy item
    LL_item* new_item = (LL_item*)malloc((sizeof (struct linked_list_item)));
    if(!new_item){
        error(99, 69);
        //fprintf( stderr, "malloc - chyba alokace paměti new_item ve funkci LL_insert\n");
        //return 0;
    }
    if(name) {
        char *new_name = (char *) calloc(sizeof(strlen(name) + 1), sizeof(char));
        if (!new_name) {
            error(99, 69);
            //free(new_item);
            //fprintf(stderr, "calloc - chyba alokace paměti new_name ve funkci LL_insert\n");
            //return 0;
        }
        //inicializuje novy item
        strcpy(new_name, name);
        new_item->name = new_name;
    } else {
        new_item->name = NULL;
    }
    new_item->next = NULL;
    new_item->datatype = datatype;

    if(!ll->next) {
        ll->next = new_item;
        ll->lenght++;
    } else {
        struct linked_list_item* tmp = ll->next;
        if(!tmp->next){
            ll->next->next = new_item;
            ll->lenght++;
            return 1;
        }
        while (tmp->next->next){     //dostane se na konec ll
            tmp->next = tmp->next->next;
        }
        tmp->next->next = new_item;
        ll->lenght++;
    }
    return 1;
}

/**
 * @brief Smaže a dealokuje celý LL
 * @param ll ukazatel na LL
 */
void LL_delete(LL* ll){
    if(!ll) { //pokud je uazatel NULL, nic neudělá
        return;
    }
    LL_item *tmp = ll->next;
    LL_item *to_delete;
    while(tmp){
        to_delete = tmp;
        tmp = tmp->next;
        free(to_delete->name);
        free(to_delete);
    }
    free(ll);
}

/**
 * @brief Zjistí počet prvků seznamu,
 * @param ll ukaztel na LL
 * @return Počet prvků v seznamu, -1 parametr NULL
 */
int LL_lenght(LL* ll){
    if(!ll){
        fprintf( stderr, "předán NULL parametrem funkci LL_lenght, hrozí siesegv\n");
        return -1;
    }
    return ll->lenght;
}
/**
 * @brief Zjistí datový typ prvku na pozici pos.
 * @param ll ukazatel na LL
 * @param pos pozice prvku
 * @return datový typ prvku na pozici pos
 */
datatype_t LL_get_item_type(LL* ll, int pos) {
    if(!ll){
        fprintf( stderr, "předán NULL parametrem funkci LL_get_item_type, hrozí siesegv\n");
        return nil;
    }
    if(LL_lenght(ll)<pos){ //pokud je seznam prazdny
        fprintf( stderr, "hledáš v LL_get_item_type na pozici mimo seznam, hrozí siesegv\n");
        return nil;
    }
    LL_item *tmp = ll->next;
    //najdu pos. prvek
    for (int i = 0; i < pos; i++) {
        tmp = tmp->next;
    }
    return tmp->datatype;
}

/**
 * @brief Zjistí název proměnné prvku na pozici pos.
 * @param ll ukazatel na LL
 * @param pos pozice prvku
 * @return ukazatel na řetězec s názvem prvku na pozici pos, NULL pokud chyba
 */
char* LL_get_item_name(LL* ll, int pos){
    if(!ll){
        fprintf( stderr, "předán NULL parametrem funkci LL_get_item_name, hrozí siesegv\n");
        return NULL;
    }
    if(LL_lenght(ll)<pos){ //pokud je seznam prazdny
        fprintf( stderr, "hledáš v LL_get_item_name na pozici mimo seznam, hrozí siesegv\n");
        return NULL;
    }
    LL_item *tmp = ll->next;
    //najdu pos. prvek
    for (int i = 0; i < pos; i++) {
        tmp = tmp->next;
    }
    return tmp->name;
}

/// end of linked list

/// hashovaci tabulka

/**
 * @brief       Hashovací funkce
 * @param str   klíč, ke kterému mám vypočítat hodnotu hashe
 * @return h    hodnota hashe
 */

size_t htab_hash_function(const char *str) {
    uint32_t hash=0;     // musí mít 32 bitů
    const unsigned char *i;
    for(i=(const unsigned char*)str;*i!='\0'; i++) {
        hash = 65599 * hash + *i;
    }
    return hash;
}

/**
 * @brief Konstruktor htabulky
 * @param n     počet prvků pole htabulky (.arr_size)
 * @return      ukazatel na htabulku, pri chybe alokace NULL
 */
var_htab_t *var_htab_init(size_t n){
    var_htab_t *hash_table = malloc(sizeof(var_htab_t)); //alokuje strukturu tabulky
    if (hash_table == NULL) {
        error(99, 69);
        //fprintf(stderr, "htab.c/htab_init/malloc - chyba alokace paměti pro hash_table\n");
        //return NULL;
    }
    var_list_head *item_arr = calloc(n, sizeof(var_list_head *)); //alokuje a nuluje pole n ukazatelu na linked listy
    if (item_arr == NULL) {
        error(99, 69);
        //fprintf(stderr, "htab.c/htab_init/calloc - chyba alokace paměti pro item_arr\n");
        //free(hash_table);
        //return NULL;
    }
    //inicializace
    hash_table->size     = 0;
    hash_table->arr_size = n;
    hash_table->item_arr = item_arr;
    return hash_table;
}

/**
 * @brief Destruktor htabulky
 * @param t     ukazatel na htabulku
 */
void var_htab_free(var_htab_t *t){
    var_htab_clear(t);
    free(t->item_arr);
    free(t);
}

/**
 * @brief zrušení všech polžek, htabulka zůstane prázdná
 * @param t     ukazatel na htabulku
 */
void var_htab_clear(var_htab_t * t){

    for (unsigned int i = 0; i < t->arr_size; i++){
        while (t->item_arr[i] != NULL ){
            var_htab_erase(t, t->item_arr[i]->data.key);
        }
    }
}

/**
 * @brief       zrušení záznamu se zadaným klíčem z htabulky
 * @param t     ukazatel na htabulku
 * @param key   řetězec-klíč na odstranění
 * @return      pokud úspěch tak true, pokud klíč v htabulce není tak false
 */
bool var_htab_erase(var_htab_t * t, htab_key_t key){
    unsigned int index = (htab_hash_function(key) % t->arr_size);
    var_item_t *p_node = t->item_arr[index];
    var_item_t *p_last = NULL;
    while (p_node != NULL){
        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            htab_key_t key_to_free = p_node->data.key;
            char *uid_to_free = p_node->data.uid;
            var_item_t * node_to_free = p_node;
            if(p_last == NULL){
                t->item_arr[index] = p_node->next;
            } else {
                p_last->next = p_node->next;
            }
            //puts(key_to_free); //vypis pro ladeni
            free(uid_to_free);
            free((void*)key_to_free);
            free(node_to_free);
            t->size -= 1;
            return true;
        }
        p_last = p_node;
        p_node = p_node->next;
    }
    return false;
}




int uid_value = 0;
char *generate_uid(char* varname){
    char *uid = malloc(sizeof(char)* (strlen(varname)+11+1)); //11 = počet cifer hodnoty int_max; +1 na \0
    if(!uid){
        error(99, 69);
    }
    sprintf(uid,"%s%d", varname, uid_value);
    uid_value++;
    return uid;
}

/**
 * @brief       V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
 *                  - pokud jej nalezne, vrátí -1
 *                  - pokud nenalezne, automaticky přidá záznam a vrátí 1
 *                  - pokud nastane chyba alokace pameti, vrati 0
 * @param t     ukazatel na htabulku
 * @param key   řetězec pro vložení
 * @return      ukazatel na starý/nově přidaný záznam
 */
int var_htab_lookup_add(var_htab_t *t, htab_key_t key, datatype_t datatype) {

    unsigned int index = (htab_hash_function(key) % t->arr_size);
    var_item_t *p_node = t->item_arr[index];
    var_item_t *p_last = t->item_arr[index];

    while (p_node != NULL){ //prochazi linkde list


        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            return -1;//&p_node->pair;            //vrati ukazatel na pair
        }
        p_last = p_node;
        p_node = p_node->next;
    }
    size_t len = strlen(key)+1; //+1 protoze misto pro '/0'
    char *p_key = malloc(len*sizeof(char));
    if (!p_key){
        error(99, 69);
        //fprintf(stderr, "htab.c/htab_lookup_add/malloc - chyba alokace paměti pro p_key\n");
        //return 0; //NULL
    }

    var_item_t *p_new_node = malloc(sizeof(var_item_t));
    if (!p_new_node){
        error(99, 69);
        //fprintf(stderr, "htab.c/htab_lookup_add/malloc - chyba alokace paměti pro p_new_node\n");
        //free(p_key);
        //return 0; //NULL
    }
    strcpy(p_key, key);

    p_new_node->data.key   = p_key;
    p_new_node->data.type = datatype;
    p_new_node->data.uid = generate_uid(p_key);
    p_new_node->next       = NULL;

    if(p_last == NULL){
        t->item_arr[index] = p_new_node;
    } else {
        p_last->next = p_new_node;
    }
    t->size += 1;

    return 1; //&p_new_node->pair;
}

/**
 * @brief       V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
 *                  - pokud jej nalezne, vrátí ukazatel na záznam
 *                  - pokud nenalezne, vrátí NULL
 * @param t     ukazatel an htabulku
 * @param key   hledaný řetězec
 * @return      ukazatel na záznam, pokud nenajde NULL
 */
var_data_t *var_htab_find(var_htab_t *t, htab_key_t key){
    unsigned int index = (htab_hash_function(key) % t->arr_size);
    var_item_t *p_node = t->item_arr[index];

    while (p_node != NULL){
        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            return &p_node->data;
        }
        p_node = p_node->next;
    }
    return NULL;
}
/// ======================== func ==============


/**
 * @brief Konstruktor htabulky
 * @param n     počet prvků pole htabulky (.arr_size)
 * @return      ukazatel na htabulku, pri chybe pameti NULL
 */
func_htab_t *func_htab_init(size_t n){
    func_htab_t *hash_table = malloc(sizeof(func_htab_t)); //alokuje strukturu tabulky
    if (!hash_table) {
        error(99, 69);
        //return NULL;
    }
    func_list_head *item_arr = calloc(n, sizeof(func_list_head *)); //alokuje a nuluje pole n ukazatelu na linked listy
    if (!item_arr) {
        error(99, 69);
        //free(hash_table);
        //return NULL;
    }
    //inicializace
    hash_table->size     = 0;
    hash_table->arr_size = n;
    hash_table->item_arr = item_arr;
    return hash_table;
}

/**
 * @brief Destruktor htabulky
 * @param t     ukazatel na htabulku
 */
void func_htab_free(func_htab_t *t){
    func_htab_clear(t);
    free(t->item_arr);
    free(t);
}

/**
 * @brief zrušení všech polžek, htabulka zůstane prázdná
 * @param t     ukazatel na htabulku
 */
void func_htab_clear(func_htab_t * t){

    for (unsigned int i = 0; i < t->arr_size; i++){
        while (t->item_arr[i] != NULL ){
            func_htab_erase(t, t->item_arr[i]->data.key);
        }
    }
}

/**
 * @brief       zrušení záznamu se zadaným klíčem z htabulky
 * @param t     ukazatel na htabulku
 * @param key   řetězec-klíč na odstranění
 * @return      pokud úspěch tak true, pokud klíč v htabulce není tak false
 */
bool func_htab_erase(func_htab_t * t, htab_key_t key){
    unsigned int index = (htab_hash_function(key) % t->arr_size);
    func_item_t *p_node = t->item_arr[index];
    func_item_t *p_last = NULL;
    while (p_node != NULL){
        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            htab_key_t key_to_free = p_node->data.key;
            func_item_t * node_to_free = p_node;
            if(p_last == NULL){
                t->item_arr[index] = p_node->next;
            } else {
                p_last->next = p_node->next;
            }
            //puts(key_to_free); //vypis pro ladeni
            free((void*)key_to_free);
            LL_delete(node_to_free->data.params);
            LL_delete(node_to_free->data.returns);
            free(node_to_free);
            t->size -= 1;
            return true;
        }
        p_last = p_node;
        p_node = p_node->next;
    }
    return false;
}

/**
 * @brief       V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
 *                  - pokud jej nalezne, vrátí -1
 *                  - pokud nenalezne, automaticky přidá záznam a vrátí 1
 *                  - pokud nastane chyba alokace pameti, vrati 0
 * @param t     ukazatel na htabulku
 * @param key   řetězec pro vložení
 * @return      ukazatel na starý/nově přidaný záznam
 */
int func_htab_lookup_add(func_htab_t *t, htab_key_t key) {

    unsigned int index = (htab_hash_function(key) % t->arr_size);
    func_item_t *p_node = t->item_arr[index];
    func_item_t *p_last = t->item_arr[index];

    while (p_node != NULL){ //prochazi linked list


        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            return -1;//&p_node->pair;            //vrati ukazatel na pair
        }
        p_last = p_node;
        p_node = p_node->next;
    }
    size_t len = strlen(key)+1; //+1 protoze misto pro '/0'
    char *p_key = malloc(len*sizeof(char));
    if (!p_key){
        error(99, 69);
        //return 0; //NULL
    }

    func_item_t *p_new_node = malloc(sizeof(func_item_t));
    if (!p_new_node){
        error(99, 69);
        //free(p_key);
        //return 0; //NULL
    }

    strcpy(p_key, key);

    p_new_node->data.key   = p_key;
    p_new_node->data.is_defined = false;
    p_new_node->data.params = LL_create();
    p_new_node->data.returns = LL_create();
    p_new_node->next       = NULL;

    if(p_last == NULL){
        t->item_arr[index] = p_new_node;
    } else {
        p_last->next = p_new_node;
    }
    t->size += 1;

    return 1; //&p_new_node->pair;
}




/**
 * @brief       V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
 *                  - pokud jej nalezne, vrátí ukazatel na záznam
 *                  - pokud nenalezne, vrátí NULL
 * @param t     ukazatel an htabulku
 * @param key   hledaný řetězec
 * @return      ukazatel na záznam, pokud nenajde NULL
 */
func_data_t *func_htab_find(func_htab_t *t, htab_key_t key){
    unsigned int index = (htab_hash_function(key) % t->arr_size);
    func_item_t *p_node = t->item_arr[index];

    while (p_node != NULL){
        if(!strcmp(p_node->data.key, key)) { //pokud je key shodny s aktualni bunkou htable
            return &p_node->data;
        }
        p_node = p_node->next;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// symtab.c
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/**
 *  @brief vytvoří a inicializuje tabulku symbolů
 *  @return vrací ukzatel na novou tabulku symbolu, při chybě alokace vrací NULL
 */
symtab_t *symtab_create(){
    symtab_t *symtab = (symtab_t*)malloc(sizeof(symtab_t));
    if (!symtab) {
        error(99, 69);
        //fprintf( stderr, "symtab.c/symtab_create/malloc - chyba alokace paměti pro symtab\n");
        //return NULL; //chyba alokace pameti
    }
    symtab->var_symtab = malloc(sizeof(stack_t));
    if(!symtab->var_symtab){
        error(99, 69);
        //fprintf( stderr, "symtab.c/symtab_create/malloc - chyba alokace paměti pro symtab->var_symtab\n");
        //free(symtab);
        //return NULL;
    }
    symtab->var_symtab->top = NULL;
    symtab->var_symtab->bottom = NULL;

    symtab->func_ht = func_htab_init(HASH_TABLE_SIZE);
    if(!symtab->func_ht){
        error(99, 69);
        //fprintf( stderr, "symtab.c/symtab_create/func_ht");
        //free(symtab->var_symtab);
        //free(symtab);
        //return NULL;
    }
    return symtab;
}

/**
 * @brief odstrani tabulku a dealokuje vsechnu jeji pamet, pokud se pošle nevytvorena tabulka (jen NULL) neudela nic
 * @param symtab tabulka symbolu na zniceni a dealokaci
 * @return 1 ok
 */
void symtab_destroy(symtab_t **symtab) {
    if(!(*symtab)){
        return;
    }
    while ((*symtab)->var_symtab->top){
        end_block(*symtab);
    }
    free((*symtab)->var_symtab);
    func_htab_free((*symtab)->func_ht);

    free((*symtab));
    (*symtab) = NULL;
}



/**
 * @brief Oznami tabulce symbolu zacatek noveho bloku
 * @return 1 pokud se podarilo, 0 pokud chyba alokace pameti, -1 neexistujici tabulka symbolu
 */
int start_block(symtab_t *symtab){ //push
    if(!symtab){
        fprintf(stderr, "Chyba: snažíš zacit blok v neexistující tabulce symbolů\n");
        return -1;
    }
    //alokace itemu zasobniku
    stack_item_t *new_block = (stack_item_t*)malloc(sizeof(stack_item_t));
    if (!new_block) {
        error(99, 69);
        //fprintf( stderr, "symtab.c/start_block/malloc - chyba alokace paměti pro new_block\n");
        //return 0; //chyba alokace pameti
    }
    new_block->var_ht = var_htab_init(HASH_TABLE_SIZE);
    if (!new_block->var_ht) {
        error(99, 69);
        //free(new_block);
        //fprintf( stderr, "symtab.c/start_block\n");
        //return 0; //chyba alokace pameti
    }
    //pushnuti itemu
    if (!symtab->var_symtab->bottom) { //pokud vkladam do prazdneho zasobniku
        new_block->below = NULL;
        symtab->var_symtab->bottom = new_block;
        symtab->var_symtab->top = new_block;
    } else { //pokud neco v zasobniku je
        new_block->below = symtab->var_symtab->top;
        symtab->var_symtab->top = new_block;
    }
    return 1;
}

/**
 * @brief Oznami tabulce symbolu konec noveho bloku
 * @return 1 pokud ok, 0 pokud tabulka symbolu neexistuje
 */
int end_block(symtab_t *symtab) { //pop
    if(!symtab){
        fprintf(stderr, "Chyba: snažíš ukoncit blok v neexistující tabulce symbolů\n");
        return -1;
    }
    stack_t *var_symtab = symtab->var_symtab;
    stack_item_t *block_to_delete = var_symtab->top;
    var_htab_t *htab_to_delete = var_symtab->top->var_ht;

    if (var_symtab->top->below) { //pokud ma zasobnik vice prvku
        var_symtab->top = var_symtab->top->below;
    } else {
        var_symtab->top = NULL;
        var_symtab->bottom = NULL;
    }

    var_htab_free(htab_to_delete);
    free(block_to_delete);
    return 1;
}

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů v současném bloku deklarovaná. Pokud je už deklarovaná,
 * vrátí -1 (chyba). Pokud není, zavede ji do tabulky a vrati 1 (ok).
 * Chyba alokace pameti 0 (chyba)
 */
int define_var(symtab_t *symtab, const char *var_name,  datatype_t var_datatype) {
    if(!symtab){
        fprintf(stderr, "Chyba: snažíš se vložit proměnnou do neexistující tabulky symbolů\n");
        return -1;
    }
    if(!symtab->var_symtab){
        fprintf(stderr, "Chyba: snažíš se vložit proměnnou do neexistující tabulky symbolů\n");
        return -1;
    }
    if(!symtab->var_symtab->top) {
        fprintf(stderr, "Chyba: snažíš se vložit proměnnou do globálního rámce\n");
        return -1;
    }
    int result = var_htab_lookup_add(symtab->var_symtab->top->var_ht, var_name, var_datatype);
    if(!result){
        error(99, 69);
        //fprintf(stderr, "symtab.c/define_var\n");
    }
    return result;
}

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů deklarovaná.
 * @return Pokud promenna neni v tabulce, nebo pokud tabulka neexistuje, vrátí -1. Pokud je, vrátí: 0 pokud nil, 1 pokud integer, 2 pokud number,
 * 3 pokud string.
 */
int get_var_datatype(symtab_t *symtab, char *var_name) {
    if(!symtab){
        return -1;
    }
    if(!symtab->var_symtab){
        return -1;
    }
    if(!symtab->var_symtab->top) {
        return -1;
    }
    stack_item_t *current_layer = symtab->var_symtab->top;
    do {
        var_data_t *data = var_htab_find(current_layer->var_ht, var_name);
        if (!data) {
            current_layer = current_layer->below;
        } else {
            switch (data->type) {
                case (0):
                    return 0;
                case (1):
                    return 1;
                case (2):
                    return 2;
                case (3):
                    return 3;
            }
        }
    } while (current_layer);
    return -1;
}

/**
 * @brief Zkontroluje, jestli je proměnná již v tabulce
 * symbolů deklarovaná.
 * @return Pokud promenna neni v tabulce, nebo pokud tabulka neexistuje, vrátí NULL. Pokud je, vrátí string uid promenne
 */
char* get_var_uid(symtab_t *symtab, char *var_name){
    if(!symtab){
        return NULL;
    }
    if(!symtab->var_symtab){
        return NULL;
    }
    if(!symtab->var_symtab->top) {
        return NULL;
    }
    stack_item_t *current_layer = symtab->var_symtab->top;
    do {
        var_data_t *data = var_htab_find(current_layer->var_ht, var_name);
        if (!data) {
            current_layer = current_layer->below;
        } else {
            return data->uid;
        }
    } while (current_layer);
    return NULL;
}
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
 * declare_function2(symtab, foo, 2, 3, integer, string, "a", integer, "b", string, "c", number);
 * tzn.
 * @param symtab ukazatel na tabulku symbolů
 * @param func_name název funkce
 * @param no_returns počet returnů
 * @param no_params počet parametrů
 * @param ... nejdřív datové typy všech returnů, a potom dvojice název_parametru, datovy_typ_parametru
 * @return
 */
int declare_function2(symtab_t *symtab, char *func_name, int no_returns, int no_params,...){

    va_list valist;
    va_start(valist, no_params);

    int x = func_htab_lookup_add(symtab->func_ht, func_name);


    if (x == 1) {
        for (int i = 0; i < (no_params + no_returns); i++) {
            if (i < no_returns) {
                set_return_type(symtab, func_name, va_arg(valist, datatype_t));
            } else {
                set_param(symtab, func_name, va_arg(valist, datatype_t), va_arg(valist, char*));
            }
        }
    }
    return x;


    va_end(valist);
}

/**
 * @brief       V tabulce vyhledá funkci odpovídající nazvu func_name a
 *                  - pokud ji nalezne, vrátí -1 (funkce uz deklarovana je, nutno zkontrolovat jestli sedi parametry)
 *                  - pokud nenalezne, automaticky ji přidá a vrátí 1 (nutno jeste ulozit parametry)
 *                  - pokud nastane chyba alokace pameti, vrati 0
 * @param symtab     ukazatel na symtabulku
 * @param func_name   nazev_funkce
 * @return      viz @ brief
 */
int declare_function(symtab_t *symtab, char *func_name){
    return func_htab_lookup_add(symtab->func_ht, func_name);
}

/**
 * @brief Nastavi priznak funkce is_defined na true. Spravne pouziti je nejdriv declare_function() a pak az toto
 * @param symtab     ukazatel na symtabulku
 * @param func_name   nazev_funkce
 * @return 1 pokud uspesne, 0 pokud neni deklarovana.
 */
int define_function(symtab_t *symtab, char *func_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(data) {
        data->is_defined = true;
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief vlozi parametr k funkci func_name, parametry vklada postupne, proto je nutne je vkladat ve spravnem poradi
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param datatype datovy typ parametru
 * @param param_name nazev parametru
 * @return 0 pokud chyba pameti, 1 pokud uspesne vlozeno, -1 pokud funkce neni deklarovana
 */
int set_param(symtab_t *symtab, char *func_name, datatype_t datatype, char *param_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data){
        return -1;
    }
    if (LL_insert(data->params, datatype, param_name) == 0) {
        return 0;
    }
    return 1;
}

/**
 * @brief Zjisti nazev parametru.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho parametru datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, nebo parametr na dane pozici neexistuje, vrátí NULL.
 * Jinak vrati ukazatel na retezec s nazvem funkce;
 */
char *get_param_name(symtab_t *symtab, char *func_name, int pos){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data) {
        fprintf( stderr, "Chyba: get_param_name - funkce neni deklarovana\n");
        return NULL;
    }
    if (pos >= LL_lenght(data->params)){
        fprintf( stderr, "Chyba: get_param_name - parametr na dane pozici neexistuje\n");
        return NULL;
    }
    return LL_get_item_name(data->params, pos);
}

/**
 * @brief Zjisti datovy typ parametru.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho parametru datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, nebo parametr na dane pozici neexistuje, vrátí -1. Pokud je, vrátí: 0 pokud
 * nil, 1 pokud integer, 2 pokud number, 3 pokud string.
 */
int get_param_type(symtab_t *symtab, char *func_name, int pos){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data) {
        fprintf( stderr, "Chyba: get_param_type - funkce neni deklarovana\n");
        return -1;
    }
    if (pos >= LL_lenght(data->params)){
        fprintf( stderr, "Chyba: get_param_type - parametr na dane pozici neexistuje\n");
        return -1;
    }
    return LL_get_item_type(data->params, pos);
}

/**
 * @brief Zjisti pocet parametru funkce.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return -1 pokud funkce neexistuje jinak pocet parametru
 */
int get_param_count(symtab_t *symtab, char *func_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data) {
        fprintf( stderr, "Chyba: get_param_count - funkce neni deklarovana\n");
        return -1;
    }
    return LL_lenght(data->params);
}

/**
 * @brief vlozi return k funkci func_name, returny vklada postupne, proto je nutne je vkladat ve spravnem poradi
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param datatype datový typ returnu
 * @return 1 pokud ok, 0 pokud chyba paměti, -1 pokud funkce neni deklarovana
 */
int set_return_type(symtab_t *symtab, char *func_name, datatype_t datatype){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data){
        fprintf( stderr, "Chyba: set_return_type - funkce neni deklarovana\n");
        return -1;
    }
    if (LL_insert(data->params, datatype, NULL) == 0) {
        return 0;
    }
    return 1;
}

/**
 * @brief Zjisti pocet returnu funkce.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return -1 pokud funkce neexistuje jinak pocet returnu
 */
int get_return_count(symtab_t *symtab, char *func_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data) {
        fprintf( stderr, "Chyba: get_return_count - funkce neni deklarovana\n");
        return -1;
    }
    return LL_lenght(data->returns);
}

/**
 * @brief Zjisti datovy typ returnu.
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @param pos ktereho returnu datovy tup chci vedet?
 * @return Pokud funkce neni deklarovana, vrátí -1. Pokud je, vrátí: 0 pokud nil, 1 pokud integer, 2 pokud number,
 * 3 pokud string.
 */
int get_return_type(symtab_t *symtab, char *func_name, int pos){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data) {
        return -1;
    }
    return LL_get_item_type(data->returns, pos);
}

/**
 * @brief Zjisti zda je funkce definovana (i pokud neni deklarovana, vrati false)
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return true pokud je definovana, false pokud neni
 */
bool is_defined(symtab_t *symtab, char *func_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data){
        return false;
    }
    return data->is_defined;
}

/**
 * @brief Zjisti zda je funkce deklarovana
 * @param symtab ukazatel na symtabulku
 * @param func_name nazev funkce
 * @return true pokud je deklarovana, false pokud neni
 */
bool is_declared(symtab_t *symtab, char *func_name){
    func_data_t *data = func_htab_find(symtab->func_ht, func_name);
    if(!data){
        return false;
    } else {
        return true;
    }
}

