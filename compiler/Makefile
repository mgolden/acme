all: acme

acme: lex.yy.c y.tab.c
	cc lex.yy.c y.tab.c -DYYDEBUG -o acme

y.tab.c: acme.y
	yacc -d -g acme.y

lex.yy.c: acme.l y.tab.c
	lex acme.l
