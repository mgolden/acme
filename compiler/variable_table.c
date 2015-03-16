#include "compiler.h"

typedef struct _variable_table_data {
  int fp_offset;
  int decl_line;
} variable_table_data;

DECLARE_ACME_HASH(variable_table_entry, symbol sym, variable_table_data vtd);

typedef struct _scope scope;

struct scope {
  scope *parent;
  int fresh;
  int current_top;
  variable_table_entry *variable_table;
}

static scope * scope_stack;

static scope *new_scope(void) {
  scope *s = GC_malloc(sizeof(scope));
  s->parent = NULL;
  s->fresh = 0;
  s->current_top = 0;
  s->variable_table = NEW_ACME_HASH(scope);
  return s;
}

/* Public functions */

/* Initialize the scope stack, make the outermost scope */
void init_scope(void) {
  scope_stack = new_scope();
  scope_stack->fresh = 1;
}

void push_scope(signature *sig) {
  s = new_scope();
  s->parent = scope_stack;
  scope_stack = s;
}

void fresh_scope(signature *sig) {
  push_scope(sig);
  scope_stack->fresh = 1;
}

void pop_scope(void) {
  scope * p = scope_stack->parent;
  if(p == NULL) {
    e_fatal("Attempt to pop bottom of scope stack")
  }
  ITERATE_ACME_HASH(scope_stack->variable_table, variable_table_entry, vte) {
    GC_free(vte);
  }
  GC_free(scope_stack);
  scope_stack = p;
}

void add_var(symbol sym) {
  variable_table_entry *vte;
  /* Check that symbol isn't already declared */
  FIND_BY_SYMBOL_ACME_HASH(scope_stack->variable_table, sym, vte);
  if(vte != NULL) {
    char error[200];
    sprintf(error, "Variable redeclared, originally declared on line %d", decl_line);
    e_warning(error);
    return;
  }
  NEW_ACME_HASH(variable_table_entry, vte);
  vte->sym = sym;
  vte->vtd.fp_offset = scope_stack->current_top++;
  vte->vtd.decl_line = yyget_lineno();
  ADD_BY_SYMBOL_ACME_HASH(symbol_table, vte);
}
