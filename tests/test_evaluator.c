#include "evaluator.h"
#include "tokens.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void test_basic_operations() {
    VarTable* vars = var_table_create();
    char* error = NULL;
    
    // 2 + 3*4 = 14
    Token expr[] = {
        {TOKEN_NUMBER, 2, NULL, 0},
        {TOKEN_NUMBER, 3, NULL, 0},
        {TOKEN_NUMBER, 4, NULL, 0},
        {TOKEN_MUL, 0, NULL, 0},
        {TOKEN_PLUS, 0, NULL, 0}
    };
    
    double result = evaluate_rpn(expr, 5, vars, &error);
    assert(error == NULL);
    assert(fabs(result - 14.0) < 1e-9);
    
    var_table_destroy(vars);
}

void test_functions() {
    VarTable* vars = var_table_create();
    char* error = NULL;
    
    // sin(PI/2) = 1
    Token expr[] = {
        {TOKEN_NUMBER, M_PI, NULL, 0},
        {TOKEN_NUMBER, 2, NULL, 0},
        {TOKEN_DIV, 0, NULL, 0},
        {TOKEN_FUNCTION, 0, "sin", 0},
    };
    
    double result = evaluate_rpn(expr, 4, vars, &error);
    assert(error == NULL);
    assert(fabs(result - 1.0) < 1e-9);
    
    var_table_destroy(vars);
}

void test_errors() {
    VarTable* vars = var_table_create();
    char* error = NULL;
    
    // Деление на ноль
    Token div_zero[] = {
        {TOKEN_NUMBER, 5, NULL, 0},
        {TOKEN_NUMBER, 0, NULL, 0},
        {TOKEN_DIV, 0, NULL, 0}
    };
    
    evaluate_rpn(div_zero, 3, vars, &error);
    assert(error != NULL);
    free(error);
    
    // Неизвестная переменная
    Token unknown_var[] = {
        {TOKEN_VARIABLE, 0, "x", 0}
    };
    
    evaluate_rpn(unknown_var, 1, vars, &error);
    assert(error != NULL);
    free(error);
    
    var_table_destroy(vars);
}

int main() {
    test_basic_operations();
    test_functions();
    test_errors();
    printf("Evaluator tests passed!\n");
    return 0;
}
