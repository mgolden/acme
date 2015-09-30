#include "misc.h"

static code_hunk * emit_push(const char * s) {
  char * result = acme_malloc(100 + 2 * strlen(s));
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.things_hash = %s->u.things_hash; se->t.a_list = %s->a_list;}\n", s, s);
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
  sprintf(result, "{sp -= %d; memset(stack+sp, 0, %d*sizeof(stack_element));}", n, n);
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

code_hunk * get_self(void) {
  // self is stored at fp-3
  return(acme_strdup("stack[sp++] = stack[fp-3];\n"));
}

code_hunk * new_i_thing(acme_int i) {
  char * result = (char *) acme_malloc(100);
  sprintf("{stack_entry *se = stack+(sp++); se->t.u.i=%lld; se->t.a_list=b_i;}\n", (long long) i);
  return CH(result);
}

code_hunk * new_f_thing(acme_float f) {
  char * result = (char *) acme_malloc(100);
  sprintf("{stack_entry *se = stack+(sp++); se->t.u.f=%24.20e; se->t.a_list=b_f;}\n", f);
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
  sprintf("{stack_entry *se = stack+(sp++); se->t.u.s=acme_strdup(\"%s\"); se->t.a_list=b_s;}\n", s);
  return CH(result);
}

code_hunk * new_sym_thing_from_symbol(symbol sym) {
  char * result = (char *) acme_malloc(100);
  sprintf("{stack_entry *se = stack+(sp++); se->t.u.sym=%d; se->t.a_list=b_sym;}\n", sym);
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


code_hunk * emit_unop_call(const char *op, code_hunk *e){
  return CCH(CCH(CCH(get_empty_block(), e), new_sym_thing(op)), call_send(0));
}

code_hunk * emit_binop_call(code_hunk *e1, const char *op, code_hunk *e2){
  return CCH(CCH(CCH(CCH(e2, get_empty_block()), e1), new_sym_thing(op)), call_send(1));
}

code_hunk * get_empty_block(void) {
  
code_hunk * block_given(void) {
  return CHS("if(stack[fp-2].b.block_function_ptr == NULL) {get_false();} else {get_true();}\n");
}

static int open_ifs;

code_hunk * start_if(void) {
  open_ifs = 1;
  return CHS("if(is_true()) {\n");
}

code_hunk * start_elseif(void) {
  open_ifs++;
  return CHS("}\nelse {\nif(is_true()) {\n");
}

code_hunk * start_else(void) {
  return CHS("}\nelse {\n");
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

char * make_block_name(const char * file_name, int n) {
    int l = strlen(file_name);
    char * result = (char *) acme_malloc(10 + l);
    strcpy(result, file_name);
    sprintf(result+l, "%d", n);
    return result;
}
