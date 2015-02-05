%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

#define return_it(tok) yylval.string=strdup(yytext); return(tok);

%}
%%
                        /* Keywords */
box                     return_it(TOKBOX);
end                     return_it(TOKEND);
def                     return_it(TOKDEF);
var                     return_it(TOKVAR);
self                    return_it(TOKSELF);
if                      return_it(TOKIF);
else                    return_it(TOKELSE);
elseif                  return_it(TOKELSEIF);
empty                   return_it(TOKEMPTY);
return                  return_it(TOKRETURN);

                        /* Numbers, strings */
[0-9]+                  {yylval.number=atoi(yytext); return(TOKNUMBER);}
\"(\\.|[^\n\\"])*\"     return_it(TOKDSTRING);
\'(\\.|[^\n\\'])*\'     return_it(TOKSSTRING);

                        /* Token */
[a-z0-9_]+?{0,1}        return_it(TOKWORD);

                        /* EOL */
\n                      return_it(TOKEOL);

                        /* Operators */
\.                      return_it(TOKDOT);

\+                      return_it(TOKPLUS);
-                       return_it(TOKMINUS);
!                       return_it(TOKBANG);
~                       return_it(TOKTILDE);

\*                      return_it(TOKSTAR);
\/                      return_it(TOKSLASH);
%                       return_it(TOKPERCENT);

\<\<                    return_it(TOKSHIFTL);
\>\>                    return_it(TOKSHIFTR);

\>                      return_it(TOKGT);
\<                      return_it(TOKLT);
\>=                     return_it(TOKGE);
\<=                     return_it(TOKLE);

==                      return_it(TOKEQEQ);
!=                      return_it(TOKNEQ);

\&                      return_it(TOKAMP);

\^                      return_it(TOKCARAT);

\|                      return_it(TOKPIPE);

\&\&                    return_it(TOKAND);

\|\|                    return_it(TOKOR);

\?                      return_it(TOKQUEST);
:                       return_it(TOKCOLON);

=                       return_it(TOKEQ);
\+=                     return_it(TOKPLUSEQ);
-=                      return_it(TOKMINUSEQ);
\*=                     return_it(TOKSTAREQ);
\/=                     return_it(TOKSLASHEQ);
%=                      return_it(TOKPERCENTEQ);
\&=                     return_it(TOKAMPEQ);
\^=                     return_it(TOKCARATEQ);
\|=                     return_it(TOKPIPEEQ);
&&=                     return_it(TOKANDEQ);
\|\|=                   return_it(TOKOREQ);
\<\<=                   return_it(TOKSHIFTLEQ);
\>\>=                   return_it(TOKSHIFTREQ);

=>                      return_it(TOKHASHROCK);

\{                      return_it(TOKLBRACE);
\}                      return_it(TOKRBRACE);
\(                      return_it(TOKLPAREN);
\)                      return_it(TOKRPAREN);
\[                      return_it(TOKLBRACK);
\]                      return_it(TOKRBRACK);

,                       return_it(TOKCOMMA);

\\\n                    /* Ignore EOL after backslash */;
[ \t]+                  /* Ignore whitespace */;
#.*                     /* Ignore comments */

%%
