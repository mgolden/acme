#ifndef RUNTIME_UTILS_H
#define RUNTIME_UTILS_H

#include "runtime.h"

/* return 0 if the (popped) top of the stack is false or nil, 1 otherwise */
int is_true();

/* Push true/false on the stack depending on value of t */
void to_boolean(thing *t);

#endif
