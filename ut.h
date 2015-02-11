#ifndef UT_H
#define UT_H

#include "../uthash/src/uthash.h"

/* Make sure we're using the libgc version of malloc and free */
#undef uthash_malloc
#undef uthash_free
#define uthash_malloc(sz) GC_malloc(sz)
#define uthash_free(ptr,sz) GC_free(ptr)

#endif
