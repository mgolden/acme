#ifndef ACME_H
#define ACME_H

/* Usual C things */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Garbage collector */
#include <gc.h>

/* For defining hashes without bringing in
 * the particulars of the hash library */
#include "acme_hash.h"

/* Numeric types */
typedef int64_t acme_int;
typedef double acme_float;

/* A symbol is a 64 bit integer */
typedef int64_t symbol;

/* A function, operates on the stack, takes the sp as an argument */
typedef int (*acme_function)(int sp);

/* typedefs for structs that are circular */
typedef struct _thing thing;
typedef struct _thing_entry thing_entry;
typedef struct _box_list_entry box_list_entry;
typedef struct _array array;
typedef struct _hash_entry hash_entry;
typedef struct _box box;
typedef struct _box_list box_list;
typedef struct _box_list_entry box_list_entry;

ACME_HASH(thing_entry, symbol sym, thing *t);
ACME_HASH(hash_entry, thing *t1, thing *t);
ACME_HASH(function_entry, symbol sym, thing *t);
ACME_HASH(symbol_entry, symbol sym, int i);

enum thing_type {
  Normal_thing_type = 0,
  I_thing_type,
  F_thing_type,
  S_thing_type,
  Sym_thing_type,
  Nil_thing_type,
  array_thing_type,
  hash_thing_type
};

struct _thing {
  int16_t thing_type;
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
    array *a;
    /* a hash */
    hash_entry *h;
  } u;
  box_list * list;
};

struct _box_list {
  /* TODO: put some sort of speedup hash here */
  box_list_entry *first;
};

struct _box {
  symbol sym;
  function_entry *functions_hash;
  thing_entry *constants_hash;
  symbol_entry *symbols_hash;
};

struct _box_list_entry {
  box_list *next;
  box *b;
};

#define MAX_STACK_DEPTH 10000

extern thing *stack;
extern int sp;

#endif
