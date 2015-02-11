#ifndef ACME_H
#define ACME_H

#include <stdint.h>

#include <gc.h>

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

#endif
