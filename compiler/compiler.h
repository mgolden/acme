#ifndef COMPILER_H
#define COMPILER_H

#include "../acme.h"

#include "misc.h"
#include "code_hunk.h"
#include "acme_error.h"
#include "label.h"
#include "variable_table.h"
#include "assign.h"
#include "function.h"

typedef struct _signature signature;

extrn symbol_definition *symbol_table;

void compiler_init(void);

#endif
