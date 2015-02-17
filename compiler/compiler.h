#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "code_hunk.h"
#include "acme_error.h"
#include "label.h"

typedef struct _signature signature;

void dump_function(char *function_name, code_hunk *c, FILE *f);

void compiler_init(void);

code_hunk * pop_stack(int n);

code_hunk * new_i_thing(acme_int n);
code_hunk * new_f_thing(acme_float f);
code_hunk * new_s_thing(const char *s);
code_hunk * new_sym_thing_from_sym(symbol sym);
code_hunk * new_sym_thing(const char *s);

signature * new_empty_signature(){printf("new_empty_signature\n"); return(NULL);}

code_hunk * param(symbol sym){printf("param(*)\n"); return(NULL);}
code_hunk * param_with_default(symbol sym, const char * p){printf("param(*, *)\n"); return(NULL);}
code_hunk * star_param(symbol sym){printf("param(*)\n"); return(NULL);}

code_hunk * new_s_thing(const char *s){printf("new_s_thing(%s)\n", s);}
static symbol add_symbol(const char *s){printf("add_symbol(%s)\n", s); return 0;}

static void add_var(symbol sym){printf("add_var(*)\n");}
static void add_public(symbol sym){printf("add_public(*)\n");}
static void add_private(symbol sym){printf("add_private(*)\n");}
static void add_const(symbol sym){printf("add_const(*)\n");}
code_hunk * get_reference(symbol sym){printf("get_reference(*)\n"); return(NULL);}
code_hunk * get_member_reference(symbol sym){printf("get_member_reference(*)\n"); return(NULL);}

code_hunk * new_array_thing(int n){printf("new_array_thing(%d)\n", n);}
code_hunk * new_hash_thing(int n){printf("new_hash(%d)\n", n);}
code_hunk * get_nil(){printf("get_nil\n"); return(NULL);}
code_hunk * block_given(){printf("block_given\n"); return(NULL);}

static void start_box(symbol sym){printf("start_box(*)\n");}

static void push_stack(const void *p){printf("push_stack(*)\n");}

static void assign_var(){printf("assign_var\n");}
static void assign_array_var(){printf("assign_array_var\n");}
static void dereference(){printf("dereference\n");}
static void start_if(){printf("start_if\n");}
static void start_else(){printf("start_else\n");}
static void start_elseif(){printf("start_elseif\n");}
static void end_if(){printf("end_if\n");}

static void do_function_call(acme_int n){printf("do_function_call(%d)\n", (int) n);}

static void signature_append(const void * x, const char * y){printf("signature_append(*,*)\n");}
static void emit_function(symbol sym, const void * p){printf("emit_function(*, *)\n");}
static void emit_pipe_param_list(const char *s){printf("emit_pipe_param_list(*)\n");}

static void push_compile(){printf("push_compile()\n");}
code_hunk * pop_compile(){printf("pop_compile()\n"); return(NULL);}


#endif
