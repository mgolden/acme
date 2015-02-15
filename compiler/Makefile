all: acme

acme: lex.yy.c y.tab.c
	cc lex.yy.c y.tab.c -DYYDEBUG -o acme

y.tab.c: acme.y ../acme.h ../uthash.h
	yacc -d acme.y

lex.yy.c: acme.l y.tab.c ../acme.h ../uthash.h
	lex acme.l
