CFLAGS = -g -Wall -Wextra -std=C99 -pedantic-errors
CC     = gcc $(CFLAGS)

all: acme

# Note that lex requires _POSIX_SOURCE, as it references fileno
acme: lex.yy.c y.tab.c
	$(CC) -D_POSIX_SOURCE lex.yy.c y.tab.c -DYYDEBUG -o acme

y.tab.c: acme.y ../acme.h ../uthash.h
	yacc -d acme.y

lex.yy.c: acme.l y.tab.c ../acme.h ../uthash.h
	lex acme.l
