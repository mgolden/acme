%{
#include "compiler.h"

void yyerror(const char *str)
{
  e_error(str);
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
  char *output_file_name = acme_strdup(input_file_name);
  char *p = strrchr(file_name, ".");
  if(strcmp(p,".ac")!=0) usage();
  *p++ = 'c';
  *p = '\0';
  output_file = fopen(output_file_name, "w");
  acme_free(output_file_name);
  yyin = fopen(input_file_name, "r");
  compiler_init();
  yyparse();
  fclose(yyin);
}

static void usage(void) {
  fprintf(stderr, "usage: acme source.ac\n");
  exit(1);
}

%}

%token TOKEOF  0  "end of file"

/* Keywords */
%token <s> TOKABILITY
%token <s> TOKEND
%token <s> TOKMETHOD
%token <s> TOKVAR
%token <s> TOKPROPERTY
%token <s> TOKSHY
%token <s> TOKSELF
%token <s> TOKIF
%token <s> TOKELSE
%token <s> TOKELSEIF
%token <s> TOKRETURN
%token <s> TOKTRY
%token <s> TOKCATCH
%token <s> TOKFINALLY
%token <s> TOKDO
%token <s> TOKBLOCKGIVEN
%token <s> TOKNIL
%token <s> TOKTRUE
%token <s> TOKFALSE
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
%token <s> TOKAT

%union 
{
  acme_int i;
  acme_float f;
  char *s;
  symbol sym;
  code_hunk *code_hunk;
}

%type <i> shy

%type <str> property_lexpr
%type <str> const_or_word
%type <str> function_name
%type <str> add_s
%type <str> mul_s
%type <str> shift_s
%type <str> comparisonA_s
%type <str> comparisonB_s
%type <str> assignop_s

%type <sym> unary
%type <sym> add
%type <sym> mul
%type <sym> shift
%type <sym> comparisonA
%type <sym> comparisonB

%type <code_hunk> buck
%type <code_hunk> nil
%type <code_hunk> true
%type <code_hunk> false
%type <code_hunk> i
%type <code_hunk> f
%type <code_hunk> s
%type <code_hunk> symbol

%type <code_hunk> internal_statement_list
%type <code_hunk> internal_statement
%type <code_hunk> external_statement_list
%type <code_hunk> external_statement
%type <code_hunk> blank_line
%type <code_hunk> expr_statement
%type <code_hunk> property_statement
%type <code_hunk> method_statement
%type <code_hunk> initializer_expression
%type <code_hunk> cval
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
%type <code_hunk> nonifexpr
%type <code_hunk> ifexpr
%type <code_hunk> elseif_clause
%type <code_hunk> else_clause
%type <code_hunk> expr
%type <code_hunk> function_call
%type <code_hunk> optional_block
%type <code_hunk> optional_pipe_param_list
%type <code_hunk> array
%type <code_hunk> expr_list
%type <code_hunk> pure_expr_list
%type <code_hunk> hash
%type <code_hunk> hash_list
%type <code_hunk> hash_element
%type <code_hunk> argument_list
%type <lexpr_hunk> pure_lexpr
%type <lexpr_hunk> var_lexpr
%type <lexpr_hunk> array_lexpr
%type <code_hunk> ability_statement
%type <code_hunk> ability_begin

%start start

%%

