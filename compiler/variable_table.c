#include "compiler.h"

ACME_HASH(variable_table_entry, symbol sym, int offset);

typedef struct _scope scope;

struct scope {
  scope *parent;
  int fresh;
  variable_table_entry *variable_table;
}

static scope * scope_stack;

static scope *new_scope(void) {
  scope *s = GC_malloc(sizeof(scope));
  s->parent = NULL;
  s->fresh = 0;
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
  ACME_HASH_ITER(scope_stack->variable_table, variable_table_entry, vte) {
      GC_free(vte);
  }
  GC_free(scope_stack);
  scope_stack = p;
}
