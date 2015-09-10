#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

void init_scope(void);
void push_scope(signature *sig);
void fresh_scope(signature *sig);
void pop_scope(void);

void add_var(const char *name);
int get_var_fp_offset(symbol sym);

#endif
