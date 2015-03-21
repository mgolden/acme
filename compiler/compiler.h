#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "misc.h"
#include "code_hunk.h"
#include "acme_error.h"
#include "label.h"
#include "variable_table.h"

typedef struct _signature signature;

#ifdef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn symbol_definition *symbol_table;

void compiler_init(void);

code_hunk * add_property(const char *name, int is_shy);
code_hunk * get_reference(code_hunk *ch);

code_hunk * assign_var(code_hunk *ch);
code_hunk * assign_array_var(code_hunk *ch);
code_hunk * dereference(code_hunk *ch);

code_hunk * clone(int n);

void start_signature(char *name, int is_shy);
code_hunk * param(symbol sym);
code_hunk * param_with_default(symbol sym, const char * p);
code_hunk * star_param(symbol sym);
void dump_function(code_hunk *ch, FILE *output_file);

#endif
