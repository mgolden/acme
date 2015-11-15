#include "compiler.h"

code_hunk * emit_yield(int n_comexprs) {
  return
    CCH6(push_empty_block(), /* blocks don't have blocks passed to them */
      CHS("stack[sp++] = stack[fp-3];\nstack[sp++] = push_sym_thing((symbol) 0);\n"), /* self and (nonexistent) function symbol */
        push_i_thing(n_comexprs), /* number of params */
          CHS("{\n  block_base b = stack[fp-4].b;\n  int ret = (*(b.block_fcn_ptr))(b.block_fp);\n"),
            pop_stack(n_comexprs+4),
              CHS("  if(ret) {return(1);}\n}\n"));
}

code_hunk * emit_end_block(void) {
    return CHS("return(0);\n");
}
