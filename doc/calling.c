#include "../acme.h"

#include <setjmp.h>

typedef struct _block_ret {
    thing t;
    acme_int ret_type;
} block_ret;


block_ret baz(jmp_buf buf, thing t, acme_int i) {
    block_ret br;
    br.t = t;
    br.ret_type = i;
    return br;
}

block_ret foo(long long a, long long b, long long c, long long d, long long e, long long f, long long g, long long h, long long i) {
    block_ret br;
    br.t.u.i = 10;
    br.t.a_list = malloc(10);
    br.ret_type = 1;
    return br;
}

thing test(thing r) {
    block_ret br;
    br = foo(1,2,3,4,5,6,7,8,9);
    thing t;
    if(br.ret_type == 0) {
        t = br.t;
    }
    else {
        t = (thing){.a_list = NULL};
    }
    return(t);
}
