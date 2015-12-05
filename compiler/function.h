#ifndef BLOCK_H
#define BLOCK_H

code_hunk * emit_yield(code_hunk *yield_expr_list_ch);

code_hunk * emit_end_block(void);


code_hunk * add_property(const char *name, int is_shy);

void start_signature(char *name, int is_shy);
code_hunk * param(const char *s);
code_hunk * param_with_default(const char *s, code_hunk * p);
code_hunk * star_param(const char *s);

void dump_function(code_hunk *ch, FILE *output_file);


#endif
