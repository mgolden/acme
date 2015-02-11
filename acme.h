#ifndef ACME_H
#define ACME_H

/* Usual C things */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Garbage collector */
#include <gc.h>

/* For defining hashes without bringing in */
/* the particulars of the hash library *
#include "acme_hash.h"

/* Various typedefs for structs that may be circular */
typedef struct _thing thing;
typedef struct _thing_entry thing_entry;

/* Numeric types */
typedef int64_t acme_int;
typedef double acme_float;

/* A symbol is a 64 bit integer */
typedef int64_t symbol;

/* A function, operates on the stack, takes no arguments */

typedef int (*acme_function)(void);

struct _thing_entry {
  /* key */
  symbol sym;
  /* The data held */
  thing *t;
  /* Internal, used only by uthash; makes this structure hashable */


#endif
