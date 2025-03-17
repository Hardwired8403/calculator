#ifndef TOKENS_H
#define TOKENS_H

/**
 * @brief Непрозрачная структура для хранения таблицы переменных
 */
typedef struct VarTable VarTable;

/**
 * @brief Создает новую таблицу переменных
 */
VarTable* var_table_create(void);

/**
 * @brief Уничтожает таблицу переменных и освобождает память
 */
void var_table_destroy(VarTable* table);

/**
 * @brief Устанавливает значение переменной в таблице
 * 
 * @param table Указатель на таблицу переменных
 * @param name Имя переменной
 * @param value Значение переменной
 */
void var_table_set(VarTable* table, const char* name, double value);

/**
 * @brief Получает значение переменной из таблицы
 * 
 * @param table Указатель на таблицу переменных
 * @param name Имя переменной
 * @param found Флаг наличия переменной (1 - найдена, 0 - не найдена)
 * @return double Значение переменной или 0 если не найдена
 */
double var_table_get(const VarTable* table, const char* name, int* found);

#endif // TOKENS_H
