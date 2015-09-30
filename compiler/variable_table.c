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
  scope *s = acme_malloc(sizeof(scope));
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
    acme_free(vte);
  }
  acme_free(scope_stack);
  scope_stack = p;
}

void add_var(const char *name) {
  variable_table_entry *vte;
  symbol sym = get_symbol(name);
  /* Check that symbol isn't already declared */
  FIND_BY_SYMBOL_ACME_HASH(scope_stack->variable_table, sym, vte);
  if(vte != NULL) {
    e_warning(error, "Variable redeclared, originally declared on line %d", decl_line);
    return;
  }
  NEW_ACME_HASH(variable_table_entry, vte);
  vte->sym = sym;
  vte->vtd.fp_offset = scope_stack->current_top++;
  vte->vtd.decl_line = yyget_lineno();
  ADD_BY_SYMBOL_ACME_HASH(symbol_table, vte);
}

/* If the symbol for the local variable is found in the stack, make a code hunk */
/* that finds it. Note that this can be used to assign into, when stack is */
/* subscripted with it */
/* e.g. fp+7 or stack[stack[fp]]+7 */
code_hunk * get_local_var_ch(symbol sym) {
  variable_table_entry *vte;
  int i = 0;
  scope * ss = scope_stack;
  while(ss != NULL) {
    FIND_BY_SYMBOL_ACME_HASH(scope_stack->variable_table, sym, vte);
    if(vte != NULL) {
      code_hunk *ret = CHS("fp");
      for(int j = 0; j > i; j++) {
        ret = CCH(CCH(CHS("stack[stack["), ret), CHS("]]"));
      }
      char off[50];
      sprintf(off, "%d", vte -> fp_offset);
      ret = CCH(CCH(CCH(CCH(ret, CHS("+")), CHS(off)))
      return ret;
    }
    i++;
  }
  return NULL;
}
