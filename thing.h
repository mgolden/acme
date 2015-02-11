#include "acme_types.h"
#include "hash.h"

enum thing_type {
  Normal_thing_type = 0,
  I_thing_type,
  F_thing_type,
  S_thing_type,
  Sym_thing_type,
  array_thing_type,
  hash_thing_type
};

struct _thing {
  int_16t thing_type;
  union {
    /* Normal case, a hash table for the public and private properties contained in the thing */
    /* Note - this must always be initialized to NULL */
    thing_entry *things_hash;
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
    /* an array */
    thing **a;
    /* a hash */
    hash *h;
  } u;
  box * eigenbox;
  thing_list * boxlist;
};

struct _box {
  symbol sym;
  function_entry *functions_hash;
  thing_entry *constants_hash;
  symbol_entry *symbols_hash;
};


  