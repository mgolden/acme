#ifndef CODE_HUNK_H
#define CODE_HUNK_H

/* The code hunks are a circular linked list */
typedef struct _code_hunk {
  code_list_entry *list;
  int comexprs; /* comma delimited expressions */
  int locvars;  /* local vars declared */
  int offset;   /* Holds the fp offset when a local var lexpr, list is NULL */
} code_hunk;


typedef struct _code_list_entry {
  code_string *prev;
  code_string *next;
  char *code;
} code_list_entry;

code_hunk *make_code_hunk(char *code, int comexprs, int locvars, int offset);
code_hunk *concatenate_code_hunks(code_hunk *ch1, code_hunk *ch2);
void free_code_hunk(code_hunk *ch);

/* concatenate code hunks */
#define CCH(ch1, ch2) \
  concatenate_code_hunks(ch1, ch2)

#endif
