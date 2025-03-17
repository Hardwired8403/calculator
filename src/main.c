#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"
#include "errors.h"  // Подключаем файл с обработкой ошибок

void print_help(const char* prog_name) {
    printf("Usage: %s \"expression\" [options]\n", prog_name);
    printf("Options:\n");
    printf("  --var name=value    Set variable value (e.g., --var x=3.14)\n");
    printf("Example:\n");
    printf("  %s \"2^sin(PI/x)\" --var x=4\n", prog_name);
}

int parse_variable(const char* arg, char** name, double* value) {
    char* eq_pos = strchr(arg, '=');
    if (!eq_pos || eq_pos == arg) return 0;
    
    *name = strndup(arg, eq_pos - arg);
    *value = strtod(eq_pos + 1, NULL);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char* error = error_create(0, "Not enough arguments.");
        fprintf(stderr, "Error: %s\n", error);
        error_destroy(error);  // Освобождаем память
        print_help(argv[0]);
        return 1;
    }

    VarTable* vars = var_table_create();
    char* error = NULL;
    const char* expression = argv[1];

    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--var") == 0) {
            if (i+1 >= argc) {
                error = error_create(0, "Missing value for --var");
                fprintf(stderr, "Error: %s\n", error);
                error_destroy(error);  // Освобождаем память
                var_table_destroy(vars);
                return 1;
            }
            
            char* name;
            double value;
            if (!parse_variable(argv[++i], &name, &value)) {
                error = error_create(0, "Invalid variable format '%s'", argv[i]);
                fprintf(stderr, "Error: %s\n", error);
                error_destroy(error);  // Освобождаем память
                var_table_destroy(vars);
                return 1;
            }
            
            var_table_set(vars, name, value);
            free(name);
        } else {
            error = error_create(0, "Unknown option '%s'", argv[i]);
            fprintf(stderr, "Error: %s\n", error);
            error_destroy(error);  // Освобождаем память
            var_table_destroy(vars);
            return 1;
        }
    }

    // Evaluate expression
    double result = evaluate_expression(expression, vars, &error);

    // Handle results
    if (error) {
        fprintf(stderr, "Error: %s\n", error);
    } else {
        printf("Result: %.6f\n", result);
    }

    var_table_destroy(vars);
    return 0;
}
