#include "parser.h"
#include "lexer.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Operator
{
    TokenType type;
    int precedence;
    int right_assoc;
} Operator;

static const Operator operators[] = {
    {TOKEN_FACT, 6, 1},  // Высший приоритет
    {TOKEN_POW, 4, 1},      // Правоассоциативный, приоритет 4
    {TOKEN_FUNCTION, 5, 0}, // Функции имеют высший приоритет
    {TOKEN_MUL, 3, 0},      // Умножение/деление: приоритет 3
    {TOKEN_DIV, 3, 0},
    {TOKEN_PLUS, 2, 0},     // Сложение/вычитание: приоритет 2
    {TOKEN_MINUS, 2, 0}
};

static int get_precedence(TokenType type)
{
    for (size_t i = 0; i < sizeof(operators) / sizeof(Operator); i++)
    {
        if (operators[i].type == type)
        {
            return operators[i].precedence;
        }
    }
    return 0;
}

void shunting_yard(Lexer *lexer, Token *output, int *output_len,
                          char **error)
{
    Token stack[256];
    int stack_ptr = 0;
    Token token;

    while ((token = lexer_next(lexer)).type != TOKEN_EOF)
    {
        switch (token.type)
        {
        // Обработка чисел и переменных
        case TOKEN_NUMBER:
        case TOKEN_VARIABLE:
            output[(*output_len)++] = token;
            break;

        // Обработка функций и скобок
        case TOKEN_FUNCTION:
            stack[stack_ptr++] = token;
            break;

        case TOKEN_COMMA:
            while (stack_ptr > 0 &&
                   stack[stack_ptr - 1].type != TOKEN_LPAREN)
            {
                output[(*output_len)++] = stack[--stack_ptr];
            }
            if (stack_ptr == 0)
            {
                *error = error_create(token.position, "Mismatched parentheses");
                error_destroy(*error);
                return;
            }
            break;

        case TOKEN_LPAREN:
            stack[stack_ptr++] = token;
            break;

        case TOKEN_RPAREN:
            while (stack_ptr > 0 &&
                   stack[stack_ptr - 1].type != TOKEN_LPAREN)
            {
                output[(*output_len)++] = stack[--stack_ptr];
            }
            if (stack_ptr == 0)
            {
                *error = error_create(token.position, "Mismatched parentheses");
                error_destroy(*error);
                return;
            }
            stack_ptr--; // Удаляем LPAREN
            if (stack_ptr > 0 &&
                stack[stack_ptr - 1].type == TOKEN_FUNCTION)
            {
                output[(*output_len)++] = stack[--stack_ptr];
            }
            break;

        case TOKEN_ERROR: {
            *error = token.name; 
            for (int j = 0; j < *output_len; j++) {
                if (output[j].type == TOKEN_VARIABLE || output[j].type == TOKEN_FUNCTION) {
                    free(output[j].name);
                }
            }
            return;
        }
        default: {
            // Находим параметры текущего оператора
            int current_prec = 0;
            int current_right_assoc = 0;
            int operator_found = 0;

            // Ищем оператор в таблице операторов
            for (size_t i = 0; i < sizeof(operators) / sizeof(Operator); i++)
            {
                if (operators[i].type == token.type)
                {
                    current_prec = operators[i].precedence;
                    current_right_assoc = operators[i].right_assoc;
                    operator_found = 1;
                    break;
                }
            }

            // Если оператор не найден - ошибка
            if (!operator_found)
            {
                *error = error_create(token.position, "Unknown operator");
                error_destroy(*error);
                return;
            }

            // Основная логика Shunting-yard для операторов
            while (stack_ptr > 0)
            {
                Token op = stack[stack_ptr - 1];
                int op_prec = get_precedence(op.type);

                // Условия переноса операторов в выходную очередь
                if ((current_right_assoc && current_prec < op_prec) ||
                    (!current_right_assoc && current_prec <= op_prec))
                {
                    output[(*output_len)++] = stack[--stack_ptr];
                }
                else
                {
                    break;
                }
            }

            // Помещаем текущий оператор в стек
            stack[stack_ptr++] = token;
            break;
          }
        }
    }

    while (stack_ptr > 0)
    {
        if (stack[stack_ptr - 1].type == TOKEN_LPAREN)
        {
            *error = error_create(token.position, "Mismatched parentheses");
            error_destroy(*error);
            return;
        }
        output[(*output_len)++] = stack[--stack_ptr];
    }
}
