/* ******************************* scanner.c ******************************** */
/*  Predmet: IFJ + IAL - FIT VUT Brno                                         */
/*  Projekt: Implementace prekladace imperativniho jazyka IFJ2021             */
/*  Cast: Lexikalni analyzator                                                */
/*  Vytvoril: Tym 102 - Klimecka Alena, rijen 2021                            */
/*  Upravy: Tym 102                                                           */
/* ************************************************************************** */
/*                              ! POZNAMKY !                                   
/*          
/*                                                               
/*                                                                            
/* ************************************************************************** */

#include "scanner.h"

int line = 1;
static char *keyword_table[]= {
    "do",
    "else",
    "end",
    "function",
    "global",
    "if",
    "local",
    "require",
    "return",
    "then",
    "while",
};
static char *datatype_table[]= {
    "number",
    "integer",
    "nil",
    "string",

};

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
    new_token->type = TYPE_OPERATOR;

    // nastavim atribut a radek
    new_token->attribute = string_get(string);
    string_delete(string);
    new_token->line = line;

}


void token_operator_sort(token_t *new_token, dynamic_string *string, char current) 
{   
    char next = getchar();

    // pokud NENASLEDUJE po znaku '~' znak '=' nastane lexikalni chyba
    if (current == '~' && next != '=') {
        lex_error(new_token, string);
    }
    // pokud op znaku '.' nenasleduje '.' nastane lexikalni chyba
    else if (current == '.' && next != '.') {
        lex_error(new_token, string);
    }
    // vratim rovnitko
    else if (current == '=' && next != '=') {
        ungetc(next, stdin);
        string_add_char(string, current);
        new_token->type = TYPE_ASSIGNMENT;
        new_token->attribute = string_get(string);
        string_delete(string);
        new_token->line = line;
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

    // urcim zda jde o keyword
    for (size_t i = 0; i < 11; i++)
    {
        if (strcmp(keyword_table[i], string_get(string)) == 0) {
            new_token->type = TYPE_KEYWORD;   
            new_token->attribute = string_get(string);
            string_delete(string);
            new_token->line = line;
            return;
        }
    }
    // datatype
    for (size_t i = 0; i < 4; i++)
    {
        if (strcmp(datatype_table[i], string_get(string)) == 0) {
            new_token->type = TYPE_DATATYPE;   
            new_token->attribute = string_get(string);
            string_delete(string);
            new_token->line = line;
            return;
        }
    }

    // nastavim token identifikatoru
    new_token->type = TYPE_IDENTIFIER;   
    new_token->attribute = string_get(string);
    string_delete(string);
    new_token->line = line;

}

void general_sequence(token_t *new_token, dynamic_string *string, char c)
{
    char new_char = getchar();
    string_add_char(string, c);
    if (c != '2') {
        if (new_char > 47 && new_char < 58) {
            string_add_char(string, new_char);
            new_char = getchar();
            if ((new_char > 47 && new_char < 58)) {
                string_add_char(string, new_char);
                return;
            }
            else {
                lex_error(new_token, string);
            }
        }
        else {
            lex_error(new_token, string);
        }
    }
    else {
        if (new_char == '5') {
            string_add_char(string, new_char);
            new_char = getchar();
            if (new_char > 47 && new_char < 54) {
                string_add_char(string, new_char);
                return;
            }
            else {
                lex_error(new_token, string);
            }
        }
        else if (new_char > 47 && new_char < 53) {
            string_add_char(string, new_char);
            new_char = getchar();
            if ((new_char > 47 && new_char < 58)) {
                string_add_char(string, new_char);
                return;
            }
            else {
                lex_error(new_token, string);
            }
        }
        else {
            lex_error(new_token, string);
        }
    }

}

void string_token(token_t *new_token,dynamic_string *string) 
{
    char new_char = getchar(); 
    // nactu vsechny znaky retezce
    while (new_char != '"'){
        // nesmi nastat konec radku
        if (new_char == '\n') {
            lex_error(new_token, string);
        }
        // znaky zapisuji primo
        if (new_char > 31 && new_char != 92) {
            string_add_char(string, new_char);
        }
        // escape sekvence ze zadani
        else if (new_char == 92) {
            string_add_char(string, new_char);
            new_char = getchar();
            switch (new_char)
            {
            // validni sekvence
            case 110:
            case 116:
            case 34:
            case 92:
                string_add_char(string, new_char);
                break;
            // obecna escape sekvence
            case '0' ... '2':
                general_sequence(new_token, string, new_char);
                break;
            // v ostatnich pripadech error
            default:
                lex_error(new_token, string);
                break;
            }
        }

        new_char = getchar();
    }
    // ukoncujici znak "
    string_add_char(string, new_char);

    // nastavim token
    new_token->type = TYPE_STRING;     
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
    new_token->type = TYPE_DECIMAL; 
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
                new_token->type = TYPE_DECIMAL; 
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
        new_token->type = TYPE_INTEGER;
        new_token->attribute = string_get(string);
        string_delete(string);
        new_token->line = line;
    }

}

void detect_block_comment(token_t *new_token, dynamic_string *string) {
    char current = getchar();
    int error_line = 0;

    // preskocim vse az po hranatou zavorku
    while (current != ']') {   
        if (current == EOF) {
            line -= error_line;
            lex_error(new_token, string);
        }
        else if (current == '\n') {
            error_line++;
            line++;
        }
        current = getchar();
    }
    // pokud neni ukoncen blokovy kometar
    if ((current = getchar()) != ']') {
        // spoustim rekurzivne
        detect_block_comment(new_token, string);
    }
}

void detect_comment(token_t *new_token, dynamic_string *string) {
    char current = getchar();
    char next = getchar();

    // blokovy komentar
    if (current == '[' && next == '[') {
        detect_block_comment(new_token, string);
        return;
    }
    // radkovy komentar
    else {
        ungetc(next, stdin);
        while(current != '\n' && current != EOF) {
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
        if ((current_char = getchar()) == EOF) {
            new_token->type = TYPE_EOF;
            new_token->attribute = "";
            new_token->line = line;
            return;
        }

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
        case ',':
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
            if ((next_char = getchar()) != '-') {
                string_add_char(string, current_char);
                ungetc(next_char, stdin);
                create_operator_token(new_token, string);
                return;
            }
            // je komentar
            else {
                detect_comment(new_token, string);
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
