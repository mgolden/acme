#include "../acme.h"
#include "blocks.h"

int main(int argc, char **argv) {
    jmp_buf caller_env;
    block_ret br = block_caller_yield_outer(caller_env);
    printf("br.ret_type = %ld\n", br.ret_type);
    printf("br.t.u.i = %ld\n", br.t.u.i);
    printf("br.t.a_list = %p\n", br.t.a_list);
    return 0;
}
