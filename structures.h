/* ************************* precedence_parser.h **************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast:    Abstraktne struktury pre parser                                  */
/*  Vytvoril: Tym 102 - Bubakova Natalia, listopad 2021                       */
/*  Upravy:   Tym 102                                                         */
/* ************************************************************************** */


#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"

#include "our_error.h"

// struktury pre DLL
typedef struct DLLElement {
	token_t *token;
    struct DLLElement *prev;
    struct DLLElement *next;
} DLLElement;

typedef struct {
	DLLElement* first;
	DLLElement* last;
	DLLElement* active;
} DLList;


/**
 * @brief Funkcia inicializuje DLL strukturu 
 * 
 * @param list Ukazatel na vopred alokovanu DLL strukturu
 */
void DLL_Init( DLList *list );

/**
 * @brief Funkcia cez *token odovzda obsah tokenu prveho prvku DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 * @param token Ukazatel na inicializovanu strukturu token, ktory sa naplni pozadovanou hodnotou
 */
void DLL_GetFirst( DLList *list, token_t *token );

/**
 * @brief Funkcia cez *token odovzda obsah tokenu posledneho prvku DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 * @param token Ukazatel na inicializovanu strukturu token, ktory sa naplni pozadovanou hodnotou
 */
void DLL_GetLast( DLList *list, token_t *token );

/**
 * @brief Funkcia vlozi obsah tokenu predavaneho cez parameter *token do tokenu prveho prvku DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 * @param token Ukazatel na inicializovanu strukturu token, ktory ukazuje na hodnotu, ktoru chce vlozit do DLL
 */
void DLL_InsertFirst( DLList *list, token_t *token );

/**
 * @brief Funkcia vlozi obsah tokenu predavaneho cez parameter *token do tokenu posledneho prvku DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 * @param token Ukazatel na inicializovanu strukturu token, ktory ukazuje na hodnotu, ktoru chce vlozit do DLL
 */
void DLL_InsertLast( DLList *list, token_t *token );

/**
 * @brief Funkcia zmaze prvy prvok struktury DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 */
void DLL_DeleteFirst( DLList *list );


/**
 * @brief Funkcia zmaze posledny prvok struktury DLL
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 */
void DLL_DeleteLast( DLList *list );


/**
 * @brief Funkcia zmaze vsetky prvky struktury DLL, zoznam je nasledne ako po inicializacii
 * 
 * @param list Ukazatel na inicializovanou DLL strukturu
 */
void DLL_Dispose( DLList *list );




/* nasleduju funkcie pre DLL prvok 'active' */

void DLL_First( DLList *list );
void DLL_Last( DLList *list );
void DLL_DeleteAfter( DLList *list );
void DLL_DeleteBefore( DLList *list ); 
void DLL_InsertAfter( DLList *list, token_t *token );
void DLL_InsertBefore( DLList *list, token_t *token );
void DLL_GetValue( DLList *list, token_t *token );
void DLL_SetValue( DLList *list, token_t *token );
void DLL_Next( DLList *list );
void DLL_Previous( DLList *list );
int DLL_IsActive( DLList *list ) ;

#endif
