%{
#include "compiler.h"

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


static void *signature = NULL;
%}

%token TOKEOF  0  "end of file"

/* Keywords */
%token <s> TOKBOX
%token <s> TOKEND
%token <s> TOKDEF
%token <s> TOKVAR
%token <s> TOKPUBLIC
%token <s> TOKPRIVATE
%token <s> TOKSELF
%token <s> TOKIF
%token <s> TOKELSE
%token <s> TOKELSEIF
%token <s> TOKEMPTY
%token <s> TOKRETURN
%token <s> TOKTRY
%token <s> TOKCATCH
%token <s> TOKFINALLY
%token <s> TOKDO
%token <s> TOKBLOCKGIVEN
%token <s> TOKNIL
%token <s> TOKYIELD

/* Numbers, strings */
%token <i> TOKI
%token <f> TOKF
%token <s> TOKS

/* Token */
%token <s> TOKWORD
%token <s> TOKCONST

/* Token */
%token <s> TOKSYMBOL

/* EOL */
%token <s> TOKEOL

/* Operators */
%token <s> TOKDOT

%token <s> TOKPLUS
%token <s> TOKMINUS
%token <s> TOKBANG
%token <s> TOKTILDE

%token <s> TOKSTAR
%token <s> TOKSLASH
%token <s> TOKPERCENT

%token <s> TOKSHIFTL
%token <s> TOKSHIFTR

%token <s> TOKGT
%token <s> TOKLT
%token <s> TOKGE
%token <s> TOKLE

%token <s> TOKEQEQ
%token <s> TOKNEQ
%token <s> TOKCOMPARE

%token <s> TOKAMP

%token <s> TOKCARAT

%token <s> TOKPIPE

%token <s> TOKAND

%token <s> TOKOR

%token <s> TOKQUEST
%token <s> TOKCOLON

%token <s> TOKEQ
%token <s> TOKPLUSEQ
%token <s> TOKMINUSEQ
%token <s> TOKSTAREQ
%token <s> TOKSLASHEQ
%token <s> TOKPERCENTEQ
%token <s> TOKAMPEQ
%token <s> TOKCARATEQ
%token <s> TOKPIPEEQ
%token <s> TOKANDEQ
%token <s> TOKOREQ
%token <s> TOKSHIFTLEQ
%token <s> TOKSHIFTREQ

%token <s> TOKLBRACE
%token <s> TOKRBRACE
%token <s> TOKLPAREN
%token <s> TOKRPAREN
%token <s> TOKLBRACK
%token <s> TOKRBRACK

%token <s> TOKHASHROCK

%token <s> TOKCOMMA

%union 
{
  acme_int i;
  acme_float f;
  char *s;
  symbol sym;
  int number;
  void *ptr;
}

%type <ptr> param_with_default
%type <ptr> initializer_expression
%type <sym> symbol
%type <sym> word
%type <sym> function_name

%type <s> unary
%type <s> add
%type <s> mul
%type <s> shift
%type <s> comparisonA
%type <s> comparisonB
%type <s> assignop
%type <s> assignboolop

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
  | internal_statement_list internal_statement
  ;

internal_statement:
  blank_line
  | statement1 expr_statement
  ;
  
statement1:
  {
    pop_stack(); /* Pop the stack before every statement */
  }
  ;
  
external_statement_list:
  {
    push_stack(get_nil());
  }
  | external_statement_list external_statement
  ;

external_statement:
  internal_statement |
  statement1 def_statement |
  statement1 box_statement |
  statement1 ppc_statement
  ;

  
blank_line:
  TOKEOL
  ;

expr_statement:
  expr TOKEOL
  ;

word:
  TOKWORD
  {
    $$ = add_symbol($1);
  }
  ;

ppc_statement:
  pp_lexpr TOKEQ expr TOKEOL
  {
    assign_var();
  }
  | pp_lexpr TOKEOL
  {
    push_stack(get_nil());
    assign_var();
  }
  | TOKCONST TOKEQ expr TOKEOL
  {
    symbol sym = add_symbol($1);
    add_const(sym);
    push_stack(get_reference(sym));
    assign_var();
  }
  ;

pp_lexpr:
  TOKPRIVATE word
  {
    add_private($2);
    push_stack(get_reference($2));
  }
  | TOKPUBLIC word
  {
    add_public($2);
    push_stack(get_reference($2));
  }

  
