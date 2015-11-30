#include <setjmp.h>

/* Yielding has two parts: setting the place to return to, and then jumping */
/* to the block. */
/* These are known as block_yield_outer and block_yield_inner */

#define ACME_BLOCK_INVOKE 1
#define ACME_BLOCK_NEXT 2
#define ACME_BLOCK_BREAK 3
#define ACME_BLOCK_RETURN 4

typedef struct _block_ret {
    thing t;
    acme_int ret_type;
} block_ret;

/* This is in the if statement, to setup the jmp_buf */
/* Modeled on setjmp */
extern block_ret block_caller_yield_outer(jmp_buf caller_env)
    __THROWNL;

/* This is used inside the "setjmp == 0" code of a yield to invoke the block */
/* Modeled on longjmp, but doesn't set sp */
extern void block_caller_yield_inner(jmp_buf block_env)
    __THROWNL __attribute__ ((__noreturn__));

/* When a block leaves, control returns to the block caller.  If the manner */
/* of leaving was anything other than a next, then the block caller will */
/* immediately leave via this call, passing back the return type.  
/* Modeled on longjmp */
extern void block_caller_leave(jmp_buf block_env, int ret_type)
    __THROWNL __attribute__ ((__noreturn__));

/* Used at the top of a block, in the function that defines the block */
/* This is exactly the same as setjmp, but returns an acme_int */
extern acme_int block_define(jmp_buf block_env)
    __THROWNL;
    
/* This is used to leave a block */
/* There are three ways to leave a block: next, break or return.  Falling */
/* out the bottom of a block is the same as next.  The third parameter here */
/* is one of the #defines above signaling which one happened */
/* Modeled on longjmp */
extern void block_leave(jmp_buf caller_env, thing t, int ret_type)
    __THROWNL __attribute__ ((__noreturn__));
