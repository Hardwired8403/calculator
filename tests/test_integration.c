#include "calculator.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void test_full_calculation() {
    VarTable* vars = var_table_create();
    var_table_set(vars, "x", 2.0);
    char* error = NULL;
    
    // (x^3 + sin(PI/2)) / 3 = (8 + 1)/3 = 3
    double result = evaluate_expression("(x^3 + sin(PI/2))/3", vars, &error);
    assert(error == NULL);
    assert(fabs(result - 3.0) < 1e-9);
    
    var_table_destroy(vars);
}

void test_error_handling() {
    VarTable* vars = var_table_create();
    char* error = NULL;
    
    evaluate_expression("2 + + 3", vars, &error);
    assert(error != NULL);
    free(error);
    error = NULL; 
    
    evaluate_expression("invalid_func(5)", vars, &error);
    assert(error != NULL);
    free(error);
    
    var_table_destroy(vars);
}

int main() {
    test_full_calculation();
    test_error_handling();
    printf("Integration tests passed!\n");
    return 0;
}
