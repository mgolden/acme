#include "variable_table.h"

ACME_HASH(variable_table_entry, symbol sym, int offset);

typedef struct _scope scope;

struct scope {
  scope *parent;
  int fresh;
  variable_table_entry *variable_table;
}

static scope * scope_stack;

/* Initialize the scope stack, make the outermost scope */
void init_scope(void) {
  scope_stack = GC_malloc(sizeof(scope));
  scope_stack->parent = NULL;
  scope_stack->fresh = 1;
  scope_stack->variable_table = NEW_ACME_HASH(scope);
}

