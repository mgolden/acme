#include "compiler.h"

static char *label_stack[MAX_LABEL_DEPTH] = {0};
static int label_stack_pointer = 0;

void label_init(void) {
  /* Do this in case we need to reinitialize */
  memset(label_stack, 0, sizeof(label_stack));
  label_stack[0] = "$";
  label_stack_pointer = 0;
}

const char *current_label(void) {
  return label_stack[label_stack_pointer];
}

void push_label(const char *label) {
  label_stack_pointer++;
  if(label_stack_pointer >= MAX_LABEL_DEPTH) {
    e_fatal("label stack too deep!");
  }
  label_stack[label_stack_pointer] = acme_strdup(label);
}

void pop_label(void) {
  if(label_stack_pointer<1) {
    e_fatal("Attempting to pop outer label '$' from label stack");
  }
  acme_free(label_stack[label_stack_pointer]);
  label_stack[label_stack_pointer] = NULL;
  label_stack_pointer--;
}

/* Return the current label in dotted notation from the start of the stack */
/* Note: must be freed by caller! */
char *get_dotted_labels(void) {
  int n = 0;
  for(int i = 0; i<=label_stack_pointer; i++) {
    n += strlen(label_stack[i]);
  }
  char * result = (char *) acme_malloc(n + label_stack_pointer + 1); /* 1 extra */
  char * p = result;
  for(int i = 0; i<=label_stack_pointer; i++) {
    char *q = label_stack[i];
    while(q != '\0') {*(p++) = *(q++);}
    *(p++) = '.';
  }
  *(--p) = '\0'; /* Remove the last . */
}
