#include "acme.h"

#include "get_symbol.h"

symbol get_symbol(const char *s) {
  symbol sym = (symbol) qhashfnv1_64((const void *) s, (size_t)strlen(s));
  if(sym == 0) {
    sprintf(stderr, "String \"%s\" hashed to 0!", s);
    e_fatal("string hashed to 0");
  }
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


const char * get_string(symbol sym) {
    symbol_definition *t;
    HASH_FIND_INT(symbol_table, &sym, t);
    if(t==NULL) {
        return NULL;
    }
    return t->s;
}
