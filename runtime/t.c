#include "../acme.h"
#include "blocks.h"

thing baz(void);

thing print(thing t) {
    printf("%ld\n", t.u.i);
    return(t);
}


int main(int argc, char **argv) {
    volatile thing t = baz();
    print((thing) t);
    return 0;
}


thing new_i_thing(acme_int i) {
    volatile thing t = {.u.i = i, .a_list = NULL};
    return (thing) t;
}


thing bar(thing x, thing y, block_env b_env, caller_env c_env, thing * p1, thing * p2, thing * p3) {
    volatile thing _acme_;
    volatile thing _acme_z;
    // _acme_ = _acme_z = call_send(1, call_send(1, new_i_thing(10), new_sym_thing("+"), x, NULL), new_sym_thing("-"), y, NULL);
    volatile thing _acme_l;
    {
        /* This is the yield statement itself */
        (*p3) = new_i_thing(4);
        (*p2) = new_i_thing(2);
        (*p1) = (thing) _acme_z;
        switch(block_caller_yield(c_env, b_env)) {
            case ACME_BLOCK_BREAK:
                block_caller_leave(b_env, ACME_BLOCK_BREAK);
                break;
            case ACME_BLOCK_RETURN:
                block_caller_leave(b_env, ACME_BLOCK_RETURN);
                break;
        }
        /* ACME_BLOCK_NEXT */
        _acme_l = (*c_env).t;
    }
    volatile thing _acme_m;
    _acme_ = _acme_m = new_i_thing(20);
    return (thing) _acme_;
}


thing baz(void) {
    printf("baz\n");
    volatile thing _acme_;
    volatile thing _acme_r;
    _acme_ = _acme_r = new_i_thing(10);
    volatile thing _acme_s;
    _acme_ = _acme_s = new_i_thing(20);
    {
        volatile thing _acme_v;
        volatile thing _acme_w;
        volatile thing _acme_z;
        caller_env c_env;
        block_env b_env;
        switch(block_define(b_env)) {
            case 0:
                // _acme_ = call_send(0, get_self(), new_sym_thing("bar"), r, s, block_env, caller_env, &v, &w, &z);
                printf("about to call bar()\n");
                _acme_ = bar(_acme_r, _acme_s, b_env, c_env, (thing *) &_acme_v, (thing *) &_acme_w, (thing *) &_acme_z);
                break;
            case ACME_BLOCK_INVOKE:
                printf("inside block\n");
                /* Caller env will be magically set by the time we get here */
                _acme_ = print((thing) _acme_z);
                // _acme_ - call_send(1, _acme_v, new_sym_thing("=="), ...);
                if(_acme_.u.i == 4) {
                    _acme_ = new_i_thing(5);
                    printf("about to block_return\n");
                    block_leave(c_env, _acme_, ACME_BLOCK_RETURN);
                }
                _acme_ = new_i_thing(7);
                printf("about to block_leave\n");
                block_leave(c_env, _acme_, ACME_BLOCK_NEXT);
            case ACME_BLOCK_RETURN:
                print(_acme_);
                printf("block return\n");
                return _acme_;
        }
        /* if i == ACME_BLOCK_BREAK, just fall through */
    }
    thing _acme_t;
    _acme_t = new_i_thing(100);
    printf("bottom return\n");
    return _acme_;
}
