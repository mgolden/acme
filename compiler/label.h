#ifndef LABEL_H
#define LABEL_H

/* Usual C things */
#include <stdio.h>
#include <string.h>

/* Garbage collector */
#include <gc.h>

#define MAX_LABEL_DEPTH 100

void label_init(void);
const char *current_label(void);
void push_label(const char *label);
void pop_label(void);
/* Return the current label in dotted notation from the start of the stack */
/* Note: must be freed by caller! */
char *get_dotted_labels(void);

#endif
