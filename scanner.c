/* ******************************* scanner.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Lexikalni analyzator                                                */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */
/*                              ! POZNAMKY !                                   
/*  - aby slo program spravne zavolat je potreba odstranit main na konci kodu          
/*                         (mam ho tu pro debug)                                               
/*                                                                            
/* ************************************************************************** */

#include "scanner.h"

int line = 1;

void lex_error(token_t *new_token, dynamic_string *string) 
{
    fprintf(stderr, "LINE: %i\nWrong lexem structure!\n", line);
    string_free(string);
    free(new_token);
    exit(1);
}

void create_operator_token(token_t *new_token, dynamic_string *string)
{
    // nastavim typ
    new_token->type = string_get(string);
    string_delete(string);

    // nastavim atribut a radek
    new_token->attribute = new_token->type;
    new_token->line = line;

}


void token_operator_sort(token_t *new_token, dynamic_string *string, char current) 
{   
    char next = getchar();

    // pokud nenasleduje po znaku '~' znak '=' nastane lexikalni chyba
    if (current == '~' && next != '=') {
        lex_error(new_token, string);
    }
    // vratim dvouznakovy token
    else if (next == '=' && current != '/') {
        string_add_char(string, current);
        string_add_char(string, next);
        create_operator_token(new_token, string);
    }
    // vratim token '//'
    else if (current == '/' && next == '/') {
        string_add_char(string, current);
        string_add_char(string, next);
        create_operator_token(new_token, string);
    }
    // vratim token '..'
    else if (current == '.' && next == '.') {
        string_add_char(string, current);
        string_add_char(string, next);
        create_operator_token(new_token, string);
    }
    // vratim jednoznakovy token
    else {
        ungetc(next, stdin);
        string_add_char(string, current);
        create_operator_token(new_token, string);
    }

} 

bool is_valid_char(char c) 
{
    if (c == '_')       // je podtrzitko
        return true;
    else if (48 <= c && c <= 57)  // je cislo
        return true;
    else if (65 <= c && c <= 90)  // je velke pismeno
        return true;
    else if (97 <= c && c <= 122)  // je male pismeno
        return true;
    else
        return false;
}

void create_word_token(token_t *new_token, dynamic_string *string) 
{
    char current;

    // pokud je znak vahodny nactu ho
    while (is_valid_char(current = getchar())) {
        string_add_char(string, current);
    }

    // nevhodny znak vratim zpet
    ungetc(current, stdin);

    // nastavim token
    new_token->type = "id/keyword/datatype";    // musime pozdeji rozhodnout 
    new_token->attribute = string_get(string);
    string_delete(string);
    new_token->line = line;

}

void string_token(token_t *new_token,dynamic_string *string) 
{
    char new_char = getchar(); 
    // nactu vsechny znaky retezce
    while(new_char != '"'){
        // nesmi nastat konec radku
        if (new_char == '\n') {
            lex_error(new_token, string);
        }

        // escape sekvence //
        //    
        // dolpnit //
        //    

        string_add_char(string, new_char);
        new_char = getchar();
    }
    // ukoncujici znak "
    string_add_char(string, new_char);

    // nastavim token
    new_token->type = "string";    // musime pozdeji rozhodnout 
    new_token->attribute = string_get(string);
    string_delete(string);
    new_token->line = line;

}

void make_exponent_token(token_t *new_token, dynamic_string *string) {

    char current;
    // nactu zbyvajici cisla
    while (48 <= (current = getchar()) && current <= 57) {
        string_add_char(string, current);
    }

    // nevhodny znak vratim zpet
    ungetc(current, stdin);

    // nastavim token
    new_token->type = "decimal"; 
    new_token->attribute = string_get(string);
    string_delete(string);
    new_token->line = line; 
}

void make_exponent_num(token_t *new_token, dynamic_string *string) 
{
    char current = getchar();
    string_add_char(string, current);
    // nasleduje cislo
    if (48 <= current && current <= 57){
        make_exponent_token(new_token, string);
    }
    // nasleduje + / -
    else if (current == '+' || current == '-') {
        // nasleduje cislo
        if (48 <= (current = getchar()) && current <= 57) {
            string_add_char(string, current);
            make_exponent_token(new_token, string);
        }
        // nenasleduje cislo -> lexikalni chyba
        else {
            lex_error(new_token, string);
        }
    }
    // nenasleduje ani jedno -> lexikalni chyba
    else {
        lex_error(new_token, string);
    }
}

