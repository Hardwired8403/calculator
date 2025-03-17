#ifndef CALCULATOR_H
#define CALCULATOR_H

typedef struct VarTable VarTable;

double evaluate_expression(const char *expr, VarTable *vars, char **error);
VarTable *var_table_create();
void var_table_set(VarTable *vars, const char *name, double value);
void var_table_destroy(VarTable *vars);

#endif
