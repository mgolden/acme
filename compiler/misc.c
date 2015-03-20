#include "misc.h"

static code_hunk * emit_push(const char * s) {
  char * result = acme_malloc(100 + 2 * strlen(s));
  sprintf(result, "{stack[sp].u.things_hash = %s->u.things_hash; stack[sp++].list = %s->list;}\n", s, s);
  return CH(result);
}

code_hunk * push_stack(int n) {
  if(n<0) {
    e_fatal("Pushing a negative number of elements from the stack");
  }
  else if(n==0} {
    return NULL;
  }
  result = (char *) acme_malloc(50);
  sprintf(result, "{sp += %d;}", n);
  return CH(result);
}

code_hunk * pop_stack(int n) {
  if(n<0) {
    e_fatal("Popping a negative number of elements from the stack");
  }
  else if(n==0} {
    return NULL;
  }
  char * result = (char *) acme_malloc(100);
  sprintf(result, "{sp -= %d; memset(stack+sp, 0, %d*sizeof(thing));}", n, n);
  return CH(result);
}

code_hunk * get_buck(void) {
  return emit_push("t_buck");
}

code_hunk * get_nil(void) {
  return emit_push("t_nil");
}

code_hunk * get_true(void) {
  return emit_push("t_true");
}

code_hunk * get_false(void) {
  return emit_push("t_false");
}

code_hunk * new_i_thing(acme_int i) {
  char * result = (char *) acme_malloc(100);
  sprintf("{thing *t = stack+(sp++); t->u.i=%lld; t->ability_list=b_i;}\n", (long long) i);
  return CH(result);
}

code_hunk * new_f_thing(acme_float f) {
  char * result = (char *) acme_malloc(100);
  sprintf("{thing *t = stack+(sp++); t->u.f=%24.20e; t->ability_list=b_f;}\n", f);
  return CH(result);
}

code_hunk * new_s_thing(const char *s) {
/*
 *  int l = 2*strlen(s)+1;
  ss = (char *) acme_malloc(l);
  char *p = ss;
  char *q = s;
  do {
    if(*q == '"') {
      *(p++) = '\\';
    }
    *(p++) = *(q++);
  } (*q != '\0');
*/
  l = strlen(s);
  result = (char *) acme_malloc(100 + l);
  sprintf("{thing *t = stack+(sp++); t->u.s=acme_strdup(\"%s\"); t->ability_list=b_s;}\n", s);
  return CH(result);
}

code_hunk * new_sym_thing_from_symbol(symbol sym) {
  char * result = (char *) acme_malloc(100);
  sprintf("{thing *t = stack+(sp++); t->u.sym=%d; t->ability_list=b_sym;}\n", sym);
  return CH(result);
}

code_hunk * new_sym_thing(const char *s) {
  symbol sym = get_symbol(s);
  return new_sym_thing_from_symbol(sym);
}

code_hunk * new_array_thing(int i) {
  char * result = (char *) acme_malloc(50);
  sprintf(result, "new_array_thing(%d);\n", i);
  return CH(result);
}

code_hunk * new_hash_thing(int i) {
  char * result = (char *) acme_malloc(50);
  sprintf(result, "new_hash_thing(%d);\n", i);
  return CH(result);
}

code_hunk * block_given(void) {
  return CH(acme_strdup("to_boolean(stack+fp-2);\n"));
}

static int open_ifs;

code_hunk * start_if(void) {
  open_ifs = 1;
  return CH(acme_strdup("if(is_true()) {\n");
}

code_hunk * start_elseif(void) {
  open_ifs++;
  return CH(acme_strdup("}\nelse {\nif(is_true(--sp)) {\n");
}

code_hunk * start_else(void) {
  return CH(acme_strdup("}\nelse {\n");
}

code_hunk * end_if(void) {
  char * result = (char *) acme_malloc(10+open_ifs);
  char * p = result;
  for(int i = 0; i<open_ifs; i++) {*(p++) = '}'}
  *(p++) = '\n';
  *(p++) = '\0';
  open_ifs = 0;
  return CH(result);
}

code_hunk * call_send(int i) {
  char * result = (char *) acme_malloc(50);
  sprintf(result, "call_send(%d);\n", i);
  return CH(result);
}

code_hunk * clone(int i) {
  char * result = (char *) acme_malloc(50);
  sprintf(result, "clone(%d);\n", i);
  return CH(result);
}

symbol get_lexpr_sym(const char *s) {
  char *c = (char *) acme_malloc(strlen(s)+2);
  char *p = c;
  char *q = s;
  while(*q!='\0') {
    *(p++) = *(q++);
  }
  *(p++) = '=';
  *(p++) = '\0';
  symbol sym = get_symbol(c);
  acme_free(c);
  return sym;
}

code_hunk * new_lexpr_symbol_thing(const char *s) {
  return new_sym_thing_from_symbol(get_lexpr_symbol(s));
}
