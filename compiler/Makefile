CFLAGS = -g -Wall -Wextra -std=c99 -pedantic-errors
CC     = gcc $(CFLAGS)

all: acme

# Note that lex requires _POSIX_SOURCE, as it references fileno
acme: lex.yy.o y.tab.o acme_error.o assign.o code_hunk.o compiler_init.o function.o label.o misc.o variable_table.o ../get_symbol.o
	$(CC) -o acme lex.yy.o y.tab.o acme_error.o assign.o code_hunk.o compiler_init.o function.o label.o misc.o variable_table.o ../get_symbol.o -lgc

y.tab.o: y.tab.c
	$(CC) -c -D_POSIX_SOURCE -DYYDEBUG y.tab.c

y.tab.c: acme.y ../acme.h
	yacc -v -d acme.y

lex.yy.c: acme.l
	lex acme.l

lex.yy.o: lex.yy.c y.tab.c ../acme.h
	$(CC) -c -D_POSIX_SOURCE -DYYDEBUG lex.yy.c

compiler.h: acme_error.h assign.h code_hunk.h function.h label.h misc.h variable_table.h
	touch compiler.h

acme_error.o: acme_error.c acme_error.h lex.yy.c # really lex.yy.h
	$(CC) -c acme_error.c

assign.o: assign.c compiler.h
	$(CC) -c assign.c

code_hunk.o: code_hunk.c compiler.h
	$(CC) -c code_hunk.c

compiler_init.o: compiler_init.c compiler.h
	$(CC) -c compiler_init.c

function.o: function.c compiler.h
	$(CC) -c function.c

label.o: label.c compiler.h
	$(CC) -c label.c

misc.o: misc.c compiler.h
	$(CC) -c misc.c

variable_table.o: variable_table.c compiler.h lex.yy.c # really lex.yy.h
	$(CC) -c variable_table.c

clean:
	/bin/rm -f *.o lex.yy.* y.tab.*
