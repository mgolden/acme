#include "compiler.h"

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch) {
  /* Get the op, that is, the operation before the = */
  char op[4];
  strncpy(op, assignop, 4);
  op[3] = '\0';
  /* If there is no explicit self, we need to search for the variable in several places */
  name = lh->name;
  /* First as a local var */
  fp_offset = get_var_fp_offset(name);
  if(fp_offset != 0) {
  }
  else {
    symbol to_call = 0;
    /* Look for shy variable */
    dotted_labels = get_dotted_labels();
    int n1 = strlen(dotted_lables);
    int n2 = strlen(name);
    char *shy_assign_str = acme_malloc(n1 + n2 + 3);
    char *str2 = shy_assign_str;
    strcpy(str2, dotted_labels);
    acme_free(dotted_labels); /* caller must free */
    str2 += n1;
    *(str2++) = '.';
    strcpy(sn2, name);
    str2 += n2;
    *(str2++) = '=';
    *str2 = '\0';
    symbol shy_assign_sym = get_symbol(shy_assign_name);
    if(function_exists(shy_assign_sym)) {
      to_call = shy_assign_sym;
      acme_free(shy_name);
    }
    else {
      char *assign_str = acme_malloc(n2+2);
      str2 = assign_str;
      strcpy(assign_str, name);
      *(str2++) = '=';
      *str2 = '\0';
      to_call = get_symbol(assign_name);
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