start:
  external_statement_list TOKEOF
  {
    dump_function(NULL, $1, output_file);
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
  error TOKEOL
  {
    e_error("Error matched");
    $$ = NULL;
  }
  | blank_line
  { $$ = $1 }
  | expr_statement /* Pop the stack before every statement */
  { $$ = CCH(pop_stack(1), $1); }
  ;
external_statement_list:
  { $$ = get_nil(); }
  | external_statement_list external_statement
  { $$ = CCH($1, $2); }
  ;

external_statement:
  internal_statement
  { $$ = $1; }
  | method_statement
  { $$ = CCH(pop_stack(1), $1); }
  | ability_statement
  { $$ = CCH(pop_stack(1), $1); }
  | property_statement
  { $$ = CCH(pop_stack(1), $1); }
  ;

  
blank_line:
  TOKEOL
  { $$ = NULL; }
  ;

expr_statement:
  expr TOKEOL
  { $$ = $1; }
  ;

const_or_word:
  TOKCONST
  { $$ = $1; }
  | TOKWORD
  { $$ = $1; }
  ;

property_statement:
  property_lexpr TOKEQ expr TOKEOL
  {
    $$ = assign_lexpr($1,$2,$3);
  }
  | property_lexpr TOKEOL
  {
    /* Just put the pp member in appropriate table, do not assign */
    $$ = NULL;
  }
  ;

shy:
  {
    $$ = 0;
  }
  | TOKSHY
  {
    $$ = 1;
  }
  ;

property_lexpr:
  TOKPROPERTY shy const_or_word
  {
    add_property($3, $2);
    $$ = make_lexpr_hunk(get_self(), $3, NULL);
  }
  ;

method_statement:
  method_line internal_statement_list TOKEND TOKEOL
  {
    dump_function($2, output_file);
    $$ = get_nil();
  }
  ;

method_line:
  method_name signature TOKEOL
  {
    fresh_scope(sig);
  }
  ;

method_name:
  TOKMETHOD shy function_name
  {
    start_signature($3, $2);
  }
  ;

function_name:
  TOKWORD
  {
    $$ = $1;
  }
  | TOKWORD TOKEQ
  {
    size_t l1 = strlen($1);
    size_t l2 = strlen($2);
    char *x = acme_malloc(l1 + l2 + 1);
    strcpy(x, $1);
    char *xx = x + l1;
    strcpy(xx, $2);
    $$ = xx;
  }
  | TOKLBRACK TOKRBRACK
  {
    $$ = acme_strdup("[]");
  }
  | TOKLBRACK TOKRBRACK TOKEQ
  {
    $$ = acme_strdup("[]=");
  }
  | TOKPLUS TOKAT
  {
    $$ = acme_strdup("+@");
  }
  | TOKMINUS TOKAT
  {
    $$ = acme_strdup("-@");
  }
  | TOKTILDE
  {
    $$ = acme_strdup($1);
  }
  | TOKBANG
  {
    $$ = acme_strdup($1);
  }
  | add_s
  { $$ = $1; }
  | mul_s
  { $$ = $1; }
  | shift_s
  { $$ = $1; }
  | comparisonA_s
  { $$ = $1; }
  | comparisonB_s
  { $$ = $1; }
  ;

signature:
  | TOKLPAREN TOKRPAREN
  | TOKLPAREN non_empty_signature TOKRPAREN
  ;

non_empty_signature:
  non_star_signature
  {}
  | non_star_signature TOKCOMMA signature_star_part
  {}
  | signature_star_part
  {}
  ;

non_star_signature:
  param_list
  {}
  | param_list TOKCOMMA param_with_default_list
  {}
  | param_with_default_list
  {}
  ;

signature_star_part:
  TOKSTAR TOKWORD
  {
    star_param(sig, $2));
  }
  ;

param_list:
  param
  | param_list TOKCOMMA param
  ;

param:
  TOKWORD
  {
    param(sig, $1);
  }
  ;
  
param_with_default_list:
  param_with_default
  | param_with_default_list TOKCOMMA param_with_default
  ;

param_with_default:
  TOKWORD TOKEQ initializer_expression
  { param_with_default(sig, $1, $3)); }
  ;

initializer_expression:
  cval
  {
    $$ = $1;
  }
  | TOKLBRACK TOKRBRACK
  {
    $$ = new_array_thing(0);
  }
  | TOKLBRACE TOKRBRACE
  {
    $$ = new_hash_thing(0);
  }
  ;

unary:
  TOKPLUS 
  {
    $$ = get_symbol("+@");
  }
  | TOKMINUS
  {
    $$ = get_symbol("-@");
  }
  | TOKBANG 
  {
    $$ = get_symbol($1);
  }
  | TOKTILDE
  {
    $$ = get_symbol($1);
  }
  ;

add:
  add_s
  {
    $$ = get_symbol($1);
  }
  ;

add_s:
  TOKPLUS {} | TOKMINUS {}
  ;

mul:
  mul_s
  {
    $$ = get_symbol($1);
  }
  ;
  
mul_s:
  TOKSTAR {} | TOKSLASH {} | TOKPERCENT {}
  ;

shift:
  shift_s
  {
    $$ = get_symbol($1);
  }
  ;

shift_s:
  TOKSHIFTL {} | TOKSHIFTR {}
  ;

comparisonA:
  comparisonA_s
  {
    $$ = get_symbol($1);
  }
  ;

comparisonA_s:
  TOKGT {} | TOKLT {} | TOKGE {} | TOKLE {}
  ;

comparisonB:
  comparisonB_s
  {
    $$ = get_symbol($1);
  }
  ;

comparisonB_s:
  TOKEQEQ {} | TOKNEQ {} | TOKCOMPARE {}
  ;

  
assignop_s:
  TOKEQ {}
  | TOKANDEQ {} | TOKOREQ {}
  | TOKPLUSEQ {} | TOKMINUSEQ {} | TOKSTAREQ {} | TOKSLASHEQ {} | TOKPERCENTEQ {}
  | TOKAMPEQ {} | TOKCARATEQ {} | TOKPIPEEQ {} | TOKSHIFTLEQ {} | TOKSHIFTREQ {}
  ;

begin:
  TOKBEGIN
  {
    push_scope(NULL);
  }
  ;

