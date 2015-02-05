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

static void * param(const char *s){printf("param(%s)\n",s); return((void *) s);}
static void * new_empty_signature(){printf("new_empty_signature\n"); return(NULL);}

static void new_string(const char *s){printf("new_string(%s)\n", s);}
static void add_var(const char *s){printf("add_var(%s)\n",s);}
static void get_reference(const char *s){printf("get_reference(%s)\n",s);}
static void get_member_reference(const char *s){printf("get_member_reference(%s)\n",s);}
static void operator_call(const char *s){printf("operator_call(%s)\n",s);}
static void unary_call(const char *s){printf("unary_call(%s)\n",s);}
static void start_box(const char *s){printf("start_box(%s)\n",s);}

static void push_nil(){printf("push_nil\n");}
static void assign_var(){printf("assign_var\n");}
static void pop_frame(){printf("pop_frame\n");}
static void push_frame(){printf("push_frame\n");}
static void pop_stack(){printf("pop_stack\n");}
static void emit_signature(){printf("emit_signature\n");}
static void dereference(){printf("dereference\n");}
static void end_box(){printf("end_box\n");}
static void start_if(){printf("start_if\n");}
static void start_else(){printf("start_else\n");}
static void start_elseif(){printf("start_elseif\n");}
static void end_if(){printf("end_if\n");}

static void do_function_call(int n){printf("do_function_call(%d)\n", n);}
static void new_int(int n){printf("new_int(%d)\n", n);}
static void new_array(int n){printf("new_array(%d)\n", n);}
static void new_hash(int n){printf("new_hash(%d)\n", n);}


static void signature_append(const void *x, const char * s){printf("signature_append(*,%s)\n", s);}

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

/* Numbers, strings */
%token <number> TOKNUMBER
%token <string> TOKDSTRING
%token <string> TOKSSTRING

/* Token */
%token <string> TOKWORD

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
  int number;
  char *string;
  void *ptr;
}

%type <ptr> signature
%type <ptr> param_list
%type <ptr> param

%type <string> unary
%type <string> add
%type <string> mul
%type <string> shift
%type <string> comparisonA
%type <string> comparisonB
%type <string> assignop
%type <string> assignboolop

%type <number> expr_list
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
    push_nil();
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
    push_nil();
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
  def_declaritor signature TOKEOL
  {
    push_frame();
    emit_signature($2); /* Also must free the signature */
  }
  ;
  
def_declaritor:
  TOKDEF TOKWORD
  ;

signature:
  {
    $$ = new_empty_signature();
  }
  | TOKLPAREN param_list TOKRPAREN
  {
    $$ = $2;
  }
  ;

param_list:
  param
  {
    $$ = new_empty_signature();
    signature_append($$, $1);
  }
  | param_list TOKCOMMA param
  {
    $$ = $1;
    signature_append($$, $3);
  }
  ;

param:
  TOKWORD
  {
    $$ = param($1);
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

val:
  TOKLPAREN expr TOKRPAREN
  {}
  | unary val
  {
    unary_call($1);
  }
  | array
  {}
  | hash
  {}
  | TOKNUMBER
  {
    new_int($1);
  }
  | TOKDSTRING
  {
    new_string($1);
  }
  | TOKSSTRING
  {
    new_string($1);
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
  lexpr arg_declaritor
  {
    do_function_call($2);
  }
  ;

array:
  TOKLBRACK TOKRBRACK
  {
    new_array(0);
  }
  | TOKLBRACK expr_list TOKRBRACK
  {
    new_array($2);
  }
  ;

expr_list:
  expr
  {
    $$ = 1;
  }
  |
  expr_list TOKCOMMA expr
  {
    $$ = $1 + 1;
  }
  ;

hash:
  TOKLBRACE TOKRBRACE
  {
    new_hash(0);
  }
  | TOKLBRACE hash_list TOKRBRACE
  {
    new_hash($2);
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
    add_var($2);
  }
  | TOKWORD
  {
    get_reference($1);
  }
  | val TOKDOT TOKWORD
  {
    get_member_reference($3);
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
