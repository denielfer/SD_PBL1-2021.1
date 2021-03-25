compile: gerador.c test.c
	gcc gerador.c -o gerador
	gcc test.c -o test -masm=intel
	./gerador
	./test
compile_todos: gerador.c test.c main_final.c arquivo_de_testes_mutiplos.c
	gcc gerador.c -o gerador
	gcc test.c -o test -masm=intel
	gcc main_final.c -o main_final -masm=intel
	gcc arquivo_de_testes_mutiplos.c -o arquivo_de_testes_mutiplos -masm=intel
run: gerador main_final
	./gerador
	./main_final