def_statement:
  def_begin internal_statement_list TOKEND TOKEOL
  {
    pop_frame();
  }
  ;
  
def_begin:
  TOKDEF function_name start_signature signature TOKEOL
  {
    push_frame();
    emit_function($2, signature); /* Also must free the signature */
  }
  ;

function_name:
  word
  {
    $$ = $1;
  }
  | TOKWORD TOKEQ
  {
    /* Allowed, but not implemented yet */
    char *c = malloc(strlen($1)+1);
    strcpy(c, $1);
    strcat(c, "=");
    $$ = add_symbol(c);
  }
  | TOKLBRACK TOKRBRACK
  {
    $$ = add_symbol("[]");
  }
  | TOKLBRACK TOKRBRACK TOKEQ
  {
    $$ = add_symbol("[]=");
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
  | param_list TOKCOMMA TOKSTAR word
  {
    signature_append(signature, star_param($4));
  }
  | param_list TOKCOMMA param_with_default_list TOKCOMMA TOKSTAR word
  {
    signature_append(signature, star_param($6));
  }
  | param_with_default_list TOKCOMMA TOKSTAR word
  {
    signature_append(signature, star_param($4));
  }
  | TOKSTAR word
  {
    signature_append(signature, star_param($2));
  }
  ;
  
param_list:
  word
  {
    signature_append(signature, param($1));
  }
  | param_list TOKCOMMA word
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
  word TOKEQ initializer_expression
  {
    $$ = param_with_default($1, $3);
  }
  ;

initializer_expression:
  symbol
  {
    $$ = new_sym_thing($1);
  }
  | TOKI
  {
    $$ = new_i_thing($1);
  }
  | TOKF
  {
    $$ = new_f_thing($1);
  }
  | TOKS
  {
    $$ = new_s_thing($1);
  }
  | TOKNIL
  {
    $$ = get_nil();
  }
  | TOKLBRACK TOKRBRACK
  {
    $$ = new_array_thing(0);
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
  TOKEQEQ | TOKNEQ | TOKCOMPARE
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
  | TOKI
  {
    push_stack(new_i_thing($1));
  }
  | TOKF
  {
    push_stack(new_f_thing($1));
  }
  | TOKS
  {
    push_stack(new_s_thing($1));
  }
  | TOKBLOCKGIVEN
  {
    push_stack(block_given());
  }
  | symbol
  {
    push_stack(new_sym_thing($1));
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
  | pure_lexpr
  {
    dereference();
  }
  | array_lexpr
  {
    push_stack(new_sym_thing(add_symbol("[]")));
    do_function_call(1);
  }
  | TOKCONST
  {
    new_sym_thing(add_symbol($1));
    dereference();
  }
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
  var_lexpr TOKEQ expr
  {
    assign_var();
  }
  | pure_lexpr TOKEQ expr
  {
    assign_var();
  }
  | pure_lexpr assignop expr
  {
    assign_var();
  }
  | pure_lexpr assignboolop expr
  {
    assign_var();
  }
  |
  array_lexpr TOKEQ expr
  {
    assign_array_var();
  }
  | array_lexpr assignop expr
  {
    assign_array_var();
  }
  | array_lexpr assignboolop expr
  {
    assign_array_var();
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
  pure_lexpr arg_declaritor function_call1 optional_block
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
    push_stack(new_array_thing(0));
  }
  | TOKLBRACK expr_list TOKRBRACK
  {
    push_stack(new_array_thing($2));
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

pure_lexpr:
  word
  {
    push_stack(get_reference($1));
  }
  | val TOKDOT word
  {
    push_stack(get_member_reference($3));
  }
  ;

var_lexpr:
  TOKVAR word
  {
    add_var($2);
    push_stack(get_reference($2));
  }
  ;
  
array_lexpr:
  word TOKLBRACK expr TOKRBRACK
  {
    push_stack(get_reference($1));
  }
  ;
  
box_statement:
  box_begin external_statement_list TOKEND
  {
    end_box();
    pop_frame();
  }
  ;

box_begin:
  TOKBOX TOKCONST TOKEOL
  {
    push_frame();
    start_box(add_symbol($2));
  }
  ;
