#ifndef ACME_TYPES_H
#define ACME_TYPES_H

#include <stdint.h>

/* Various typedefs for structs that may be circular */
typedef struct _thing thing;
typedef struct _symbol_table_entry symbol_table_entry;

/* Numeric types */
typedef int64_t acme_int;
typedef double acme_float;

/* A symbol is a 64 bit integer */
typedef int64_t symbol;

#endif
