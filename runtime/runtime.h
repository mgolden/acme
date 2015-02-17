#ifndef RUNTIME_H
#define RUNTIME_H

#ifdef MAIN
#define extrn extern
#else
#define extrn
#endif

extrn thing *stack;
extrn box_list *b_true, *b_false, *b_nil, *b_i, *b_f, *b_s, *b_sym, *b_hash, *b_array;
extrn thing *t_true, *t_false, *t_nil;
extrn int sp;

#ifdef MAIN
extern box_list *special_boxes[];
else
box_list *special_boxes[] = {b_true, b_false, b_nil, b_i, b_f, b_s, b_sym, b_hash, b_array};
#endif

#endif
