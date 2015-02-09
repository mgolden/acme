#ifndef HASH_H
#define HASH_H

#include <gc.h>

#include "../acme_types.h"

#include "../uthash/src/uthash.h"

/* Make sure we're using the libgc version of malloc and free */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) GC_malloc(sz)
#define uthash_free(ptr,sz) GC_free(ptr)

struct _symbol_table_entry {
  /* key */
  symbol sym;
  /* The data held */
  thing *t;
  /* Internal, used only by uthash; makes this structure hashable */
  UT_hash_handle hh;
};

#endif
