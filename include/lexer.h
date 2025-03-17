#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POW,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_VARIABLE,
    TOKEN_FUNCTION,
    TOKEN_COMMA,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_FACT
} TokenType;

typedef struct {
    TokenType type;
    double value;      // Для чисел
    char *name;        // Для переменных и функций
    int position;      // Позиция в исходной строке
} Token;

typedef struct Lexer Lexer;

Lexer *lexer_create(const char *input);
Token lexer_next(Lexer *lexer);
void lexer_destroy(Lexer *lexer);

#endif
