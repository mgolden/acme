#ifndef RUNTIME_H
#define RUNTIME_H

#ifdef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn thing *stack;
extrn box_list *b_true, *b_false, *b_nil, *b_i, *b_f, *b_s, *b_sym;
extrn thing *t_true, *t_false, *t_nil;
extrn int sp;

#endif
