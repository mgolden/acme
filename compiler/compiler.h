#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "misc.h"
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
