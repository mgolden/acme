#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "code_hunk.h"

void dump_function(char *function_name, code_hunk *c, FILE *f);

static void * new_empty_signature(){printf("new_empty_signature\n"); return(NULL);}
static void * param(symbol sym){printf("param(*)\n"); return(NULL);}
static void * param_with_default(symbol sym, const char * p){printf("param(*, *)\n"); return(NULL);}
static void * star_param(symbol sym){printf("param(*)\n"); return(NULL);}

static void * new_s_thing(const char *s){printf("new_s_thing(%s)\n", s);}
static symbol add_symbol(const char *s){printf("add_symbol(%s)\n", s); return 0;}

static void add_var(symbol sym){printf("add_var(*)\n");}
static void add_public(symbol sym){printf("add_public(*)\n");}
static void add_private(symbol sym){printf("add_private(*)\n");}
static void add_const(symbol sym){printf("add_const(*)\n");}
static void * get_reference(symbol sym){printf("get_reference(*)\n"); return(NULL);}
static void * get_member_reference(symbol sym){printf("get_member_reference(*)\n"); return(NULL);}

static void * new_i_thing(acme_int n){printf("new_i_thing(%d)\n", (int) n);}
static void * new_f_thing(acme_float f){printf("new_f_thing(%lf)\n", (double) f);}
static void * new_sym_thing(symbol sym){printf("new_sym_thing(*)\n");}
static void * new_array_thing(int n){printf("new_array_thing(%d)\n", n);}
static void * new_hash_thing(int n){printf("new_hash(%d)\n", n);}
static void * get_nil(){printf("get_nil\n"); return(NULL);}
static void * block_given(){printf("block_given\n"); return(NULL);}

static void start_box(symbol sym){printf("start_box(*)\n");}

static void push_stack(const void *p){printf("push_stack(*)\n");}

static void assign_var(){printf("assign_var\n");}
static void assign_array_var(){printf("assign_array_var\n");}
static void pop_frame(){printf("pop_frame\n");}
static void push_frame(){printf("push_frame\n");}
static void pop_stack(){printf("pop_stack\n");}
static void dereference(){printf("dereference\n");}
static void end_box(){printf("end_box\n");}
static void start_if(){printf("start_if\n");}
static void start_else(){printf("start_else\n");}
static void start_elseif(){printf("start_elseif\n");}
static void end_if(){printf("end_if\n");}

static void do_function_call(acme_int n){printf("do_function_call(%d)\n", (int) n);}

static void signature_append(const void * x, const char * y){printf("signature_append(*,*)\n");}
static void emit_function(symbol sym, const void * p){printf("emit_function(*, *)\n");}
static void emit_pipe_param_list(const char *s){printf("emit_pipe_param_list(*)\n");}

static void push_compile(){printf("push_compile()\n");}
static void * pop_compile(){printf("pop_compile()\n"); return(NULL);}


#endif
