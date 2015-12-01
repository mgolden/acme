#include "../acme.h"

#include <setjmp.h>

struct _caller_env {
    __jmp_buf env;
    thing t;
};
typedef struct _caller_env caller_env[1];

typedef __jmp_buf block_env;

int64_t baz(caller_env c_env, thing t, int64_t i) {
    (*c_env).t.u.i = 7;
    (*c_env).t.a_list = malloc(20);
    return i;
}

int64_t foo(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, int64_t g, int64_t h, int64_t i) {
    int64_t j;
    caller_env c_env;
    thing t;
    t.u.i = 6;
    t.a_list = malloc(10);
    j = baz(c_env, t, j);
    return j;
}

thing test(thing r) {
    int64_t k;
    k = foo(1,2,3,4,5,6,7,8,9);
    thing t;
    t.u.i = k;
    return(t);
}
