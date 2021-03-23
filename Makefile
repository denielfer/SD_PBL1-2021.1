compile: generateMatrices.c test.c
	gcc generateMatrices.c -o gerador
	gcc -pg test.c -o test -masm=intel
	./gerador
	./test
	gprof -b ./test>teste.txt
compile_todos: generateMatrices.c test.c main_final.c arquivo_de_testes_mutiplos.c
	gcc generateMatrices.c -o gerador
	gcc -pg test.c -o test -masm=intel
	gcc -pg main_final.c -o main_final -masm=intel
	gcc -pg arquivo_de_testes_mutiplos.c -o arquivo_de_testes_mutiplos -masm=intel
run: main_final gerador
	./gerador
	./main_final