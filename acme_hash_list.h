/* This file insulates us from particularities of uthash and utlist */
/* It's abandonware at this point, and we might want to remove it at some point */

#ifndef ACME_HASH_H
#define ACME_HASH_H

#include "uthash/src/uthash.h"

/* Make sure we're using the libgc version of malloc and free */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) GC_malloc(sz)
#define uthash_free(ptr,sz) GC_free(ptr)

#define ACME_HASH(name, keytype, key, valtype, val) \
typedef struct _ ## name { \
  keytype key; \
  valtype *val; \
  UT_hash_handle hh; \
} name;

#endif
