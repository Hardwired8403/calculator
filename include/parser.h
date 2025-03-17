#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"   // Для Token и Lexer
#include "errors.h"  // Для обработки ошибок

/**
 * @brief Преобразует поток токенов в обратную польскую нотацию (RPN)
 * 
 * @param lexer Указатель на инициализированный лексер
 * @param output Выходной массив токенов в формате RPN
 * @param output_len Указатель на переменную для записи длины выходного массива
 * @param error Указатель для возврата сообщения об ошибке (должен быть освобожден вызывающей стороной)
 */
void shunting_yard(
    Lexer* lexer,
    Token* output,
    int* output_len,
    char** error
);

#endif // PARSER_H
