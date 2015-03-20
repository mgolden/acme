#ifndef FNV1_H
#define FNV1_H

#include <stdlib.h>
#include <stdint.h>

uint32_t qhashfnv1_32(const void *data, size_t nbytes);
uint64_t qhashfnv1_64(const void * data, size_t nbytes);

#endif