buck:
  TOKBUCK
  {
    $$ = get_buck();
  }
  ;

nil:
  TOKNIL
  {
    $$ = get_nil();
  }
  ;

true:
  TOKTRUE
  {
    $$ = get_true();
  }
  ;

false:
  TOKFALSE
  {
    $$ = get_false();
  }
  ;

i:
  TOKI
  {
    $$ = new_i_thing($1);
  }
  ;

f:
  TOKF
  {
    $$ = new_f_thing($1);
  }
  ;

s:
  TOKS
  {
    $$ = new_s_thing($1);
  }
  ;

symbol:
  TOKSYMBOL
  {
    $$ = new_sym_thing($1);
  }
  ;

cval:
  buck
  { $$ = $1; }
  | nil
  { $$ = $1; }
  | true
  { $$ = $1; }
  | false
  { $$ = $1; }
  | i
  { $$ = $1; }
  | f
  { $$ = $1; }
  | s
  { $$ = $1; }
  | symbol
  { $$ = $1; }
  ;

val:
  cval
  { $$ = $1; }
  | TOKLPAREN expr TOKRPAREN
  { $$ = $2; }
  | TOKSELF
  { $$ = get_self(); }
  | TOKBLOCKGIVEN
  { $$ = block_given(); }
  | TOKAT TOKLPAREN pure_expr_list TOKRPAREN
  {
    $$ = CCH($3, clone($3->comexprs));
    $$->comexprs = 0;
  }
  | array
  { $$ = $1; }
  | hash
  { $$ = $1; }
  | unary val
  { $$ = CCH(CCH(CCH(get_nil(), $2), new_sym_thing_from_symbol($1)), call_send(0)); }
  | function_call
  { $$ = $1; }
  | pure_lexpr
  { $$ = dereference($1); }
  | array_lexpr
  { $$ = dereference($1); }
  | TOKCONST
  { $$ = dereference(make_lexpr_hunk(get_self(), $1, NULL)); }
  | val TOKDOT TOKCONST
  { $$ = dereference(make_lexpr_hunk($1, $3, NULL)); }
  | begin TOKEOL internal_statement_list TOKEND
  {
    pop_scope();
    $$ = CCH(CCH(push_stack($3->locvars), $3), pop_stack(n));
    $$->locvars = 0;
  }
  | TOKYIELD argument_list
  {
    $$ = CCH(CCH(CCH($2, call_block()), pop_stack($2->comexprs)), possible_return_from_block());
  }
  ;

factor:
  val
  { $$ = $1; }
  | factor mul val
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing_from_symbol($2)), call_send(1));
  }
  ;

term:
  factor
  { $$ = $1; }
  | term add factor
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing_from_symbol($2)), call_send(1));
  }
  ;

exprI:
  term
  { $$ = $1; }
  | exprI shift term
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing_from_symbol($2)), call_send(1));
  }
  ;

exprH:
  exprI
  { $$ = $1; }
  | exprH comparisonA exprI
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing_from_symbol($2)), call_send(1));
  }
  ;
  
exprG:
  exprH
  { $$ = $1; }
  | exprG comparisonB exprH
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing_from_symbol($2)), call_send(1));
  }
  ;

exprF:
  exprG
  { $$ = $1; }
  | exprF TOKAMP exprG
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing($2)), call_send(1));
  }
  ;

exprE:
  exprF
  { $$ = $1; }
  | exprE TOKCARAT exprF
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing($2)), call_send(1));
  }
  ;
  
exprD:
  exprE
  { $$ = $1; }
  | exprD TOKPIPE exprE
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing($2)), call_send(1));
  }
  ;
  
exprC:
  exprD
  { $$ = $1; }
  | exprC TOKAND exprD
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing($2)), call_send(1));
  }
  ;

exprB:
  exprC
  { $$ = $1; }
  | exprB TOKOR exprC
  {
    $$ = CCH(CCH(CCH(CCH($3, get_nil()), $1), new_sym_thing($2)), call_send(1));
  }
  ;

exprA:
  exprB
  { $$ = $1; }
  | exprB TOKQUEST exprA TOKCOLON exprA
  {
    $$ = CCH(CCH(CCH(CCH(CCH($1,start_if()),$3),start_else()),$5),end_if());
  }
  ;
  
nonifexpr:
  var_lexpr TOKEQ expr
  {
    $$ = assign_lexpr($1,$2,$3);
  }
  | pure_lexpr assignop_s expr
  {
    $$ = assign_lexpr($1,$2,$3);
  }
  | array_lexpr assignop_s expr
  {
    $$ = assign_lexpr($1,$2,$3);
  }
  | exprA
  { $$ = $1; }
  ;

ifexpr:
  TOKIF nonifexpr TOKEOL internal_statement_list elseif_clause else_clause TOKEND
  {
    $$ = CCH(CCH(CCH(CCH(CCH($2,start_if()),$4),$5),$6),end_if());
  }
  ;

