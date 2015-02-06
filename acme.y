%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str)
{
  fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
  return 1;
}

main()
{
  yyparse();
}

static void * new_empty_signature(){printf("new_empty_signature\n"); return(NULL);}
static void * param(const char *s){printf("param(%s)\n", s); return(NULL);}
static void * param_with_default(const char *s, const char * p){printf("param(%s, *)\n", s); return(NULL);}
static void * star_param(const char *s){printf("param(%s, *)\n", s); return(NULL);}

static void * new_string(const char *s){printf("new_string(%s)\n", s);}
static void * add_symbol(const char *s){printf("add_symbol(%s)\n", s);}

static void * add_var(const char *s){printf("add_var(%s)\n", s); return(NULL);}
static void * get_reference(const char *s){printf("get_reference(%s)\n", s); return(NULL);}
static void * get_member_reference(const char *s){printf("get_member_reference(%s)\n", s); return(NULL);}

static void * new_int(long long n){printf("new_int(%d)\n", (int) n);}
static void * new_array(long long n){printf("new_array(%d)\n", (int) n);}
static void * new_hash(long long n){printf("new_hash(%d)\n", (int) n);}
static void * get_nil(){printf("get_nil\n"); return(NULL);}
static void * block_given(){printf("block_given\n"); return(NULL);}

static void operator_call(const char *s){printf("operator_call(%s)\n", s);}
static void unary_call(const char *s){printf("unary_call(%s)\n", s);}
static void start_box(const char *s){printf("start_box(%s)\n", s);}

static void push_stack(const void *p){printf("push_stack(*)\n");}

static void assign_var(){printf("assign_var\n");}
static void pop_frame(){printf("pop_frame\n");}
static void push_frame(){printf("push_frame\n");}
static void pop_stack(){printf("pop_stack\n");}
static void dereference(){printf("dereference\n");}
static void end_box(){printf("end_box\n");}
static void start_if(){printf("start_if\n");}
static void start_else(){printf("start_else\n");}
static void start_elseif(){printf("start_elseif\n");}
static void end_if(){printf("end_if\n");}

static void do_function_call(long long n){printf("do_function_call(%d)\n", (int) n);}

static void signature_append(const void * x, const char * y){printf("signature_append(*,*)\n");}
static void emit_function(const char * s, const void * p){printf("emit_function(%s, *)\n", s);}
static void emit_pipe_param_list(const char *s){printf("emit_pipe_param_list(*)\n");}

static void push_compile(){printf("push_compile()\n");}
static void * pop_compile(){printf("pop_compile()\n"); return(NULL);}

static void *signature = NULL;
%}

%token TOKEOF  0  "end of file"

/* Keywords */
%token <string> TOKBOX
%token <string> TOKEND
%token <string> TOKDEF
%token <string> TOKVAR
%token <string> TOKSELF
%token <string> TOKIF
%token <string> TOKELSE
%token <string> TOKELSEIF
%token <string> TOKEMPTY
%token <string> TOKRETURN
%token <string> TOKTRY
%token <string> TOKCATCH
%token <string> TOKDO
%token <string> TOKBLOCKGIVEN
%token <string> TOKNIL
%token <string> TOKYIELD

/* Numbers, strings */
%token <number> TOKNUMBER
%token <string> TOKSTRING

/* Token */
%token <string> TOKWORD

/* Token */
%token <string> TOKSYMBOL

/* EOL */
%token <string> TOKEOL

/* Operators */
%token <string> TOKDOT

%token <string> TOKPLUS
%token <string> TOKMINUS
%token <string> TOKBANG
%token <string> TOKTILDE

%token <string> TOKSTAR
%token <string> TOKSLASH
%token <string> TOKPERCENT

%token <string> TOKSHIFTL
%token <string> TOKSHIFTR

%token <string> TOKGT
%token <string> TOKLT
%token <string> TOKGE
%token <string> TOKLE

%token <string> TOKEQEQ
%token <string> TOKNEQ

%token <string> TOKAMP

%token <string> TOKCARAT

%token <string> TOKPIPE

%token <string> TOKAND

%token <string> TOKOR

%token <string> TOKQUEST
%token <string> TOKCOLON

%token <string> TOKEQ
%token <string> TOKPLUSEQ
%token <string> TOKMINUSEQ
%token <string> TOKSTAREQ
%token <string> TOKSLASHEQ
%token <string> TOKPERCENTEQ
%token <string> TOKAMPEQ
%token <string> TOKCARATEQ
%token <string> TOKPIPEEQ
%token <string> TOKANDEQ
%token <string> TOKOREQ
%token <string> TOKSHIFTLEQ
%token <string> TOKSHIFTREQ

%token <string> TOKLBRACE
%token <string> TOKRBRACE
%token <string> TOKLPAREN
%token <string> TOKRPAREN
%token <string> TOKLBRACK
%token <string> TOKRBRACK

%token <string> TOKHASHROCK

%token <string> TOKCOMMA

%union 
{
  long long number;
  char *string;
  void *ptr;
}

%type <ptr> param_with_default
%type <ptr> initializer_expression
%type <ptr> symbol

