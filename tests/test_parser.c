#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <string.h>

void test_shunting_yard() {
    const char* expr = "3 + 4 * 2 / (1 - 5)^2";
    Lexer* l = lexer_create(expr);
    Token output[32];
    int output_len = 0;
    char* error = NULL;
    
    shunting_yard(l, output, &output_len, &error);
    assert(error == NULL);
    
    // Правильная последовательность RPN:
    // 3 4 2 * 1 5 - 2 ^ / +
    TokenType expected[] = {
        TOKEN_NUMBER,  // 3
        TOKEN_NUMBER,  // 4
        TOKEN_NUMBER,  // 2
        TOKEN_MUL,     // *
        TOKEN_NUMBER,  // 1
        TOKEN_NUMBER,  // 5
        TOKEN_MINUS,   // -
        TOKEN_NUMBER,  // 2
        TOKEN_POW,     // ^
        TOKEN_DIV,     // /
        TOKEN_PLUS     // +
    };
    
    assert(output_len == 11);
    for (int i = 0; i < 11; i++) {
        printf("Token %d: type=%d\n", i, output[i].type);
        assert(output[i].type == expected[i]);
    }
    
    lexer_destroy(l);
}

void test_parentheses() {
    const char* expr = "sin(max(2,3))";
    Lexer* l = lexer_create(expr);
    Token output[32];
    int output_len = 0;
    char* error = NULL;
    
    shunting_yard(l, output, &output_len, &error);
    assert(error == NULL);
    
    assert(output[0].type == TOKEN_NUMBER && output[0].value == 2);
    assert(output[1].type == TOKEN_NUMBER && output[1].value == 3);
    assert(output[2].type == TOKEN_FUNCTION && strcmp(output[2].name, "max") == 0);
    assert(output[3].type == TOKEN_FUNCTION && strcmp(output[3].name, "sin") == 0);
    
    lexer_destroy(l);
}

int main() {
    test_shunting_yard();
    test_parentheses();
    printf("Parser tests passed!\n");
    return 0;
}
