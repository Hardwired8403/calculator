#include "evaluator.h"
#include "tokens.h"
#include "errors.h"
#include <math.h>
#include <stdlib.h>

double factorial(double n) {
    if (n < 0 || n != (int)n) return NAN;
    double res = 1;
    for (int i = 2; i <= n; i++) res *= i;
    return res;
}

double evaluate_rpn(Token *tokens, int token_count, VarTable *vars, char **error) {
    double stack[256];
    int stack_ptr = 0;
    
    for (int i = 0; i < token_count; i++) {
        Token token = tokens[i];
        switch (token.type) {
            case TOKEN_NUMBER:
                stack[stack_ptr++] = token.value;
                break;
                
            case TOKEN_VARIABLE: {
                int found;
                double value = var_table_get(vars, token.name, &found);
                if (!found) {
                    *error = error_create(token.position, "Undefined variable: %s", token.name);
                    return 0;
                }
                stack[stack_ptr++] = value;
                break;
            }
                
            case TOKEN_FUNCTION: {
                if (strcmp(token.name, "sin") == 0) {
                    if (stack_ptr < 1) {
                        *error = error_create(token.position, "Not enough arguments for sin()");
                        return 0;
                    }
                    double arg = stack[--stack_ptr];
                    stack[stack_ptr++] = sin(arg);
                } else if (strcmp(token.name, "cos") == 0) {
                    if (stack_ptr < 1) {
                        *error = error_create(token.position, "Not enough arguments for cos()");
                        return 0;
                    }
                    double arg = stack[--stack_ptr];
                    stack[stack_ptr++] = cos(arg);
                } else {
                    *error = error_create(token.position, "Unknown function: %s", token.name);
                    return 0;
                }
                break;
            }

            case TOKEN_FACT: {
                if (stack_ptr < 1) {
                    *error = error_create(token.position, "Not enough arguments for factorial");
                }

                stack[stack_ptr-1] = factorial(stack[stack_ptr-1]);
                break;
            }
                
            // Обработка операторов
            default: {
                if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS ||
                    token.type == TOKEN_MUL || token.type == TOKEN_DIV ||
                    token.type == TOKEN_POW) {
                    if (stack_ptr < 2) {
                        *error =  error_create(token.position, "Not enough operands for operator");
                        return 0;
                    }
                    double b = stack[--stack_ptr];
                    double a = stack[--stack_ptr];
                    switch (token.type) {
                        case TOKEN_PLUS: stack[stack_ptr++] = a + b; break;
                        case TOKEN_MINUS: stack[stack_ptr++] = a - b; break;
                        case TOKEN_MUL: stack[stack_ptr++] = a * b; break;
                        case TOKEN_DIV: 
                            if (b == 0) {
                                *error = error_create(token.position, "Division by zero");
                                return 0;
                            }
                            stack[stack_ptr++] = a / b; 
                            break;
                        case TOKEN_POW: stack[stack_ptr++] = pow(a, b); break;
                        default: break;
                    }
                }
                break;
            }
        }
    }
    
    if (stack_ptr != 1) {
        int err_pos = token_count > 0 ? tokens[token_count-1].position : 0;
        *error = error_create(err_pos, "Invalid expression structure");
        return 0;
    }
    
    return stack[0];
}
