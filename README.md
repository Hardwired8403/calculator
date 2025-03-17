# Калькулятор на C: Руководство по расширению

## Добавление новых унарных операций

### 1. Определение токена
Добавьте новый тип токена в `include/lexer.h`:
```c
typedef enum {
    ...
    TOKEN_UNARY_OP,  // Новый тип для унарных операций
    TOKEN_SQRT,      // Пример: квадратный корень
    ...
} TokenType;
```

### 2. Регистрация операции
В файле `src/tokens.c` добавьте операцию в таблицу:
```c
static struct {
    const char* name;
    int precedence;
    int is_unary;
} ops[] = {
    ...
    {"sqrt", 5, 1},  // 1 - флаг унарности
    {"neg", 4, 1},   // Пример унарного минуса
};
```

### 3. Обработка в лексере
Модифицируйте `src/lexer.c`:
```c
if (strcmp(name, "sqrt") == 0) {
    return create_token(TOKEN_SQRT, 0, NULL, start_pos);
}
```

### 4. Логика вычислений
Добавьте обработку в `src/evaluator.c`:
```c
case TOKEN_SQRT:
    if (stack_ptr < 1) {
        *error = error_create(token.position, "Missing operand for sqrt");
        return 0;
    }
    stack[stack_ptr-1] = sqrt(stack[stack_ptr-1]);
    break;
```
