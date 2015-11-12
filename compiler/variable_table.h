#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include "compiler.h"

void init_scope(void);
void push_scope(void);
void fresh_scope(void);
void pop_scope(void);

void add_var(const char *name);
code_hunk * get_local_var_ch(symbol sym);

#endif
