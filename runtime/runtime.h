#ifndef RUNTIME_H
#define RUNTIME_H

#include "../acme.h"

#ifndef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn thing *stack;
extrn ability_list *b_buck, *b_nil, *b_true, *b_false, *b_i, *b_f, *b_s, *b_sym, *b_hash, *b_array;
extrn thing *t_buck, *t_nil, *t_true, *t_false;
extrn int sp;

#ifndef MAIN
extern ability_list *special_ability_lists[];
#else
ability_list *special_ability_lists[] = {b_buck, b_nil, b_true, b_false, b_i, b_f, b_s, b_sym, b_hash, b_array};
#endif

#endif
