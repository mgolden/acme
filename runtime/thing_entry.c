#include "hash.h"

/* This adds a (sym, pointer to thing) pair into a symbol table. */
/* If an entry is already there, move the pointer to point at the new thing */
void create_thing_entry(thing_entry *thing_table, symbol sym, thing *t) {
  thing_entry *te;

  HASH_FIND_INT(thing_table, &sym, te);  /* id already in the hash? */
  if(te==NULL) {
    te = (thing_entry *)GC_malloc(sizeof(thing_entry));
    st->sym = sym;
    HASH_ADD_INT(thing_table, sym, st);  /* sym is the name of key field */
  }
  te->t = t;
}
