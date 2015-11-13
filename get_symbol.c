#include "acme.h"

#include "get_symbol.h"

symbol get_symbol(const char *s) {
  symbol sym = (symbol) qhashfnv1_64((const void *) s, (size_t)strlen(s));
  if(sym == 0) {
    fprintf(stderr, "get_symbol: String \"%s\" hashed to 0!", s);
    exit(1);
  }
  symbol_definition *t;
  HASH_FIND_INT(symbol_table, &sym, t);
  if(t==NULL) {
    NEW_ACME_HASH(symbol_definition, sd);
    sd -> s = acme_strdup(s);
    ADD_BY_SYMBOL_ACME_HASH(symbol_table, sym, sd);
  }
  else {
    if(strcmp(t->s, s)!=0) {
      fprintf(stderr, "get_symbol: collision of different strings as same symbol: %s != %s\n", t->s, s);
      exit(1);
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
