#include "compiler.h"

/* Insert a new code block in the (circular) list, */
/* between the tail and the head */
static code_hunk *insert_code_hunk(code_hunk *list, char *code) {
  code_hunk *new = GC_malloc(sizeof code_hunk);
  new->code = code;
  new->extra = 0;
  if(list == NULL) {
    new->prev = new->next = new;
  }
  else {
    code_hunk *last = list->prev;
    last->next = new;
    list->prev = new;
    new->prev = last;
    new->next = list;
  }
  return new;
}

/* The only difference between appending and prepending */
/* Is what becomes the new head */
/* Here, the old head is the head still */
code_hunk *append_code_hunk(code_hunk *list, char *code) {
  insert_code_hunk(list, code);
  return list;
}

/* Here the new head becomes the head */
code_hunk *prepend_code_hunk(code_hunk *list, char *code) {
  return insert_code_hunk(list, code);
}

/* Concatenate lists */
code_hunk *concatenate_code_hunk_lists(code_hunk *list1, code_hunk *list2) {
  if(code_hunk1 == NULL) {
    if(code_hunk1 == NULL) {
      return NULL;
    }
    return code_hunk2;
  }
  if(code_hunk2 == NULL) {
    return code_hunk1;
  }
  list1_end = list1->prev;
  list2_end = list2->prev;
  list1_end->next = list2;
  list2->prev = list1;
  list2_end->next = list1;
  list1->prev = list2;
  return list1;
}

/* This may not be necessary, but should make things easier */
/* for GC */
/* NOTE: Next insert a constant string into the code hunk chain */
void free_code_hunk(code_hunk *list) {
  if(list == NULL) {return;}
  /* Break the circle */
  list->prev->next = NULL;
  while(list != NULL) {
    code_hunk *old = list;
    GC_free(list->code);
    GC_free(list);
    list = old;
  }
}
