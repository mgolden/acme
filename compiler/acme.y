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

static char *input_file_name;
static FILE *output_file;

int main(int argc, char **argv)
{
  if(argc != 1) usage();
  char *input_file_name = argv[1];
  char *output_file_name = strdup(input_file_name);
  char *p = strrchr(file_name, ".");
  if(strcmp(p,".ac")!=0) usage();
  *p++ = 'c';
  *p = '\0';
  output_file = fopen(output_file_name, "w");
  GC_free(output_file_name);
  yyin = fopen(input_file_name, "r");
  yyparse();
  fclose(yyin);
}

static void usage(void) {
  fprintf(stderr, "usage: acme source.ac\n");
  exit(1);
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
%token <s> TOKBEGIN

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
%token <s> TOKBUCK

%union 
{
  acme_int i;
  acme_float f;
  char *s;
  symbol sym;
  code_hunk *code_hunk;
}

%type <sym> symbol
%type <sym> word
%type <sym> const
%type <sym> const_or_word
%type <sym> function_name

%type <sym> unary
%type <sym> add
%type <sym> mul
%type <sym> shift
%type <sym> comparisonA
%type <sym> comparisonB
%type <sym> assignop
%type <sym> assignboolop

%type <code_hunk> internal_statement_list
%type <code_hunk> internal_statement
%type <code_hunk> statement1
%type <code_hunk> external_statement_list
%type <code_hunk> external_statement
%type <code_hunk> blank_line
%type <code_hunk> expr_statement
%type <code_hunk> ppc_statement
%type <code_hunk> pp_lexpr
%type <code_hunk> def_statement
%type <code_hunk> def_begin
%type <code_hunk> start_signature
%type <code_hunk> signature
%type <code_hunk> non_empty_signature
%type <code_hunk> param_list
%type <code_hunk> param_with_default_list
%type <code_hunk> param_with_default
%type <code_hunk> initializer_expression
%type <code_hunk> val
%type <code_hunk> factor
%type <code_hunk> term
%type <code_hunk> exprI
%type <code_hunk> exprH
%type <code_hunk> exprG
%type <code_hunk> exprF
%type <code_hunk> exprE
%type <code_hunk> exprD
%type <code_hunk> exprC
%type <code_hunk> exprB
%type <code_hunk> exprA
%type <code_hunk> exprA1
%type <code_hunk> exprA2
%type <code_hunk> nonifexpr
%type <code_hunk> ifexpr
%type <code_hunk> ifexpr1
%type <code_hunk> elseif_clause
%type <code_hunk> elseif_clause1
%type <code_hunk> else_clause
%type <code_hunk> else_clause1
%type <code_hunk> expr
%type <code_hunk> function_call
%type <code_hunk> function_call1
%type <code_hunk> optional_block
%type <code_hunk> optional_block1
%type <code_hunk> optional_pipe_param_list
%type <code_hunk> array
%type <code_hunk> expr_list
%type <code_hunk> pure_expr_list
%type <code_hunk> hash
%type <code_hunk> hash_list
%type <code_hunk> hash_element
%type <code_hunk> arg_declaritor
%type <code_hunk> pure_lexpr
%type <code_hunk> var_lexpr
%type <code_hunk> array_lexpr
%type <code_hunk> box_statement
%type <code_hunk> box_begin

%start start

%%

start:
  external_statement_list TOKEOF
  {
    dump_code($1, output_file);
  }
  ;

internal_statement_list:
  {
    $$ = get_nil();
  }
  | internal_statement_list internal_statement
  {
    $$ = CCH($1, $2);
  }
  ;

internal_statement:
  blank_line
  { $$ = $1 }
  | expr_statement /* Pop the stack before every statement */
  { $$ = CCH(pop_stack(1), $2); }
  ;
external_statement_list:
  { get_nil(); }
  | external_statement_list external_statement
  { $$ = CCH($1, $2); }
  ;

external_statement:
  internal_statement
  { $$ = $1; }
  | def_statement
  { $$ = CCH(pop_stack(), $2); }
  | box_statement
  { $$ = CCH(pop_stack(), $2); }
  | ppc_statement
  { $$ = CCH(pop_stack(), $2); }
  ;

  
blank_line:
  TOKEOL
  { $$ = NULL; }
  ;

expr_statement:
  expr TOKEOL
  { $$ = $1; }
  ;

word:
  TOKWORD
  { $$ = add_symbol($1); }
  ;

const:
  TOKCONST
  { $$ = add_symbol($1); }
  ;

const_or_word:
  const
  { $$ = $1; }
  | word
  { $$ = $1; }
  ;

ppc_statement:
  pp_lexpr TOKEQ expr TOKEOL
  {
    $$ = assign_var();
  }
  | pp_lexpr TOKEOL
  {
    $$ = CCH(get_nil(), assign_var());
  }
  | const TOKEQ expr TOKEOL
  {
    add_const($1);
    $$ = CCH(get_reference($1), assign_var());
  }
  ;

pp_lexpr:
  TOKPRIVATE word
  {
    add_private($2);
    $$ = get_reference($2);
  }
  | TOKPUBLIC word
  {
    add_public($2);
    $$ = get_reference($2);
  }
  ;
  
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
  | add
  {
    $$ = $1;
  }
  | mul
  {
    $$ = $1;
  }
  | shift
  {
    $$ = $1;
  }
  | comparisonA
  {
    $$ = $1;
  }
  | comparisonB
  {
    $$ = $1;
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
    $$ = add_symbol($1);
  }
  ;

add:
  TOKPLUS | TOKMINUS
  {
    $$ = add_symbol($1);
  }
  ;

mul:
  TOKSTAR | TOKSLASH | TOKPERCENT
  {
    $$ = add_symbol($1);
  }
  ;
  
shift:
  TOKSHIFTL | TOKSHIFTR
  {
    $$ = add_symbol($1);
  }
  ;

comparisonA:
  TOKGT | TOKLT | TOKGE | TOKLE
  {
    $$ = add_symbol($1);
  }
  ;

comparisonB:
  TOKEQEQ | TOKNEQ | TOKCOMPARE
  {
    $$ = add_symbol($1);
  }
  ;

assignop:
  TOKPLUSEQ | TOKMINUSEQ | TOKSTAREQ | TOKSLASHEQ | TOKPERCENTEQ | TOKAMPEQ | TOKCARATEQ | TOKPIPEEQ | TOKSHIFTLEQ | TOKSHIFTREQ
  {
    $$ = add_symbol($1);
  }
  ;

assignboolop:
  TOKANDEQ | TOKOREQ
  {
    $$ = add_symbol($1);
  }
  ;

symbol:
  TOKSYMBOL
  {
    $$ = add_symbol($1);
  }
  ;

begin:
  TOKBEGIN
  {
    push_scope();
  }
  ;
  
val:
  TOKLPAREN expr TOKRPAREN
  {}
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
  | TOKBLOCKGIVEN
  {
    $$ = block_given();
  }
  | symbol
  {
    $$ = new_sym_thing($1);
  }
  | TOKBUCK TOKLPAREN pure_expr_list TOKRPAREN
  {
    $$ = CCH($2, clone($2));
  }
  | array
  { $$ = $1; }
  | hash
  { $$ = $1; }
  | unary val
  {
    new_sym_thing($1);
    $$ = CCH($2, call_send(1));
  }
  | function_call
  {}
  | pure_lexpr
  {
    $$ = CCH($1, dereference());
  }
  | array_lexpr
  {
    new_sym_thing(add_symbol("[]"));
    $$ = call_send(1);
  }
  | const
  {
    new_sym_thing($1);
    $$ = dereference();
  }
  | begin TOKEOL internal_statement_list TOKEND
  {
    pop_scope();
    int n = count_vars($3);
    $$ = CCH(CCH(push_stack(n), $3), pop_stack($3));
  }
  ;

factor:
  val
  {}
  | factor mul val
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

term:
  factor
  {}
  | term add factor
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprI:
  term
  {}
  | exprI shift term
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprH:
  exprI
  {}
  | exprH comparisonA exprI
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;
  
exprG:
  exprH
  {}
  | exprG comparisonB exprH
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprF:
  exprG
  {}
  | exprF TOKAMP exprG
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprE:
  exprF
  {}
  | exprE TOKCARAT exprF
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;
  
exprD:
  exprE
  {}
  | exprD TOKPIPE exprE
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;
  
exprC:
  exprD
  {}
  | exprC TOKAND exprD
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprB:
  exprC
  {}
  | exprB TOKOR exprC
  {
    new_sym_thing($2);
    $$ = call_send(2);
  }
  ;

exprA:
  exprB
  {}
  | exprB exprA1 TOKQUEST exprA exprA2 TOKCOLON exprA
  {
    $$ = CCH(CCH(CCH(CCH(CCH($1,$2),$4),$5),$7),end_if());
  }
  ;
  
exprA1:
  {
    $$ = start_if();
  }
  ;
  
exprA2:
  {
    $$ = start_else();
  }
  ;
  
nonifexpr:
  var_lexpr TOKEQ expr
  {
    $$ = CCH(CCH($1,$3),assign_var());
  }
  | pure_lexpr TOKEQ expr
  {
    $$ = assign_var();
  }
  | pure_lexpr assignop expr
  {
    $$ = assign_var();
  }
  | pure_lexpr assignboolop expr
  {
    $$ = assign_var();
  }
  |
  array_lexpr TOKEQ expr
  {
    $$ = assign_array_var();
  }
  | array_lexpr assignop expr
  {
    $$ = assign_array_var();
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
  pure_lexpr argument_list function_call1 optional_block
  {
    call_send($2);
  }
  ;

function_call1:
  {
    signature = new_empty_signature();
  }
  ;

optional_block:
  {
    get_nil();
  }
  | TOKDO optional_pipe_param_list TOKEOL optional_block1 internal_statement_list TOKEND
  {
    pop_compile();
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
    new_array_thing(0);
  }
  | TOKLBRACK expr_list TOKRBRACK
  {
    new_array_thing($2);
  }
  ;

expr_list:
  pure_expr_list
  {
    $$ = $1;
  }
  | pure_expr_list TOKCOMMA hash_list
  {
    new_hash_thing($3);
    $$ = $1 + 1;
  }
  | hash_list
  {
    new_hash_thing($1);
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
    new_hash_thing(0);
  }
  | TOKLBRACE hash_list TOKRBRACE
  {
    new_hash_thing($2);
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

argument_list:
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
    $$ = CCH($1, get_reference($1));
  }
  | val TOKDOT const_or_word
  {
    $$ = CCH($1, get_member_reference($3));
  }
  | TOKBUCK TOKDOT const_or_word
  {
    $$ = CCH(buck(), get_member_reference($3));
  }
  ;

var_lexpr:
  TOKVAR word
  {
    add_var($2);
    get_reference($2);
  }
  ;
  
array_lexpr:
  word TOKLBRACK expr TOKRBRACK
  {
    get_reference($1);
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
  TOKBOX const TOKEOL
  {
    push_frame();
    start_box($2);
  }
  ;
