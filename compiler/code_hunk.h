#ifndef CODE_HUNK_H
#define CODE_HUNK_H

typedef struct _code_hunk code_hunk;
typedef struct _code_list_entry code_list_entry;
typedef struct _lexpr_hunk lexpr_hunk;

/* The code hunks are a circular linked list */
struct _code_hunk {
  code_list_entry *list;
  int comexprs; /* comma delimited expressions */
  int locvars;  /* local vars declared */
  int offset;   /* Holds the fp offset when a local var lexpr, list is NULL */
};

struct _code_list_entry {
  code_list_entry *prev;
  code_list_entry *next;
  char *code;
};

/* A lexpr hunk is a code_hunk plus a symbol, used to return from lexpr
 * non-terminals before the lexpr is either assigned or dereferenced */

struct _lexpr_hunk {
  code_hunk *self_ch;
  symbol sym;
  code_hunk *subscript_ch;
};

code_hunk *make_code_hunk(char *code, int comexprs, int locvars, int offset);
code_hunk *concatenate_code_hunks(code_hunk *ch1, code_hunk *ch2);
void free_code_hunk(code_hunk *ch);
lexpr_hunk *make_lexpr_hunk(code_hunk *self_ch, const char *name, *subscript_ch);

/* concatenate code hunks */
#define CCH(ch1, ch2) \
  concatenate_code_hunks(ch1, ch2)

#define CH(str) \
  make_code_hunk(str, 0, 0, 0)

#define CHS(str) \
  make_code_hunk(acme_strdup(str), 0, 0, 0)
  
#endif
