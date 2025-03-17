#define _USE_MATH_DEFINES

#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Lexer {
    const char *input;
    int position;
    int length;
};

static Token create_token(TokenType type, double value, char *name, int pos) {
    Token token;
    token.type = type;
    token.value = value;
    token.name = name;
    token.position = pos;
    return token;
}

Lexer *lexer_create(const char *input) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    return lexer;
}

static int is_function_char(char c) {
    return isalpha(c) || c == '_';
}

Token lexer_next(Lexer *lexer) {
    while (lexer->position < lexer->length && 
           isspace(lexer->input[lexer->position])) {
        lexer->position++;
    }

    if (lexer->position >= lexer->length) {
        return create_token(TOKEN_EOF, 0, NULL, lexer->position);
    }

    char current = lexer->input[lexer->position];
    int start_pos = lexer->position;

    // Числа
    if (isdigit(current) || current == '.') {
        char *end;
        double value = strtod(lexer->input + lexer->position, &end);
        lexer->position = end - lexer->input;
        return create_token(TOKEN_NUMBER, value, NULL, start_pos);
    }

    // Операторы и скобки
    switch(current) {
        case '+': lexer->position++; return create_token(TOKEN_PLUS, 0, NULL, start_pos);
        case '-': lexer->position++; return create_token(TOKEN_MINUS, 0, NULL, start_pos);
        case '*': lexer->position++; return create_token(TOKEN_MUL, 0, NULL, start_pos);
        case '/': lexer->position++; return create_token(TOKEN_DIV, 0, NULL, start_pos);
        case '^': lexer->position++; return create_token(TOKEN_POW, 0, NULL, start_pos);
        case '(': lexer->position++; return create_token(TOKEN_LPAREN, 0, NULL, start_pos);
        case ')': lexer->position++; return create_token(TOKEN_RPAREN, 0, NULL, start_pos);
        case ',': lexer->position++; return create_token(TOKEN_COMMA, 0, NULL, start_pos);
        case '!': lexer->position++; return create_token(TOKEN_FACT, 0, NULL, start_pos);
    }

    // Переменные и функции
    if (is_function_char(current)) {
        int len = 0;
        while (lexer->position + len < lexer->length && 
               is_function_char(lexer->input[lexer->position + len])) {
            len++;
        }
        
        char *name = strndup(lexer->input + lexer->position, len);
        lexer->position += len;
        
        // Проверка на константу PI
        if (strcmp(name, "PI") == 0) {
            free(name);
            return create_token(TOKEN_NUMBER, M_PI, NULL, start_pos);
        }
        
        // Проверка на функцию (если после имени идет '(')
        if (lexer->position < lexer->length && 
            lexer->input[lexer->position] == '(') {
            return create_token(TOKEN_FUNCTION, 0, name, start_pos);
        }
        
        return create_token(TOKEN_VARIABLE, 0, name, start_pos);
    }

    // Неизвестный символ
    lexer->position++;
    return create_token(TOKEN_ERROR, 0, NULL, start_pos);
}

void lexer_destroy(Lexer *lexer) {
    free(lexer);
}
