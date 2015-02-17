#include "compiler.h"

/* Note: throughout, we are using GC_free, which may not be necessary
 * but should make things easier for GC */

code_hunk *make_code_hunk(char *code, int comexprs, int locvars, int offset) {
  code_hunk *new = (code_hunk *) GC_malloc(sizeof(code_hunk));
  new->comexprs = comexprs;
  new->locvars = locvars;
  new->offset = offset;
  code_list_entry *new_s = (code_list_entry *) GC_malloc(sizeof(code_list_entry));
  new_s->prev = new_s;
  new_s->next = new_s;
  new_s->code = code;
  return new;
}


/* Append the second code hunk onto the first */
code_hunk *concatenate_code_hunks(code_hunk *ch1, code_hunk *ch2) {
  if(ch1 == NULL) {
    if(ch2 == NULL) {
      return NULL;
    }
    return ch2;
  }
  if(ch2 == NULL) {
    return ch1;
  }
  
  code_list_entry *list1_end = ch1->list->prev;
  code_list_entry *list2_end = ch2->list->prev;
  list1_end->next = ch2->list;
  ch2->list->prev = list1_end;
  list2_end->next = ch1->list;
  ch1->list->prev = list2_end;

  ch1->comexprs += ch2->comexprs;
  ch1->locvars += ch2->locvars;
  if(ch1->offset != 0 || ch2->offset != 0) {
    fprintf(stderr, "FATAL ERROR: concatenating code_hunks with offset\n");
    exit(1);
  }

  ch2->list = NULL;
  GC_free(ch2);
  
  return ch1;
}

/* NOTE: Never insert a constant string into the code hunk chain */
void free_code_hunk(code_hunk *ch) {
  if(ch == NULL) {return;}
  /* Break the circle */
  code_list_entry *list = ch->list;
  list->prev->next = NULL;
  while(list != NULL) {
    code_list_entry *next = list->next;
    list->next = list->prev = NULL;
    GC_free(list->code);
    GC_free(list);
    list = next;
  }
  GC_free(ch);
}
