compile: gerador.c test.c
	gcc gerador.c -o gerador
	gcc -pg test.c -o test -masm=intel
	./gerador
	./test
	gprof -b ./test>teste.txt
compile_todos: gerador.c test.c main_final.c arquivo_de_testes_mutiplos.c
	gcc gerador.c -o gerador
	gcc -pg test.c -o test -masm=intel
	gcc -pg main_final.c -o main_final -masm=intel
	gcc -pg arquivo_de_testes_mutiplos.c -o arquivo_de_testes_mutiplos -masm=intel
run: gerador main_final
	./gerador
	./main_final
