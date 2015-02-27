/* This file insulates us from particularities of uthash and utlist
 * It's abandonware at this point, and we might want to remove it at
 * some point */

#ifndef ACME_HASH_H
#define ACME_HASH_H

#include "uthash/src/uthash.h"

/* Make sure we're using the libgc version of malloc and free */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) GC_malloc(sz)
#define uthash_free(ptr,sz) GC_free(ptr)

#define ACME_HASH(typename, keydecl, valdecl) \
typedef struct _ ## typename { \
  keydecl; \
  valdecl; \
  UT_hash_handle hh; \
} typename;
/* NB: UT_hash_handle is internal, used only by uthash;
 * makes this structure hashable */

#define NEW_ACME_HASH(typename, v) \
  { \
    (typename *) v = (typename *) GC_malloc(sizeof(typename)); \
    v->hh = NULL; \
  }

#endif

#define ACME_HASH_ITER(hashname, membertype, var) \
  ;membertype *var, *tmp; \
  HASH_ITER(hh, hashname, var, tmp)
