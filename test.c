/**
 * Componente Curricular: MI - SISTEMAS DIGITAIS
 * Autor: Daniel Fernandes Campos, João Pedro Oliveria Rocha e João Pedro Rios Carvalho
 * Data: 22/03/2021
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor, 
 * tais como provindos de livros e apostilas, e páginas ou documentos 
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int* readMatrices();
int** multiplyMat(int** xMat, int** yMat, int dim);
int** multiplyMatASM(int** xMat, int** yMat, int dim);
void writeToFile(int** multMatm, int dim);

int main(void){
        printf("lendo\n");
        int* matArray = readMatrices();
        int** xMat = matArray[0];
        int** yMat = matArray[1];
        int dim = matArray[2];
        printf("mutiplicando matriz de %d\n",dim);
        clock_t start_t, end_t;
        double total;
        int** multMat;
	start_t = clock();
        multMat = multiplyMatASM(xMat, yMat, dim);
        end_t = clock();
        total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("tempo da mutiplicação em assembly em segundos:%lf\n", total);
	start_t = clock();
        multMat = multiplyMat(xMat, yMat, dim);
        end_t = clock();
        total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("tempo da mutiplicação em C em segundos:%lf\n", total);
        printf("Salvando o resultado\n");
        writeToFile(multMat, dim);
	free(matArray);
	free(multMat);
	for( int i=0;i<dim;i++ )
		free(xMat[i]);
        free(xMat);
	for( int i=0;i<dim;i++ )
		free(yMat[i]);
        free(yMat);
}

int** multiplyMatASM(int** xMat, int** yMat, int dim){
        //allocating resulting matrix
        int** multMat = (int**) malloc(sizeof(int*) * dim);
        for(int i=0; i<dim; i++) {
                multMat[i] = (int*) malloc(sizeof(int) * dim);
        }
        int *ly=0,
            *lx=0,
            x=0,
            y=0,
            r=0,
            i=0,
            j=0,
            k=0,
            temp=0;
//        printf("entrando no asm\n");
        //multiplying both matrices
	__asm__ __volatile__(
        "start_loop_1:\n" // definimos a branch de inicio do
        "       cmp %[i], %[dim]\n" // comparamos o valor de i com dim
        "       jge end_loop1\n" // se i for >= a dim pulamos para o final do loop1
        "       mov %[j],0\n"// zeramos o valor do de j para o procimo loop
        "       mov %[x],[%[xMat]+%[lx]*8]\n" // caregamos a referencia da linha de x que vamos mutiplicar pelas colunas de y na variavel "x"
        "       mov %[r],[%[rMat]+%[lx]*8]\n" // caregamos a referencia da linha de r na qual os resultados serao escritos
        "       inc %[lx]\n"//incrementamos o contador de linha de x, que consiste no mesmo contador de linha da resultante, lx segue os valores de lx
        "       inc %[i]\n"// incrementador o i, varaivel do loop
        "       start_loop2:\n"// delcaração da branch inical do loop 2
        "               cmp %[j],%[dim]\n" // compara o valor de j com o valor da dimenção
        "               jge start_loop_1\n"// se j for maior ou igual o loop2 é terminado e pulamos para o inicio do loop1
        "               mov %[k],0\n"// movemos k para 0, k=0, para realizarmos os procimo loop
        "               mov %[temp],0\n"// guardamos 0 em tempo pous mov nao estava aceitando valores imediatos
        "               mov %[ly],0\n"// zeramos o contador de linhas de y, pois ly segue os valores de k
        "               mov [%[r]+%[j]*4],%[temp]\n" // movemos o valor de temp, 0, para o elemento que sera o resultado dessa mutilicação, para limpar o lixo, e especificamente 0 pois é o neutro da soma
        "               start_loop3:\n"// criamos a branch inicial do loop3
        "                       cmp %[k],%[dim]\n"// comparamos o valor de k com dim
        "                       jge end_loop2\n"// se o valor for maior ou igual terminamos o loop e vamos para o conjunto de instruçoes do final do loop2
        "                       mov %[y], [%[yMat]+%[ly]*8]\n" // caregamos a linha que contem o elemento a ser usado na mutiplicação da matriz y
        "                       mov %[temp], [ %[y]+%[j]*4 ]\n" // movemos o elemento q sera usado da linha de y para temp
        "                       imul %[temp], [%[x]+%[k]*4]\n" // mutiplicamos o elemento que foi carregado em temp pelo elemento correspondente da matriz x
        "                       inc %[ly]\n" // incrementamos a linha de y
        "                       inc %[k]\n" // incrementamos k, contador do loop
        "                       add [%[r]+%[j]*4], %[temp]\n"// somamos o resultado da mutiplicação na posição correspondente na matriz resultado 
        "                       jmp start_loop3\n"// pulamos para o inicio do loop3 para repetirmos o processo ate sua condição verdadeira
        "               end_loop2:\n"// declaramos a branch do final do loop2
        "                       inc %[j]\n" // incrementamos j, isso é feito aqui pois j é usado no loop3 e caso essa incrementação fosse feito anteriormente a mutiplicação seria erada
        "                       jmp start_loop2\n"// pulamos para o inicio do loop1
        "end_loop1:"// branch para o final do loop1, que consiste no final da mutiplicação
        :[rMat] "+r" (multMat), [lx] "+r" (lx), [ly] "+r" (ly), [i] "+r" (i), [j] "+r" (j), [k] "+r" (k), [temp] "+r" (temp),
         [x] "+r" (x), [y] "+r" (y), [r] "+r" (r), [yMat] "+r" (yMat), [xMat] "+r" (xMat), [dim] "+r" (dim)
        );
//        printf("%d %d %d | %d %d | %d\n",i,j,k,lx,ly,temp);
        return multMat;
}

int* readMatrices(){
        int dim;
        FILE *fpointer = fopen("matrices.txt", "r");
        fscanf(fpointer,"%d\n\n",&dim);
        //allocating both matrices
        int** xMat = malloc(sizeof(int*) * dim);
        for(int i=0; i<dim; i++) {
                xMat[i] = malloc(sizeof(int) * dim);
        }
        int** yMat = malloc(sizeof(int*) * dim);
        for(int i=0; i<dim; i++) {
                yMat[i] = malloc(sizeof(int) * dim);
        }
        //reading from file
        for(int i=0; i<dim; i++){
                for(int j=0; j<dim; j++){
                        fscanf(fpointer, "%d", &xMat[i][j]);
                }
                fscanf(fpointer, "\n");
        }
//        printf("\n");
        fscanf(fpointer,"\n");
        for(int i=0; i<dim; i++){
                for(int j=0; j<dim; j++){
                        fscanf(fpointer, "%d", &yMat[i][j]);
                }
                fscanf(fpointer, "\n");
        }
//        printf("\n");
        fclose(fpointer);
        //allocating array to return matrices
        int* array = malloc(sizeof(int**) *3);
        array[0] = xMat;
        array[1] = yMat;
        array[2] = dim;
        return array;
}
int** multiplyMat(int** xMat, int** yMat, int dim){
        //allocating resulting matrix
        int** multMat = malloc(sizeof(int*) * dim);
        for(int i=0; i<dim; i++) {
                multMat[i] = malloc(sizeof(int) * dim);
        }
        //multiplying both matrices
        for(int i=0; i<dim; i++){
                //printf("%d ",i);
                for(int j=0; j<dim; j++){
                        multMat[i][j] = 0;
                        for(int k=0; k<dim; k++){
                                multMat[i][j] += xMat[i][k] * yMat[k][j];
                                //printf("%d %d %d\n",i,j,k);
                        }
                }
        }
//        printf("\n\n");
        return multMat;
}
void writeToFile(int** multMat, int dim){
        FILE *fpointer = fopen("result.txt", "w");
        for(int i = 0; i < dim; i++){
                for(int j = 0; j < dim; j++){
                        fprintf(fpointer, "%02d ", multMat[i][j]);
                }
                fprintf(fpointer,"\n");
//                printf("%d ",i);
        }
//        printf("\n");
        fclose(fpointer);
}