void create_num_token(token_t *new_token, dynamic_string *string) 
{
    char current;

    // nacitam cisla
    while (48 <= (current = getchar()) && current <= 57) {
        string_add_char(string, current);
    }

    // token je desetinny
    if (current == '.') {

        string_add_char(string, current);

        // token ma desetinnou cast
        if (48 <= (current = getchar()) && current <= 57) {

            string_add_char(string, current);

            // nacitam cisla za desetinnou carkou
            while (48 <= (current = getchar()) && current <= 57) {
                string_add_char(string, current);
            }
            if (current == 'e' || current == 'E') {
                string_add_char(string, current);
                make_exponent_num(new_token, string);
            }
            else {
                ungetc(current, stdin);
                // nastavim token
                new_token->type = "decimal"; 
                new_token->attribute = string_get(string);
                string_delete(string);
                new_token->line = line;
            }
        }
        // token nema desetinnou cast -> lexikalni chyba
        else {
            lex_error(new_token, string);
        }
    }
    else if (current == 'e' || current == 'E') {
        string_add_char(string, current);
        make_exponent_num(new_token, string);
    }

    // token je celociselny
    else {
        ungetc(current, stdin);
        // nastavim token
        new_token->type = "integer";
        new_token->attribute = string_get(string);
        string_delete(string);
        new_token->line = line;
    }

}

void detect_block_comment() {
    char current = getchar();

    // preskocim vse az po hranatou zavorku
    while (current != ']') {   
        if (current == '\n') {
            line++;
        }
        current = getchar();
    }
    // pokud neni ukoncen blokovy kometar
    if (current = getchar() != ']') {
        // spoustim rekurzivne
        detect_block_comment();
    }
}

void detect_comment() {
    char current = getchar();
    char next = getchar();

    // konec komentare
    if (current == '\n' || next == '\n') {
        line++;
        return;
    }
    // blokovy komentar
    else if (current == '[' && next == '[') {
        detect_block_comment();
        return;
    }
    // radkovy komentar
    else {
        while(current != '\n') {
            current = getchar();
        }
        line++;
    }

}

void get_token(token_t *new_token, dynamic_string *string)
{          
    char current_char;
    char next_char;

    while(true) {
        current_char = getchar();

        switch (current_char)
        {
        // bile znaky -> preskakuji
        case ' ':   
            break;
        case '\n':
            line++;     // jsem na dalsim radku
            break;

        // konec hned po prvnim znaku
        case ':':
        case '*':
        case '(':
        case ')':
        case '+':
        case '#':
            string_add_char(string, current_char);
            create_operator_token(new_token, string);
            return;

        // konec po prvnim / druhem znaku
        case '<':
        case '>':
        case '/':
        case '=':
        case '~':
        case '.':
            token_operator_sort(new_token, string, current_char);
            return;

        // id / klicove slovo / datovy typ
        case '_':
        case 'a' ... 'z':    
        case 'A' ... 'Z':
            string_add_char(string, current_char);
            create_word_token(new_token, string);
            return;

        // celociselny / desetinny literal
        case '0' ... '9':
            string_add_char(string, current_char);
            create_num_token(new_token, string);
            return;

        // operator minus / komentar
        case '-':
            // je operator '-'
            if (next_char = getchar() != '-') {
                string_add_char(string, current_char);
                ungetc(next_char, stdin);
                create_operator_token(new_token, string);
                return;
            }
            // je komentar
            else {
                detect_comment();
            }
            break;

        // retezcovy literal
        case '"':
            string_add_char(string, current_char);
            string_token(new_token, string);
            return;
        default:
            break;
        }
    
    }

}


// main jen pro ucely debugovani

/*
int main()
{
    // token 1

    // inicializace
    token_t* new_token = malloc(sizeof(token_t));
    if(new_token == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(99);
    }
    dynamic_string *string = string_init();

    // nacte info k aktualnimu tokenu do new_token
    get_token(new_token, string);

    // pro ucely testovani
    printf("%i\n",new_token->line);
    printf("%s\n",new_token->type);
    printf("%s\n",new_token->attribute);

    // po pouziti potreba uvolnit
    string_free(string);
    free(new_token);
    
    return 0;
}
*/