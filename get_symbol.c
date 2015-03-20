#include "acme.h"

#include "get_sym.h"

symbol get_sym(const char *s) {
  symbol sym = (symbol) qhashfnv1_64((const void *) s, (size_t)strlen(s));
  symbol_definition *t;
  HASH_FIND_INT(symbol_table, &sym, t);
  if(t==NULL) {
    NEW_ACME_HASH(symbol_definition, sd);
    sd -> sym = sym;
    sd -> s = acme_strdup(s);
    ADD_BY_SYMBOL_ACME_HASH(symbol_table, sd);
  }
  else {
    if(strcmp(t->s, s)!=0) {
      sprintf(stderr, "collision: %s != %s\n", t->s, s);
      e_fatal("collision of different strings as same symbol");
    }
    /* else same string inserted again, all ok */
  }
  return sym;
}

symbol get_no_eq_sym(symbol sym) {
  symbol_definition *t;
  HASH_FIND_INT(symbol_table, &sym, t);
  if(t==NULL) {
    e_fatal("attempt to get no_eq_sym for unknown sym");
  }
  char * s = acme_strdup(t->s);
  int l = strlen(s);
  if(l<2) {
    sprintf(stderr, "symbol: %s\n", s);
    e_fatal("attempt to get no_eq_sym for too-short string");
  }
  if(s[l-1] != '=') {
    sprintf(stderr, "symbol: %s\n", s);
    e_fatal("attempt to get no_eq_sym for a sym that doesn't end in '='");
  }
  s[l-1] = '\0';
  symbol sym_no_eq = get_sym(s);
  acme_free(s);
  return(sym_no_eq);
}
