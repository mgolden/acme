#include "compiler.h"

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch_r) {
    /* Get the op, that is, the operation before the = */
    char op[4];
    strncpy(op, assignop, 4);
    op[3] = '\0';
    /* remove the = */
    int l_op = strlen(op);
    if(l_op == 0 || op[l_op-1] != '=') {
        e_fatal("assign_lexpr called with op not ending in '=': %s\n", op);
    }
    op[--l_op] = '\0';
    /* local vars */
    code_hunk * self_ch = lh->self_ch;
    symbol sym = lh->sym;
    code_hunk * subscript_ch = lh->subscript_ch;
    /* The return value */
    code_hunk * ret;
    if(self_ch == NULL) {
        /* First case: If no explicit self, find symbol in the local variable table */
        code_hunk * offset_ch = get_local_var_ch(sym);
        if(offset_ch != NULL) {
            /* We have a local variable */
            code_hunk * ret = CCH3(CHS("{\nint offset = "), offset_ch, ";\n");
            if(subscript_ch == NULL) {
                /* Not an array, so assignment is pretty simple */
                if(l_op > 0) {
                    ret = CCH2(ret, emit_binop_call(CHS("stack[offset]"), op, ch_r));
                }
                else {
                    ret = CCH2(ret, ch_r);
                }
                /* Assign, and do not pop the stack! */
                ret = CCH2(ret, CHS("stack[offset] = stack[sp-1];\n"));
            }
            else {
                /* Local array variable */
                /* Push a NULL onto the stack */
                ret = CCH2(ret, "stack[sp++] = NULL;\n");
                /* Evaluate subscript onto stack */
                ret = CCH(ret, subscript_ch);
                if(l_op > 0) {
                    /* Duplicate the subscript on top of the stack, to use as param to the [] */                    
                    ret = CCH2(ret, CHS("stack[sp] = stack[sp-1]; sp++;\n");
                    /* Now make the call, we have the subscript, next the empty block, next the local var, next the symbol */
                    ret = CCH5(ret, get_empty_block(), CHS("stack[sp++] = stack[offset];\n", new_sym_thing("[]"), call_send(1));
                    /* Now the stack has the subscript and the value of the subscripted local var, emit the operator */
                    ret = emit_binop_call(ret, op, ch_r);
                }
                else {
                    ret = CCH2(ret, ch_r);
                }
                /* At this point ret has added to the stack a NULL, the subscript, and the value to assign */
                /* Move the value to assign to where the NULL is */
                ret = CCH2(ret, "stack[sp-3] = stack[sp-1];\n");
                /* Now, add to the stack the local "self" and then the symbol */
                ret = CCH5(ret, get_empty_block(), CHS("stack[sp++] = stack[offset];\n"), new_sym_thing("[]="), call_send(2));
                /* Pop the stack! Ignore what []= returns! (We've duplicated the value) */
                ret = CCH2(ret, CHS("sp--; stack[sp] = NULL;\n"));
            }
            /* Close the brace */
            ret = CCH2(ret, CHS("}\n"));
            return ret;
        }
        /* Didn't find it in the local vars, so the target is self */
        self_ch = get_self();
    }
    /* Not a local variable */
    /* Push a NULL onto the stack */
    ret = CCH2(ret, "stack[sp++] = NULL;\n");
    if(subscript_ch == NULL) {
        /* Not an array */
        /* Push the "self" on the stack */
        ret = CCH2(ret, self_ch);
        if(l_op > 0) {
            /* Push the empty block */
            ret = CCH2(ret, get_empty_block());
            /* Duplicate the self */
            ret = CCH2(ret, CHS("stack[sp] = stack[sp-2]; sp++;\n"));
            /* Stack now has NULL, self, empty_block, self.  Call getter */
            ret = CCH5(ret, new_sym_thing_from_symbol(symbol), call_send(1));
            /* emit operator */
            ret = emit_binop_call(ret, op, ch_r);
        }
        else {
            ret = CCH2(ret, ch_r);
        }
        /* At this point ret has added to the stack a NULL, self, and the value to assign */
        /* Move the value to assign to where the NULL is */
        ret = CCH2(ret, "stack[sp-3] = stack[sp-1];\n");
        /* Duplicate the self */
        ret = CCH2(ret, "stack[sp] = stack[sp-2]; sp++;\n");
        /* Now we have added value to assign, self, value to assign, self */
        /* Need to make the x= symbol */
        char * lexpr_str = get_string(symbol);
        int l = strlen(lexp_str);
        char * setter_str = acme_malloc(l + 2);
        strcpy(setter_str, lexpr_str);
        setter_str[l] = '=';
        setter_str[l+1] = '\0';
        /* Now do the assignment call */
        ret = CCH5(ret, get_empty_block(), new_sym_thing(setter_str), call_send(1));
        /* Stack now contains value to assign, self, return value from setter */
        /* Pop the stack twice: get rid of the extra self and return value from setter */
        ret = CCH2(ret, CHS("sp-=2; stack[sp] = stack[sp+1] = NULL;\n"));
    }
    else {
        /* Array */
        if(l_op > 0) {
            code_hunk * x = CCH4(subscript_ch, self_ch, get_empty_block(), new_sym_thing("[]"), call_send(2));
            ch_r2= emit_binop_call(x, op, ch_r);
        }
        else {
            ch_r2 = ch_r;
        }
        code_hunk * x = CCH4(self_ch, get_empty_block(), symbol, call_send(1));
        return CCH5(self_ch, get_empty_block()), new_sym_thing("[]=")), call_send(1)));
    
}

code_hunk * dereference(lexpr_hunk *lh){
}