%type <string> unary
%type <string> add
%type <string> mul
%type <string> shift
%type <string> comparisonA
%type <string> comparisonB
%type <string> assignop
%type <string> assignboolop

%type <number> expr_list
%type <number> pure_expr_list
%type <number> hash_list
%type <number> arg_declaritor

%start start

%%

start:
  external_statement_list TOKEOF
  {}
  ;

internal_statement_list:
  {
    push_stack(get_nil());
  }
  | internal_statement_list popped_internal_statement
  ;

popped_internal_statement:
  blank_line
  | popped_statement1 expr_statement
  ;
  
popped_statement1:
  {
    pop_stack(); /* Pop the stack before every statement */
  }
  ;
  
external_statement_list:
  {
    push_stack(get_nil());
  }
  | external_statement_list popped_external_statement
  ;

popped_external_statement:
  popped_internal_statement | popped_statement1 def_statement
  ;

  
blank_line:
  TOKEOL
  ;

expr_statement:
  expr TOKEOL
  ;

def_statement:
  def_begin internal_statement_list TOKEND
  {
    pop_frame();
  }
  ;
  
def_begin:
  TOKDEF TOKWORD start_signature signature TOKEOL
  {
    push_frame();
    emit_function($2, signature); /* Also must free the signature */
  }
  ;

start_signature:
  {
    signature = new_empty_signature();
  }
  ;
  
signature:
  | TOKLPAREN TOKRPAREN
  | TOKLPAREN non_empty_signature TOKRPAREN
  ;

/* There must be a better way to do this */
non_empty_signature:
  param_list
  {}
  | param_list TOKCOMMA param_with_default_list
  {}
  | param_with_default_list
  {}
  | param_list TOKCOMMA TOKSTAR TOKWORD
  {
    signature_append(signature, star_param($4));
  }
  | param_list TOKCOMMA param_with_default_list TOKCOMMA TOKSTAR TOKWORD
  {
    signature_append(signature, star_param($6));
  }
  | param_with_default_list TOKCOMMA TOKSTAR TOKWORD
  {
    signature_append(signature, star_param($4));
  }
  | TOKSTAR TOKWORD
  {
    signature_append(signature, star_param($2));
  }
  ;
  
param_list:
  TOKWORD
  {
    signature_append(signature, param($1));
  }
  | param_list TOKCOMMA TOKWORD
  {
    signature_append(signature, param($3));
  }
  ;

param_with_default_list:
  param_with_default
  {
    signature_append(signature, $1);
  }
  | param_with_default_list TOKCOMMA param_with_default
  {
    signature_append(signature, $3);
  }
  ;

param_with_default:
  TOKWORD TOKEQ initializer_expression
  {
    $$ = param_with_default($1, $3);
  }
  ;

initializer_expression:
  symbol
  {
    $$ = $1;
  }
  | TOKNUMBER
  {
    $$ = new_int($1);
  }
  | TOKSTRING
  {
    $$ = new_string($1);
  }
  | TOKNIL
  {
    $$ = get_nil();
  }
  | TOKLBRACK TOKRBRACK
  {
    $$ = new_array(0);
  }
  | TOKLBRACE TOKRBRACE
  {
    $$ = new_hash(0);
  }
  ;

unary:
  TOKPLUS | TOKMINUS | TOKBANG | TOKTILDE  
  {
    $$ = $1;
  }
  ;

add:
  TOKPLUS | TOKMINUS
  {
    $$ = $1;
  }
  ;

mul:
  TOKSTAR | TOKSLASH | TOKPERCENT
  {
    $$ = $1;
  }
  ;
  
shift:
  TOKSHIFTL | TOKSHIFTR
  {
    $$ = $1;
  }
  ;

comparisonA:
  TOKGT | TOKLT | TOKGE | TOKLE
  {
    $$ = $1;
  }
  ;

comparisonB:
  TOKEQEQ | TOKNEQ
  {
    $$ = $1;
  }
  ;

assignop:
  TOKPLUSEQ | TOKMINUSEQ | TOKSTAREQ | TOKSLASHEQ | TOKPERCENTEQ | TOKAMPEQ | TOKCARATEQ | TOKPIPEEQ | TOKSHIFTLEQ | TOKSHIFTREQ
  {
    $$ = $1;
  }
  ;

assignboolop:
  TOKANDEQ | TOKOREQ
  {
  $$ = $1;
  }
  ;

symbol:
  TOKSYMBOL
  {
    $$ = add_symbol($1);
  }
  ;

val:
  TOKLPAREN expr TOKRPAREN
  {}
  | TOKNUMBER
  {
    push_stack(new_int($1));
  }
  | TOKSTRING
  {
    push_stack(new_string($1));
  }
  | TOKBLOCKGIVEN
  {
    push_stack(block_given());
  }
  | symbol
  {
    push_stack($1);
  }
  | array
  {}
  | hash
  {}
  | unary val
  {
    unary_call($1);
  }
  | function_call
  {}
  | lexpr
  {
    dereference();
  }
  | box_expression
  {}
  ;

