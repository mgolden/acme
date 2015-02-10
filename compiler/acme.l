%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../acme_types.h"

#include "y.tab.h"

/* Return a constant string, no need to duplicate */
#define return_s_c(tok, _s) {yylval.s=_s; return(tok);}
/* Return an acme_integer */
#define return_i(tok) {yylval.i=atoll(yytext); return(tok);}
/* Return an acme_float */
#define return_f(tok) {yylval.f=atof(yytext); return(tok);}
/* Return a string, need to duplicate */
#define return_s(tok) {yylval.s=strdup(yytext); return(tok);}
/* +1 because we don't want to include the initial colon */
#define return_sym(tok) {yylval.s=strdup(yytext+1); return(tok);}

%}
%%
                        /* Keywords */
box                     return_s_c(TOKBOX, "box");
end                     return_s_c(TOKEND, "end");
def                     return_s_c(TOKDEF, "def");
var                     return_s_c(TOKVAR, "var");
public                  return_s_c(TOKPUBLIC, "public");
private                 return_s_c(TOKPRIVATE, "private");
self                    return_s_c(TOKSELF, "self");
if                      return_s_c(TOKIF, "if");
else                    return_s_c(TOKELSE, "else");
elseif                  return_s_c(TOKELSEIF, "elseif");
empty                   return_s_c(TOKEMPTY, "empty");
return                  return_s_c(TOKRETURN, "return");
try                     return_s_c(TOKTRY, "try");
catch                   return_s_c(TOKCATCH, "catch");
finally                 return_s_c(TOKFINALLY, "finally");
do                      return_s_c(TOKDO, "do");
block_given?            return_s_c(TOKBLOCKGIVEN, "block_given?");
nil                     return_s_c(TOKNIL, "nil");
yield                   return_s_c(TOKYIELD, "yield");

                        /* Numbers, strings */
[0-9]+                  return_i(TOKI);
[0-9]+\.[0-9]+          return_f(TOKF);
\"(\\.|[^\n\\"])*\"     return_s(TOKS);
\'(\\.|[^\n\\'])*\'     return_s(TOKS);

                        /* Token */
[a-z][A-Za-z0-9_]*\?{0,1}    return_s(TOKWORD);
[A-Z][A-Za-z0-9_]*\?{0,1}    return_s(TOKCONST);

                        /* Symbol */
:[A-Za-z0-9_\[\]]+\?{0,1}    return_sym(TOKSYMBOL);

                        /* EOL */
\n                      return_s_c(TOKEOL, "\n");
;                       return_s_c(TOKEOL, ";");

                        /* Operators */
\.                      return_s_c(TOKDOT, ".");

\+                      return_s_c(TOKPLUS, "+");
-                       return_s_c(TOKMINUS, "-");
!                       return_s_c(TOKBANG, "!");
~                       return_s_c(TOKTILDE, "~");

\*                      return_s_c(TOKSTAR, "*");
\/                      return_s_c(TOKSLASH, "/");
%                       return_s_c(TOKPERCENT, "%");

\<\<                    return_s_c(TOKSHIFTL, "<<");
\>\>                    return_s_c(TOKSHIFTR, ">>");

\>                      return_s_c(TOKGT, ">");
\<                      return_s_c(TOKLT, "<");
\>=                     return_s_c(TOKGE, ">=");
\<=                     return_s_c(TOKLE, "<=");

==                      return_s_c(TOKEQEQ, "==");
!=                      return_s_c(TOKNEQ, "!=");
\<=\>                   return_s_c(TOKCOMPARE, "<=>");

\&                      return_s_c(TOKAMP, "&");

\^                      return_s_c(TOKCARAT, "^");

\|                      return_s_c(TOKPIPE, "|");

\&\&                    return_s_c(TOKAND, "&&");

\|\|                    return_s_c(TOKOR, "||");

\?                      return_s_c(TOKQUEST, "?");
:                       return_s_c(TOKCOLON, ":");

=                       return_s_c(TOKEQ, "=");
\+=                     return_s_c(TOKPLUSEQ, "+=");
-=                      return_s_c(TOKMINUSEQ, "-=");
\*=                     return_s_c(TOKSTAREQ, "*=");
\/=                     return_s_c(TOKSLASHEQ, "/=");
%=                      return_s_c(TOKPERCENTEQ, "%=");
\&=                     return_s_c(TOKAMPEQ, "&=");
\^=                     return_s_c(TOKCARATEQ, "^=");
\|=                     return_s_c(TOKPIPEEQ, "|=");
&&=                     return_s_c(TOKANDEQ, "&&=");
\|\|=                   return_s_c(TOKOREQ, "||=");
\<\<=                   return_s_c(TOKSHIFTLEQ, "<<=");
\>\>=                   return_s_c(TOKSHIFTREQ, ">>=");

=>                      return_s_c(TOKHASHROCK, "=>");

\{                      return_s_c(TOKLBRACE, "{");
\}                      return_s_c(TOKRBRACE, "}");
\(                      return_s_c(TOKLPAREN, "(");
\)                      return_s_c(TOKRPAREN, ")");
\[                      return_s_c(TOKLBRACK, "[");
\]                      return_s_c(TOKRBRACK, "]");

,                       return_s_c(TOKCOMMA, ",");

\\[ \t]*\n              /* Ignore EOL after backslash */;
[ \t]+                  /* Ignore whitespace */;
#.*                     /* Ignore comments */

%%
