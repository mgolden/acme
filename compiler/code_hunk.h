#ifndef CODE_HUNK_H
#define CODE_HUNK_H

/* The code hunks are a circular linked list */
typedef struct _code_hunk {
  code_hunk *prev;
  code_hunk *next;
  char *hunk;
  int commas; /* comma delimited expressions */
  int vars;   /* local vars declared */
} code_hunk;

code_hunk *append_code_hunk(code_hunk *list, char *code);
code_hunk *prepend_code_hunk(code_hunk *list, char *code);
code_hunk *concatenate_code_hunk_lists(code_hunk *list1, code_hunk *list2);
void free_code_hunk(code_hunk *list);

#define CH(var, val) \
  code_hunk *var = GC_malloc(sizeof code_hunk);\
  var->hunk = val;

#define CCH(list1, list2) \
  code_hunk *concatenate_code_hunk_lists(list1, list2)
  
#endif
