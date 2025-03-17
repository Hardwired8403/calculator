#include "tokens.h"
#include <string.h>
#include <stdlib.h>

typedef struct Symbol {
    char *name;
    double value;
    struct Symbol *next;
} Symbol;

struct VarTable {
    Symbol *head;
};

VarTable *var_table_create() {
    VarTable *table = malloc(sizeof(VarTable));
    table->head = NULL;
    return table;
}

void var_table_set(VarTable *table, const char *name, double value) {
    Symbol *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    
    Symbol *new_symbol = malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->value = value;
    new_symbol->next = table->head;
    table->head = new_symbol;
}

double var_table_get(const VarTable *table, const char *name, int *found) {
    Symbol *current = table->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            *found = 1;
            return current->value;
        }
        current = current->next;
    }
    *found = 0;
    return 0;
}

void var_table_destroy(VarTable *table) {
    Symbol *current = table->head;
    while (current) {
        Symbol *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    free(table);
}
