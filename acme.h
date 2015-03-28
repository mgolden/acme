#ifndef ACME_H
#define ACME_H

/* Usual C things */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Garbage collector */
#include <gc.h>

#define acme_malloc GC_malloc
#define acme_free GC_free
#define acme_realloc GC_realloc
#define acme_strdup GC_strdup

/* For defining hashes without bringing in
 * the particulars of the hash library */
#include "acme_hash.h"

/* Numeric types */
typedef int64_t acme_int;
typedef double acme_float;

/* A symbol is a 64 bit integer */
typedef int64_t symbol;

/* A function, operates on the stack, takes the fp as an argument */
typedef int (*acme_fcn_ptr)(int fp);

/* typedefs for structs that are circular */
typedef struct _thing thing;
typedef struct _ability_list_entry ability_list_entry;
typedef struct _array array;
typedef struct _ability ability;
typedef struct _ability_list ability_list;
typedef struct _block_base block_base;
typedef union _stack_element stack_element;

DECLARE_ACME_HASH(thing_entry, symbol sym, thing *t);
DECLARE_ACME_HASH(hash_entry, thing *t1, thing *t);
DECLARE_ACME_HASH(function_entry, symbol sym, thing *t);
DECLARE_ACME_HASH(symbol_entry, symbol sym, int i);
DECLARE_ACME_HASH(symbol_definition, symbol sym, char *s);

struct _thing {
  union {
    /* Normal case, a hash table for the properties contained in the thing */
    /* Note - this must always be initialized to NULL */
    thing_entry *things_hash;
    /* All of the below are known as "atomic" things */
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
  ability_list *a_list;
};

struct _ability_list {
  /* TODO: put some sort of speedup hash here */
  ability_list_entry *first;
};

struct _ability {
  symbol sym;
  function_entry *functions_hash;
  thing_entry *constants_hash;
  symbol_entry *symbols_hash;
};

struct _ability_list_entry {
  ability_list *next;
  ability *b;
};

struct _block_base {
  acme_int block_fp;
  acme_fcn_ptr block_fcn_ptr;
};

union _stack_element {
  thing t;
  block_base b;
};

#define MAX_STACK_DEPTH 10000

#ifndef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn symbol_definition *symbol_table;

#endif
