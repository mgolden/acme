#ifndef ASSIGN_H
#define ASSIGN_H

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch);
code_hunk * dereference(lexpr_hunk *lh);

#endif
