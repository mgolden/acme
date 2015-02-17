#include "compiler.h"

code_hunk * pop_stack(int n) {
  if(n<0) {
    e_fatal("Popping a negative number of elements from the stack");
  }
  else if(n==0} {
    return NULL;
  }
  result = (char *) GC_malloc(150);
  sprintf(result, "{sp -= %d; memset(stack+sp, 0, %d*sizeof(thing));}", n, n);
  return CH(result);
}

code_hunk * get_nil {
  return CH(strdup("get_nil(sp)\n");
}

symbol get_sym(const char *s) {
  return 12346;
}

code_hunk * new_i_thing(acme_int i) {
  result = (char *) GC_malloc(150);
  sprintf("{thing *t = stack+sp; t->thing_type=I_thing_type; t->u->i=%d;}\n", i);
  return CH(result);
}

code_hunk * new_f_thing(acme_float f) {
  result = (char *) GC_malloc(150);
  sprintf("{thing *t = stack+sp; t->thing_type=I_thing_type; t->u->i=%d;}\n", f);
  return CH(result);
}

code_hunk * new_s_thing(const char *s) {
/*
 *  int l = 2*strlen(s)+1;
  ss = (char *) GC_malloc(l);
  char *p = ss;
  char *q = s;
  do {
    if(*q == '"') {
      *(p++) = '\\';
    }
    *(p++) = *(q++);
  } (*q != '\0');
*/
  result = (char *) GC_malloc(150 + l);
  sprintf("{thing *t = stack+sp; t->thing_type=S_thing_type; t->u->s=strdup(\"%s\");}\n", s);
  return CH(result);
}

code_hunk * new_sym_thing_from_sym(symbol sym) {
  result = (char *) GC_malloc(150);
  sprintf("{thing *t = stack+sp; t->thing_type=Sym_thing_type; t->u->sym=%d;}\n", sym);
  return CH(result);
}

code_hunk * new_sym_thing(const char *s) {
  symbol sym = get_sym(s);
  return new_sym_thing_from_sym(sym);
}
