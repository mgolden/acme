#include <setjmp.h>
#include "../acme.h"

/* Yielding has two parts: setting the place to return to, and then jumping */
/* to the block. */
/* These are known as block_yield_outer and block_yield_inner */

#define ACME_BLOCK_INVOKE 1
#define ACME_BLOCK_NEXT 2
#define ACME_BLOCK_BREAK 3
#define ACME_BLOCK_RETURN 4

/* Note that here we using __jmp_buf rather than jmp_buf. */
/* This is specific to gcc.  It leaves out the signal masking stuff */
/* which is pretty large and in this context unnecessary */
struct _caller_env {
    __jmp_buf env;
    thing t;
};
typedef struct _caller_env caller_env[1];

typedef __jmp_buf block_env;

/* This is in the if statement, to setup the jmp_buf */
/* Modeled on setjmp */
extern int64_t block_caller_yield_outer(caller_env c_env)
    __THROWNL;

/* This is used inside the "setjmp == 0" code of a yield to invoke the block */
/* Modeled on longjmp, but doesn't set sp */
extern void block_caller_yield_inner(block_env b_env)
    __THROWNL __attribute__ ((__noreturn__));

/* When a block leaves, control returns to the block caller.  If the manner */
/* of leaving was anything other than a next, then the block caller will */
/* immediately leave via this call, passing back the return type.  
/* Modeled on longjmp */
extern void block_caller_leave(block_env b_env, int64_t ret_type)
    __THROWNL __attribute__ ((__noreturn__));

/* Used at the top of a block, in the function that defines the block */
/* This is exactly the same as setjmp, but returns an int64_t */
extern int64_t block_define(block_env b_env)
    __THROWNL;
    
/* This is used to leave a block */
/* There are three ways to leave a block: next, break or return.  Falling */
/* out the bottom of a block is the same as next.  The third parameter here */
/* is one of the #defines above signaling which one happened */
/* Modeled on longjmp */
extern void block_leave(caller_env c_env, thing t, int64_t ret_type)
    __THROWNL __attribute__ ((__noreturn__));