elseif_clause:
  { $$ = NULL; }
  | elseif_clause TOKELSEIF nonifexpr TOKEOL internal_statement_list
  {
    $$ = CCH(CCH(CCH($1, $3),start_elseif()), $5);
  }
  ;

else_clause:
  { $$ = NULL; }
  |
  TOKELSE TOKEOL internal_statement_list
  {
    $$ = CCH(start_else(), $3);
  }
  ;


expr:
  nonifexpr
  { $$ = $1; }
  | ifexpr
  { $$ = $1; }
  ;

function_call:
  pure_lexpr argument_list optional_block
  {
    $$ = CCH(CCH(CCH(CCH($2, $3), $1->self_ch), new_sym_thing($1->name)), call_send($2->comexprs));
    $$->comexprs = 0;
    /* Free the lexpr_hunk, but not the code_hunk it contains */
    if($1->subscript_ch) {
      /* You can't have a subscript here */
      e_fatal("Found non-empty subscript in function call lexpr_hunk");
    }
    acme_free($1->name);
    acme_free($1);
  }
  ;

optional_block:
  {
    $$ = get_nil();
  }
  | do optional_pipe_param_list TOKEOL internal_statement_list TOKEND
  {
    end_block();
    dump_function($4, output_file);
  }
  ;

do:
  TOKDO
  {
    char * name = make_block_name();
    start_signature(name, 1);
    acme_free(name);
  }
  ;

optional_pipe_param_list:
  { push_scope(sig); }
  | TOKPIPE param_list TOKPIPE
  { push_scope(sig); }
  ;

array:
  TOKLBRACK TOKRBRACK
  {
    $$ = new_array_thing(0);
  }
  | TOKLBRACK expr_list TOKRBRACK
  {
    $$ = CCH($2, new_array_thing($2->comexprs));
    $$->comexprs = 0;
  }
  ;

expr_list:
  pure_expr_list
  {
    $$ = $1;
  }
  | pure_expr_list TOKCOMMA hash_list
  {
    code_hunk *ch = CCH($3, new_hash_thing($3->comexprs));
    ch->comexprs = 1; /* We've made a single hash out of the comexprs */
    $$ = CCH(ch, $1); /* Note: pushing on stack in reverse order */
  }
  | hash_list
  {
    $$ = CCH($1, new_hash_thing($1->comexprs));
    $$->comexprs = 1; /* We've made a single hash out of the comexprs */
  }
  ;

pure_expr_list:
  expr
  {
    $$ = $1;
    $$->comexprs = 1;
  }
  |
  pure_expr_list TOKCOMMA expr
  {
    $$ = CCH($3, $1); /* Note: pushing on stack in reverse order */
    $$->comexprs++;
  }
  ;

hash:
  TOKLBRACE TOKRBRACE
  {
    $$ = new_hash_thing(0);
  }
  | TOKLBRACE hash_list TOKRBRACE
  {
    $$ = CCH($2, new_hash_thing($2->comexprs);
    $$->comexprs = 0;
  }
  ;

hash_list:
  hash_element
  {
    $$ = $1;
  }
  | hash_list TOKCOMMA hash_element
  {
    $$ = CCH($3,$1); /* Note: pushing on stack in reverse order */
  }
  ;

hash_element:
  expr TOKHASHROCK expr
  {
    $$ = CCH($3,$1); /* Note: pushing on stack in reverse order */
    $$->comexprs = 2;
  }
  ;

argument_list:
  TOKLPAREN TOKRPAREN
  {
    $$ = NULL;
  }
  | TOKLPAREN expr_list TOKRPAREN
  {
    $$ = $2;
  }
  ;

pure_lexpr:
  TOKWORD
  {
    $$ = make_lexpr_hunk(NULL, $1, NULL);
  }
  | val TOKDOT TOKWORD
  {
    $$ = make_lexpr_hunk($1, $2, NULL);
  }
  ;

var_lexpr:
  TOKVAR TOKWORD
  {
    add_var($2);
    $$ = make_lexpr_hunk(NULL, $2, NULL);
  }
  ;
  
array_lexpr:
  pure_lexpr TOKLBRACK expr TOKRBRACK
  {
    $1->subscript_ch = $3;
    $$ = $1 ;
  }
  ;
  
ability_statement:
  ability_begin external_statement_list TOKEND
  {
    pop_label()
    pop_scope();
    $$ = CCH(CCH(CCH($1, push_stack($3->locvars)), $3), pop_stack(n));
    $$->locvars = 0;
  }
  ;

ability_begin:
  TOKABILITY TOKCONST TOKEOL
  {
    push_scope(NULL);
    push_label($2);
    $$ = NULL;
  }
  ;
