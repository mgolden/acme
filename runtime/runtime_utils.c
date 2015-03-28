#include "runtime_utils.h"

#define PUSH(x) \
  {stack_element *se = stack+(sp++); se->t.u.things_hash = x->u.things_hash; se->t.a_list = x->a_list;}

int is_true(void) {
  stack_element *se = stack+(--sp);
  ability_list *bl = se->t.a_list;
  int result;
  if(bl == b_nil || bl == b_false) {
    result = 0;
  }
  else {
    result = 1;
  }
  /* Make sure to clear the top of the stack */
  se->t.u.things_hash = NULL;
  se->t.a_list = NULL;
  return result;
}

void to_boolean(thing *t) {
  ability_list * bl = t->a_list;
  thing *val;
  if(bl == b_nil || bl == b_false) {
    val = t_false;
  }
  else {
    val = t_true;
  }
  PUSH(val);
}
