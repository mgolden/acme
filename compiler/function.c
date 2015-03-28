#include "compiler.h"

code_hunk * emit_yield(int n_comexprs) {
  return
    CCH(CCH(CCH(CCH(CCH(get_empty_block(), /* blocks don't have blocks passed to them */
      CH(acme_strdup("stack[sp++] = stack[fp-3];\nstack[sp++] = (symbol) 0;\n"))), /* self and (nonexistent) function symbol */
        new_i_thing(n_comexprs)), /* number of params */
          CH(acme_strdup("{\n  block_base *b = stack[fp-4].b;\n  int ret = (*b->block_fcn_ptr)(*b->block_fp);\n"))),
            pop_stack(n_comexprs+4)),
              CH(acme_strdup("if(ret) {return(1);}\n}\n")));
}
