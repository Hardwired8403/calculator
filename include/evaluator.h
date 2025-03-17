#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "lexer.h"
#include "tokens.h"

/**
 * @brief Вычисляет выражение в обратной польской нотации (RPN)
 * 
 * @param tokens Массив токенов в формате RPN 
 * @param token_count Количество токенов
 * @param vars Таблица переменных
 * @param error Указатель для возврата сообщения об ошибке
 * @return double Результат вычисления или 0 при ошибке
 */
double evaluate_rpn(Token *tokens, 
                   int token_count, 
                   VarTable *vars, 
                   char **error);

#endif
