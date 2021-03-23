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
        int** multMat = multiplyMatASM(xMat, yMat, dim);
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
        //multiplying both matrices
        int *ly=0,
            *lx=0,
            x=0,
            y=0,
            r=0,
            i=0,
            j=0,
            k=0,
            temp=0;
        printf("entrando no asm\n");
        __asm__ __volatile__(
        "start_loop_1:\n"
        "       cmp %[i], %[dim]\n"
        "       jge end_loop1\n"
        "       mov %[j],0\n"
        "       mov %[x],[%[xMat]+%[lx]*8]\n"
        "       mov %[r],[%[rMat]+%[lx]*8]\n"
        "       inc %[lx]\n"
        "       inc %[i]\n"
        "       start_loop2:\n"
        "               cmp %[j],%[dim]\n"
        "               jge start_loop_1\n"
        "               mov %[k],0\n"
        "               mov %[temp],0\n"
        "               mov %[ly],0\n"
        "               mov [%[r]+%[j]*4],%[temp]\n"
        "               start_loop3:\n"
        "                       cmp %[k],%[dim]\n"
        "                       jge end_loop2\n"
        "                       mov %[y], [%[yMat]+%[ly]*8]\n"
        "                       mov %[temp], [ %[y]+%[j]*4 ]\n"
        "                       imul %[temp], [%[x]+%[k]*4]\n"
        "                       inc %[ly]\n"
        "                       inc %[k]\n"
        "                       add [%[r]+%[j]*4], %[temp]\n"
        "                       jmp start_loop3\n"
        "               end_loop2:\n"
        "                       inc %[j]\n"
        "                       jmp start_loop2\n"
        "end_loop1:"
        :[rMat] "+r" (multMat), [lx] "+r" (lx), [ly] "+r" (ly), [i] "+r" (i), [j] "+r" (j), [k] "+r" (k), [temp] "+r" (temp),
         [x] "+r" (x), [y] "+r" (y), [r] "+r" (r), [yMat] "+r" (yMat), [xMat] "+r" (xMat), [dim] "+r" (dim)
        );
        printf("%d %d %d | %d %d | %d\n",i,j,k,lx,ly,temp);
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
        printf("\n");
        fscanf(fpointer,"\n");
        for(int i=0; i<dim; i++){
                for(int j=0; j<dim; j++){
                        fscanf(fpointer, "%d", &yMat[i][j]);
                }
                fscanf(fpointer, "\n");
        }
        printf("\n");
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
                printf("%d ",i);
                for(int j=0; j<dim; j++){
                        multMat[i][j] = 0;
                        for(int k=0; k<dim; k++){
                                multMat[i][j] += xMat[i][k] * yMat[k][j];
                                //printf("%d %d %d\n",i,j,k);
                        }
                }
        }
        printf("\n\n");
        return multMat;
}
void writeToFile(int** multMat, int dim){
        FILE *fpointer = fopen("result.txt", "w");
        for(int i = 0; i < dim; i++){
                for(int j = 0; j < dim; j++){
                        fprintf(fpointer, "%02d ", multMat[i][j]);
                }
                fprintf(fpointer,"\n");
                printf("%d ",i);
        }
        printf("\n");
        fclose(fpointer);
}