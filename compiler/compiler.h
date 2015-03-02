#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "code_hunk.h"
#include "acme_error.h"
#include "label.h"
#include "variable_table.h"

typedef struct _signature signature;

#ifdef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn symbol_definition *symbol_table;

void dump_function(char *function_name, code_hunk *c, FILE *f);

void compiler_init(void);

code_hunk * get_buck(void);
code_hunk * get_nil(void);
code_hunk * get_true(void);
code_hunk * get_false(void);
code_hunk * get_buck(void);

code_hunk * new_i_thing(acme_int n);
code_hunk * new_f_thing(acme_float f);
code_hunk * new_s_thing(const char *s);
code_hunk * new_sym_thing_from_sym(symbol sym);
code_hunk * new_sym_thing(const char *s);

code_hunk * new_array_thing(int i);
code_hunk * new_hash_thing(int i);

code_hunk * block_given(void);

code_hunk * start_if(void);
code_hunk * start_elseif(void);
code_hunk * start_else(void);
code_hunk * end_if(void);

code_hunk * call_send(int n);
code_hunk * clone(int n);

symbol get_lexpr_sym(const char * s);
code_hunk * new_lexpr_symbol_thing(const char * s);

code_hunk * param(symbol sym){printf("param(*)\n"); return(NULL);}
code_hunk * param_with_default(symbol sym, const char * p){printf("param(*, *)\n"); return(NULL);}
code_hunk * star_param(symbol sym){printf("param(*)\n"); return(NULL);}

static symbol add_symbol(const char *s){printf("add_symbol(%s)\n", s); return 0;}

static void add_public(symbol sym){printf("add_public(*)\n");}
static void add_sym(symbol sym){printf("add_private(*)\n");}
static void add_const(symbol sym){printf("add_const(*)\n");}
code_hunk * get_reference(symbol sym){printf("get_reference(*)\n"); return(NULL);}

static void assign_var(){printf("assign_var\n");}
static void assign_array_var(){printf("assign_array_var\n");}
static void dereference(){printf("dereference\n");}

static void do_function_call(acme_int n){printf("do_function_call(%d)\n", (int) n);}

static void emit_function(symbol sym, const void * p){printf("emit_function(*, *)\n");}
static void emit_pipe_param_list(const char *s){printf("emit_pipe_param_list(*)\n");}

#endif
