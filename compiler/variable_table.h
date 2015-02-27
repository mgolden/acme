#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

void init_scope(void);
void push_scope(signature *sig);
void fresh_scope(signature *sig);
void pop_scope(void);

#endif