factor:
  val
  {}
  | factor mul val
  {
    operator_call($2);
  }
  ;

term:
  factor
  {}
  | term add factor
  {
    operator_call($2);
  }
  ;

exprI:
  term
  {}
  | exprI shift term
  {
    operator_call($2);
  }
  ;

exprH:
  exprI
  {}
  | exprH comparisonA exprI
  {
    operator_call($2);
  }
  ;
  
exprG:
  exprH
  {}
  | exprG comparisonB exprH
  {
    operator_call($2);
  }
  ;

exprF:
  exprG
  {}
  | exprF TOKAMP exprG
  {
    operator_call($2);
  }
  ;

exprE:
  exprF
  {}
  | exprE TOKCARAT exprF
  {
    operator_call($2);
  }
  ;
  
exprD:
  exprE
  {}
  | exprD TOKPIPE exprE
  {
    operator_call($2);
  }
  ;
  
exprC:
  exprD
  {}
  | exprC TOKAND exprD
  {
    operator_call($2);
  }
  ;

exprB:
  exprC
  {}
  | exprB TOKOR exprC
  {
    operator_call($2);
  }
  ;

exprA:
  exprB
  {}
  | exprB exprA1 TOKQUEST exprA exprA2 TOKCOLON exprA
  {
    end_if();
  }
  ;
  
exprA1:
  {
    start_if();
  }
  ;
  
exprA2:
  {
    start_else();
  }
  ;
  
nonifexpr:
  lexpr TOKEQ expr
  {
    assign_var();
  }
  | lexpr assignop expr
  {
    assign_var();
  }
  | lexpr assignboolop expr
  {
    assign_var();
  }
  | exprA
  {}
  ;

ifexpr:
  TOKIF nonifexpr TOKEOL ifexpr1 internal_statement_list elseif_clause else_clause TOKEND
  {
    end_if();
  }
  ;

ifexpr1:
  {
    start_if();
  }
  ;
  
elseif_clause:
  | elseif_clause TOKELSEIF nonifexpr elseif_clause1 internal_statement_list
  ;

elseif_clause1:
  {
    start_elseif();
  }
  ;

else_clause:
  |
  TOKELSE else_clause1 internal_statement_list
  ;

else_clause1:
  {
    start_else();
  }
  ;

expr:
  nonifexpr
  | ifexpr
  ;

function_call:
  lexpr arg_declaritor function_call1 optional_block
  {
    do_function_call($2);
  }
  ;

function_call1:
  {
    signature = new_empty_signature();
  }
  ;

optional_block:
  {
    push_stack(get_nil());
  }
  | TOKDO optional_pipe_param_list TOKEOL optional_block1 internal_statement_list TOKEND
  {
    push_stack(pop_compile());
  }
  ;

optional_block1:
  {
    push_compile();
  }
  ;

optional_pipe_param_list:
  {
    emit_pipe_param_list(signature);
  }
  | TOKPIPE param_list TOKPIPE
  {
    emit_pipe_param_list(signature);
  }
  ;

array:
  TOKLBRACK TOKRBRACK
  {
    push_stack(new_array(0));
  }
  | TOKLBRACK expr_list TOKRBRACK
  {
    push_stack(new_array($2));
  }
  ;

expr_list:
  pure_expr_list
  {
    $$ = $1;
  }
  | pure_expr_list TOKCOMMA hash_list
  {
    push_stack(new_hash($3));
    $$ = $1 + 1;
  }
  | hash_list
  {
    push_stack(new_hash($1));
    $$ = 1;
  }
  ;

pure_expr_list:
  expr
  {
    $$ = 1;
  }
  |
  pure_expr_list TOKCOMMA expr
  {
    $$ = $1 + 1;
  }
  ;

hash:
  TOKLBRACE TOKRBRACE
  {
    push_stack(new_hash(0));
  }
  | TOKLBRACE hash_list TOKRBRACE
  {
    push_stack(new_hash($2));
  }
  ;

hash_list:
  hash_element
  {
    $$ = 1;
  }
  | hash_list TOKCOMMA hash_element
  {
    $$ = $1 + 2;
  }
  ;

hash_element:
  expr TOKHASHROCK expr
  ;

arg_declaritor:
  TOKLPAREN TOKRPAREN
  {
    $$ = 0;
  }
  | TOKLPAREN expr_list TOKRPAREN
  {
    $$ = $2;
  }
  ;

lexpr:
  TOKVAR TOKWORD
  {
    push_stack(add_var($2));
  }
  | TOKWORD
  {
    push_stack(get_reference($1));
  }
  | val TOKDOT TOKWORD
  {
    push_stack(get_member_reference($3));
  }
  ;

box_expression:
  box_begin external_statement_list TOKEND
  {
    end_box();
    pop_frame();
  }
  ;

box_begin:
  box_declaritor box_arg TOKEOL
  ;

box_declaritor:
  TOKBOX
  {
    push_frame();
  }
  ;
  
box_arg:
  {
    start_box("");
  }
  | TOKLPAREN TOKWORD TOKRPAREN
  {
    start_box($2);
  }
  ;
