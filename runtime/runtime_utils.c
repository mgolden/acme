#include "runtime_utils.h"

#define PUSH(x) \
  {thing *t = stack+(sp++); t->u.things_hash = x->u.things_hash; t->ability_list = x->ability_list;}

int is_true() {
  thing *t = stack + (--sp);
  ability_list *bl = t->ability_list;
  int result;
  if(bl == b_nil || bl == b_false) {
    result = 0;
  }
  else {
    result = 1;
  }
  /* Make sure to clear the top of the stack */
  t->u.things_hash = t->ability_list = NULL;
  return result;
}

void to_boolean(thing *t) {
  ability_list * bl = t->ability_list;
  thing *val;
  if(bl == b_nil || bl == b_false) {
    val = t_false;
  }
  else {
    val = t_true;
  }
  PUSH(val);
}

#endif
