#include "compiler.h"

code_hunk * emit_yield(code_hunk *yield_expr_list_ch) {
    return CCH(
        CHS("{\n"),
        yield_expr_list_ch,
        CHS("int ret_type;\nif((ret_type = block_caller_yield(c_env, b_env)) != ACME_BLOCK_NEXT) {\nblock_caller_leave(b_env, ret_type);\n}\n_acme_ = (*c_env).t;\n}\n"
        );
}

code_hunk * emit_end_block(void) {
    return CHS("return(0);\n");
}
