build:
	bison -d calculator.y
	flex calculator.l
	gcc -o calculator calculator.tab.c lex.yy.c

run: build
	./calculator

clean:
	rm -f calculator y.tab.c y.tab.h lex.yy.c calculator.tab.c  calculator.tab.h