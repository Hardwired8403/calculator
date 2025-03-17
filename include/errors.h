#ifndef ERRORS_H
#define ERRORS_H

#include <stdarg.h>  // Для работы с variadic arguments
#include <stdio.h>   // Для vsnprintf
#include <stdlib.h>  // Для malloc/free

/**
 * @brief Создает форматированное сообщение об ошибке с позицией
 * 
 * @param position Позиция в исходной строке (от 0)
 * @param format Строка формата printf-style
 * @param ... Аргументы для формата
 * @return char* Динамически выделенная строка с ошибкой (требует free)
 */
char* error_create(int position, const char* format, ...);

/**
 * @brief Освобождает ресурсы сообщения об ошибке
 * 
 * @param error Указатель на строку ошибки
 */
void error_destroy(char* error);

#endif // ERRORS_H
