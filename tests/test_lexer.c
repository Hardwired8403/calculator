#include "lexer.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void test_numbers() {
    Lexer* l = lexer_create("3.14 42");
    Token t = lexer_next(l);
    assert(t.type == TOKEN_NUMBER && t.value == 3.14);
    
    t = lexer_next(l);
    assert(t.type == TOKEN_NUMBER && t.value == 42);
    
    lexer_destroy(l);
}

void test_operators() {
    const char* ops = "+-*/^()";
    Lexer* l = lexer_create(ops);
    
    TokenType expected[] = {
        TOKEN_PLUS, TOKEN_MINUS, TOKEN_MUL,
        TOKEN_DIV, TOKEN_POW, TOKEN_LPAREN,
        TOKEN_RPAREN
    };
    
    for (int i = 0; i < 7; i++) {
        Token t = lexer_next(l);
        assert(t.type == expected[i]);
    }
    
    lexer_destroy(l);
}

void test_functions() {
    Lexer* l = lexer_create("sin(x) PI");
    Token t = lexer_next(l);
    assert(t.type == TOKEN_FUNCTION && strcmp(t.name, "sin") == 0);
    
    t = lexer_next(l);
    assert(t.type == TOKEN_LPAREN);
    
    t = lexer_next(l);
    assert(t.type == TOKEN_VARIABLE && strcmp(t.name, "x") == 0);
    
    t = lexer_next(l);
    assert(t.type == TOKEN_RPAREN);
    
    t = lexer_next(l);
    assert(t.type == TOKEN_NUMBER && t.value == M_PI);
    
    lexer_destroy(l);
}

int main() {
    test_numbers();
    test_operators();
    test_functions();
    printf("Lexer tests passed!\n");
    return 0;
}
