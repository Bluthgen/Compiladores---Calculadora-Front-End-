all: calc

calculadora.tab.c calculadora.tab.h:	calculadora.y
	bison -d calculadora.y --locations

lex.yy.c: calculadora.l calculadora.tab.h
	flex calculadora.l

calc: lex.yy.c calculadora.tab.c calculadora.tab.h
	gcc -o calc calculadora.tab.c lex.yy.c asa.c simbolos.c executa.c calculadora.c -lfl -lm

clean:
	rm calc calculadora.tab.c lex.yy.c calculadora.tab.h