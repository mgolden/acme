#ifndef GET_SYM_H
#define GET_SYM_H

/* From qlibc */
uint64_t qhashfnv1_64(const void * data, size_t nbytes);

symbol get_sym(const char *s);
symbol get_no_eq_sym(symbol sym);

#endif
