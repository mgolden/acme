/******************************************************************************
 * qLibc
 *
 * Copyright (c) 2010-2014 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

/******************************************************************************
 * Just two functions taken from qLibc
 *****************************************************************************/

#include "fnv1.h"

/**
 * Get 32-bit FNV1 hash.
 *
 * @param data      source data
 * @param nbytes    size of data
 *
 * @return 32-bit unsigned hash value.
 *
 * @code
 *  uint32_t hashval = qhashfnv1_32((void*)"hello", 5);
 * @endcode
 *
 * @code
 *  Fowler/Noll/Vo hash
 *
 *  The basis of this hash algorithm was taken from an idea sent as reviewer
 *  comments to the IEEE POSIX P1003.2 committee by:
 *
 *      Phong Vo (http://www.research.att.com/info/kpv/)
 *      Glenn Fowler (http://www.research.att.com/~gsf/)
 *
 *  In a subsequent ballot round:
 *
 *      Landon Curt Noll (http://www.isthe.com/chongo/)
 *
 *  improved on their algorithm.  Some people tried this hash and found that
 *  it worked rather well. In an EMail message to Landon, they named it the
 *  ``Fowler/Noll/Vo'' or FNV hash.
 *
 *  FNV hashes are designed to be fast while maintaining a low collision rate.
 *  The FNV speed allows one to quickly hash lots of data while maintaining
 *  a reasonable collision rate.  See:
 *
 *      http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *
 *  for more details as well as other forms of the FNV hash.
 * @endcode
 */
uint32_t qhashfnv1_32(const void *data, size_t nbytes) {
    if (data == NULL || nbytes == 0)
        return 0;

    unsigned char *dp;
    uint32_t h = 0x811C9DC5;

    for (dp = (unsigned char *) data; *dp && nbytes > 0; dp++, nbytes--) {
#ifdef __GNUC__
        h += (h<<1) + (h<<4) + (h<<7) + (h<<8) + (h<<24);
#else
        h *= 0x01000193;
#endif
        h ^= *dp;
    }

    return h;
}


/**
 * Get 64-bit FNV1 hash integer.
 *
 * @param data      source data
 * @param nbytes    size of data
 *
 * @return 64-bit unsigned hash value.
 *
 * @code
 *   uint64_t fnv64 = qhashfnv1_64((void*)"hello", 5);
 * @endcode
 */
uint64_t qhashfnv1_64(const void *data, size_t nbytes) {
    if (data == NULL || nbytes == 0)
        return 0;

    unsigned char *dp;
    uint64_t h = 0xCBF29CE484222325ULL;

    for (dp = (unsigned char *) data; *dp && nbytes > 0; dp++, nbytes--) {
#ifdef __GNUC__
        h += (h << 1) + (h << 4) + (h << 5) +
        (h << 7) + (h << 8) + (h << 40);
#else
        h *= 0x100000001B3ULL;
#endif
        h ^= *dp;
    }

    return h;
}

