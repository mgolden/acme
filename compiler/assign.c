#include "compiler.h"

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch) {
  /* Get the op, that is, the operation before the = */
  char op[4];
  strncpy(op, assignop, 4);
  op[3] = '\0';
  /* If there is no explicit self, we need to search for the variable in several places */
  name = lh->name;
  fp_offset = get_var_fp_offset(name);
  if(fp_offset == 0) {
    /* Look for shy variable */
    dotted_labels = get_dotted_labels();
    int n1 = strlen(dotted_lables);
    int n2 = strlen(name);
    char *shy_name = acme_malloc(n1 + n2 + 2);
    strcpy(shy_name, dotted_labels);
    char *sn2 = shy_name + n1;
    *(sn2++) = '.';
    strcpy(sn2, name);
    // Try to find shy name
    if(!found) {
      // Try to find name
    }
  }
  /* Make the code_hunk for us to return */
  code_hunk *ret = CCH(CH("{\n"),lh->self_ch));
  lh->self_ch = NULL;
  ret = CCH(ret,CH("\nthing *self=stack[sp];\nstack[sp--]=NULL\n;")
  CCH(
  if(op[0] != '\0') {
    /* Not a pure = assignment */
    
  
}

code_hunk * dereference(lexpr_hunk *lh){
}
