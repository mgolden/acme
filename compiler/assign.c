#include "compiler.h"

static code_hunk * push_var_in_context(lexpr_hunk *lh);

code_hunk * assign_lexpr(lexpr_hunk *lh, const char *assignop, code_hunk *ch) {
  /* Get the op, that is, the operation before the = */
  char op[4];
  strncpy(op, assignop, 4);
  op[3] = '\0';
  /* remove the = */
  int l_op = strlen(op);
  if(l_op>1 && op[--l_op]=='=') {op[l_op] = '\0';}
  symbol op_sym;
  if(l_op>0) {
    op_sym = get_symbol(op);
  }
  /* Push the result of the right-hand code hunk */
  code_hunk *ret = ch;
  /* First case: If no explicit self, find symbol in the local variable table */
  if(lh->self_ch == NULL) {
    code_hunk * local_ch = get_local_var_ch(lh->sym);
    if(local_ch != NULL) {
      /* We have a local variable, if not an array, assignment is pretty simple */
      code_hunk * offset_ch = CCH(CCH(CHS("{\nint offset = "), local_ch), "\n"); 
      if(l_op > 0) {
        /* TODO */
      }
      
  push_var_in_context(lh);
  /* Make the code_hunk for us to return */
  code_hunk *ret = CCH(CHS("{\n"),lh->self_ch));
  lh->self_ch = NULL;
  ret = CCH(ret,CHS("\nthing *self=stack[sp];\nstack[sp--]=NULL\n;"));
  if(op[0] != '\0') {
    /* Not a pure = assignment */
    
  
}

static code_hunk * push_var_in_context(lexpr_hunk *lh) {
  /* Find the object being assigned */
  /* If there is no explicit self, we need to search for the variable in several places */
    /* First as a local var */
    fp_offset = get_var_fp_offset(lh->sym);
    if(fp_offset != 0) {
      /* Found! Push the local var */
      char local_str[100];
      sprintf(local_str, "stack[sp++]=stack[fp+%d];\n", fp_offset);
      CCH (ret, CHS(local_str));
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
        /* Now as a non-shy variable */
        char *assign_str = acme_malloc(n2+2);
        str2 = assign_str;
        strcpy(assign_str, name);
        *(str2++) = '=';
        *str2 = '\0';
        to_call = get_symbol(assign_name);
      }
    }
  }
}
    
code_hunk * dereference(lexpr_hunk *lh){
}
