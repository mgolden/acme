#include "hash.h"

/* This adds a (sym, pointer to thing) pair into a symbol table. */
/* If an entry is already there, move the pointer to point at the new thing */
void create_symbol_table_entry(symbol_table_entry *symbol_table, symbol sym, thing *t) {
  symbol_table_entry *ste;

  HASH_FIND_INT(symbol_table, &sym, ste);  /* id already in the hash? */
  if(ste==NULL) {
    ste = (symbol_table_entry *)GC_malloc(sizeof(symbol_table_entry));
    ste->sym = sym;
    HASH_ADD_INT(symbol_table, sym, ste);  /* sym is the name of key field */
  }
  ste->t = t;
}
