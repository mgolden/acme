#ifndef MISC_H
#define MISC_H

#include "compiler.h"

#include "code_hunk.h"

code_hunk * push_stack(int n);
code_hunk * pop_stack(int n);

code_hunk * get_buck(void);
code_hunk * get_nil(void);
code_hunk * get_true(void);
code_hunk * get_false(void);

code_hunk * get_self(void);

code_hunk * new_i_thing(acme_int i);
code_hunk * new_f_thing(acme_float f);
code_hunk * new_s_thing(const char *s);
code_hunk * new_sym_thing_from_symbol(symbol sym);
code_hunk * new_sym_thing(const char *s);
code_hunk * new_array_thing(int i);
code_hunk * new_hash_thing(int i);

code_hunk * emit_unop_call(const char *op, code_hunk *e);
code_hunk * emit_binop_call(code_hunk *e1, const char *op, code_hunk *e2);

code_hunk * get_empty_block(void);
code_hunk * block_given(void);

code_hunk * start_if(void);
code_hunk * start_elseif(void);
code_hunk * start_else(void);
code_hunk * end_if(void);

code_hunk * call_send(int i);

code_hunk * clone(int i);

char * make_block_name(const char * file_name, int n);

#endif
