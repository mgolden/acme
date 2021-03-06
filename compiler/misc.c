#include "misc.h"

static code_hunk * emit_push(const char * s) {
  char * result = acme_malloc(100 + 2 * strlen(s));
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.things_hash = %s->u.things_hash; se->t.a_list = %s->a_list;}\n", s, s);
  return CH(result);
}

code_hunk * push_stack(int n) {
  if(n<0) {
    e_fatal("Pushing a negative number of elements on the stack");
  }
  else if(n==0) {
    return NULL;
  }
  char * result = (char *) acme_malloc(50);
  sprintf(result, "{sp += %d;}", n);
  return CH(result);
}

code_hunk * pop_stack(int n) {
  if(n<0) {
    e_fatal("Popping a negative number of elements from the stack");
  }
  else if(n==0) {
    return NULL;
  }
  char * result = (char *) acme_malloc(100);
  sprintf(result, "{sp -= %d; memset(stack+sp, 0, %d*sizeof(stack_element));}", n, n);
  return CH(result);
}

code_hunk * push_buck(void) {
  return emit_push("t_buck");
}

code_hunk * push_nil(void) {
  return emit_push("t_nil");
}

code_hunk * push_true(void) {
  return emit_push("t_true");
}

code_hunk * push_false(void) {
  return emit_push("t_false");
}

code_hunk * push_self(void) {
  // self is stored at fp-3
  return(CHS("stack[sp++] = stack[fp-3];\n"));
}

code_hunk * push_i_thing(acme_int i) {
  char * result = (char *) acme_malloc(100);
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.i=%lld; se->t.a_list=b_i;}\n", (long long) i);
  return CH(result);
}

code_hunk * push_f_thing(acme_float f) {
  char * result = (char *) acme_malloc(100);
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.f=%24.20e; se->t.a_list=b_f;}\n", f);
  return CH(result);
}

code_hunk * push_s_thing(const char *s) {
/*
 *  size_t l = 2*strlen(s)+1;
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
  size_t l = strlen(s);
  char * result = (char *) acme_malloc(100 + l);
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.s=acme_strdup(\"%s\"); se->t.a_list=b_s;}\n", s);
  return CH(result);
}

code_hunk * push_sym_thing_from_symbol(symbol sym) {
  char * result = (char *) acme_malloc(100);
  sprintf(result, "{stack_entry *se = stack+(sp++); se->t.u.sym=%" PRIu64 "; se->t.a_list=b_sym;}\n", sym);
  return CH(result);
}

code_hunk * push_sym_thing(const char *s) {
  symbol sym = get_symbol(s);
  return push_sym_thing_from_symbol(sym);
}

code_hunk * push_array_thing(int i) {
  char result[50];
  sprintf(result, "push_array_thing(%d);\n", i);
  return CHS(result);
}

code_hunk * push_hash_thing(int i) {
  char result[50];
  sprintf(result, "push_hash_thing(%d);\n", i);
  return CHS(result);
}


code_hunk * emit_unop_call(const char *op, code_hunk *e){
  return CCH4(push_empty_block(), e, push_sym_thing(op), call_send(0));
}

code_hunk * emit_binop_call(code_hunk *e1, const char *op, code_hunk *e2){
  return CCH5(e2, push_empty_block(), e1, push_sym_thing(op), call_send(1));
}

code_hunk * push_empty_block(void) {
  return CHS("stack[sp++] = NULL;\n");
}

code_hunk * block_given(void) {
  return CHS("if(stack[fp-2].b.block_function_ptr == NULL) {push_false();} else {push_true();}\n");
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
  for(int i = 0; i<open_ifs; i++) {*(p++) = '}';}
  *(p++) = '\n';
  *(p++) = '\0';
  open_ifs = 0;
  return CH(result);
}

code_hunk * call_send(int i) {
  char result[50];
  sprintf(result, "call_send(%d);\n", i);
  return CHS(result);
}

code_hunk * clone(int i) {
  char result[50];
  sprintf(result, "clone(%d);\n", i);
  return CHS(result);
}

char * make_block_name(const char * file_name, int n) {
    size_t l = strlen(file_name);
    char * result = (char *) acme_malloc(10 + l);
    strcpy(result, file_name);
    sprintf(result+l, "%d", n);
    return result;
}
