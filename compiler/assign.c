#include "compiler.h"

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch_r) {
    /* Get the op, that is, the operation before the = */
    char op[4];
    strncpy(op, assignop, 4);
    op[3] = '\0';
    /* remove the = */
    size_t l_op = strlen(op);
    if(l_op == 0 || op[l_op-1] != '=') {
        e_fatal("assign_lexpr called with op not ending in '=': %s\n", op);
    }
    op[--l_op] = '\0';
    /* local vars */
    code_hunk * self_ch = lh->self_ch;
    const char * name = lh->name;
    symbol sym = get_symbol(name);
    code_hunk * subscript_ch = lh->subscript_ch;
    /* The return value */
    code_hunk * ret;
    if(self_ch == NULL) {
        /* First case: If no explicit self, find symbol in the local variable table */
        code_hunk * offset_ch = get_local_var_ch(sym);
        if(offset_ch != NULL) {
            /* We have a local variable */
            code_hunk * ret = CCH3(CHS("{\nint offset = "), offset_ch, CHS(";\n"));
            if(subscript_ch == NULL) {
                /* Not an array, so assignment is pretty simple */
                /* Push the return value on the stack */
                ret = CCH2(ret, ch_r);
                if(l_op > 0) {
                    /* Perform the op if present */
                    ret = CCH5(ret, push_empty_block(), CHS("stack[offset]"), push_sym_thing(op), call_send(1));
                }
                /* Assign, and do not pop the stack! */
                ret = CCH2(ret, CHS("stack[offset] = stack[sp-1];\n"));
            }
            else {
                /* Local array variable */
                /* Push a NULL, the subscript, and the RHS onto the stack */
                ret = CCH4(ret, CHS("stack[sp++] = NULL;\n"), subscript_ch, ch_r);
                if(l_op > 0) {
                    /* Push an empty block on the stack */
                    ret = CCH2(ret, push_empty_block());
                    /* Duplicate the subscript on top of the stack, to use as param to the [] */                    
                    ret = CCH2(ret, CHS("stack[sp] = stack[sp-3]; sp++;\n"));
                    /* Now make the subscript call. We have the subscript, add the empty block, the local var, and the symbol */
                    ret = CCH5(ret, push_empty_block(), CHS("stack[sp++] = stack[offset];\n"), push_sym_thing("[]"), call_send(1));
                    /* The stack now has NULL, subscript, RHS, empty_block, and value. Add op and call */
                    ret = CCH3(ret, push_sym_thing(op), call_send(1));
                }
                /* At this point ret has added to the stack a NULL, the subscript, and the value to assign */
                /* Move the value to assign to where the NULL is */
                ret = CCH2(ret, CHS("stack[sp-3] = stack[sp-1];\n"));
                /* Duplicate the subscript */
                ret = CCH2(ret, CHS("stack[sp] = stack[sp-2]; sp++;\n"));
                /* Now, push empty block, the array and then the symbol */
                ret = CCH5(ret, push_empty_block(), CHS("stack[sp++] = stack[offset];\n"), push_sym_thing("[]="), call_send(2));
                /* Now the stack has the value to assign, the subscript, and the return from the array setter */
                /* Pop the return val and the extra subscript off the stack */
                ret = CCH2(ret, CHS("sp-=2; stack[sp] = stack[sp+1] = NULL;\n"));
            }
            /* Close the brace */
            ret = CCH2(ret, CHS("}\n"));
            return ret;
        }
        /* Didn't find it in the local vars, so the target is self */
        self_ch = push_self();
    }
    /* Not a local variable */
    /* Push a NULL onto the stack */
    ret = CCH2(ret, CHS("stack[sp++] = NULL;\n"));
    if(subscript_ch == NULL) {
        /* Not an array */
        /* Need to make the name= setter string */
        size_t l = strlen(name);
        char * setter_str = acme_malloc(l + 2);
        strcpy(setter_str, name);
        setter_str[l] = '=';
        setter_str[l+1] = '\0';
        /* Push self and rhs on stack */
        ret = CCH3(ret, self_ch, ch_r);
        if(l_op > 0) {
            /* push an empty block */
            ret = CCH2(ret, push_empty_block());
            /* Call the getter: push empty block, self, sym, then call */
            ret = CCH5(ret, push_empty_block(), CHS("stack[sp] = stack[sp-4]; sp++;\n"), push_sym_thing_from_symbol(sym), call_send(0));
            /* Stack now has NULL, self, RHS, empty block, and the current value.  Push the op and call. */
            ret = CCH3(ret, push_sym_thing(op), call_send(1));
        }
        /* At this point ret has added to the stack a NULL, self, and the value to assign */
        /* Move the value to assign to where the NULL is */
        ret = CCH2(ret, CHS("stack[sp-3] = stack[sp-1];\n"));
        /* Push empty block */
        ret = CCH2(ret, push_empty_block());
        /* Duplicate the self */
        ret = CCH2(ret, CHS("stack[sp] = stack[sp-3]; sp++;\n"));
        /* Now we have added value to assign, self, value to assign, empty block, self */
        /* Now do the assignment call */
        ret = CCH3(ret, push_sym_thing(setter_str), call_send(1));
        /* Stack now contains value to assign, self, return value from setter */
        /* Pop the stack twice: get rid of the extra self and return value from setter */
        ret = CCH2(ret, CHS("sp-=2; stack[sp] = stack[sp+1] = NULL;\n"));
    }
    else {
        /* Array */
        /* TODO: DRY this out.  Too much duplication of above */
        /* Call the getter to get the array object: push empty block, self, symbol, then call */
        ret = CCH5(ret, push_empty_block(), self_ch, push_sym_thing_from_symbol(sym), call_send(0));
        /* Stack now has NULL and the array. */
        /* Push the subscript and the RHS */
        ret = CCH3(ret, subscript_ch, ch_r);
        if(l_op > 0) {
            /* Push an empty block on the stack */
            ret = CCH2(ret, push_empty_block());
            /* Duplicate the subscript on top of the stack, to use as param to the [] */                    
            ret = CCH2(ret, CHS("stack[sp] = stack[sp-3]; sp++;\n"));
            /* Stack now has NULL, the array, subscript, RHS, empty_block, subscript */ 
            /* Now make the subscript call. We have the subscript, add the empty block, the local var, and the symbol */
            ret = CCH5(ret, push_empty_block(), CHS("stack[sp] = stack[sp-6]; sp++;\n"), push_sym_thing("[]"), call_send(1));
            /* The stack now has NULL, the array, subscript, RHS, empty_block, and value. Add op and call */
            ret = CCH3(ret, push_sym_thing(op), call_send(1));
        }
        /* stack has NULL, the array, subscript, and the value to assign */
        /* Move the value to assign to where the NULL is */
        ret = CCH2(ret, CHS("stack[sp-4] = stack[sp-1];\n"));
        /* Now, duplicate the subscript, add empty block, duplicate the array, add []= and call */
        ret = CCH6(ret, CHS("stack[sp] = stack[sp-2]; sp++;\n"), push_empty_block(), CHS("stack[sp] = stack[sp-4]; sp++;\n"), push_sym_thing("[]="), call_send(2));
        /* stack has value to assign, the array, subscript, and the return value */
        /* Pop the stack three times: get rid of the extra array and what []= returns */
        ret = CCH2(ret, CHS("sp-=3; stack[sp] = stack[sp+1] = stack[sp+2] = NULL;\n"));
    }
    return ret;
}

code_hunk * dereference(lexpr_hunk *lh){
}
