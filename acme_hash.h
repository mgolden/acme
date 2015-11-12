/* This file insulates us from particularities of uthash and utlist
 * It's abandonware at this point, and we might want to remove it at
 * some point */

#ifndef ACME_HASH_H
#define ACME_HASH_H

#include "uthash/src/uthash.h"

/* Make sure we're using the libgc version of malloc and free */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) acme_malloc(sz)
#define uthash_free(ptr,sz) acme_free(ptr)

/* Wrap all the uthash internals so they aren't visible from our code */

/* Note: here the proper use in the code is "DECLARE_ACME_HASH(...);" *
 * We are leaving out the ; here to avoid pedantic errors since an *
 * extra ; is not allowed outside of a function */
#define DECLARE_ACME_HASH(typename, keydecl, valdecl) \
typedef struct _ ## typename { \
  keydecl; \
  valdecl; \
  UT_hash_handle hh; \
} typename
/* NB: UT_hash_handle is internal, used only by uthash;
 * makes this structure hashable */

/* Create a new, unfilled ACME_HASH entry */
#define NEW_ACME_HASH(typename, v) \
  ;typename * v = (typename *) acme_malloc(sizeof(typename));

/* Iterate over an ACME_HASH */
#define ITERATE_ACME_HASH(hashname, membertype, var) \
  ;membertype *var, *tmp; \
  HASH_ITER(hh, hashname, var, tmp)

/* Find an entry in an ACME_HASH keyed by a symbol */
#define FIND_BY_SYMBOL_ACME_HASH(the_hash, sym, var) \
  { \
    symbol __sym = (sym); \
    HASH_FIND_INT(the_hash, &__sym, (var)); \
  }

/* Add to an ACME_HASH an entry keyed by a symbol */
/* Note: must be unique.  If a collision, it's an error */
/* Note: key field must be a symbol named sym */
/* Note: the variable the_hash is side effected by this */
#define ADD_BY_SYMBOL_ACME_HASH(the_hash, entry) \
  HASH_ADD_INT(the_hash, sym, entry);

#endif
