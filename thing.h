#include "acme_types.h"
#include "hash.h"

enum thing_type {
  Normal_thing_type = 0,
  I_thing_type,
  F_thing_type,
  S_thing_type,
  Sym_thing_type,
  Ref_thing_type
};

struct _thing {
  int_16t thing_type;
  union {
    /* Normal case, a hash table for the vars contained in the thing */
    /* Note - this must always be initialized to NULL */
    hash_entry *hash_table;
    /* All of the below are known as "atomic" things */
    /* They cannot have an eigenclass */
    /* an integer */
    acme_int i;
    /* a float */
    acme_float d;
    /* a string */
    char *s;
    /* a symbol */
    symbol sym;
    /* used only internally, a non-deferenced pointer to something */
    thing *t;
  } u;
  thing * eigenbox;
  list * boxlist;
};
