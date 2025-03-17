#include "calculator.h"
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "errors.h"

double evaluate_expression(const char *expr, VarTable *vars, char **error) {
    Lexer *lexer = lexer_create(expr);
    Token output[256];
    int output_len = 0;
    
    shunting_yard(lexer, output, &output_len, error);
    lexer_destroy(lexer);
    
    if (*error) return 0;
    
    return evaluate_rpn(output, output_len, vars, error);
}
