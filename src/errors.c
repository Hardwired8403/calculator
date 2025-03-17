#include "errors.h"

#define ERROR_BUF_SIZE 512

char* error_create(int position, const char* format, ...) {
    char buffer[ERROR_BUF_SIZE];
    va_list args;
    
    // Форматируем пользовательское сообщение
    va_start(args, format);
    int msg_len = vsnprintf(buffer, ERROR_BUF_SIZE, format, args);
    va_end(args);

    // Собираем финальное сообщение с позицией
    char* full_msg = malloc(ERROR_BUF_SIZE);
    if (!full_msg) return NULL;
    
    int total_len = snprintf(full_msg, ERROR_BUF_SIZE, 
        "[Error at position %d] %s", position, buffer);
    
    if (total_len >= ERROR_BUF_SIZE) {
        full_msg[ERROR_BUF_SIZE-1] = '\0';
    }
    
    return full_msg;
}

void error_destroy(char* error) {
    free(error);
}
