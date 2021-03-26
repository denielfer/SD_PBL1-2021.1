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

int* createMatrix();
void writeToFile(int** xMat, int** yMat, int dim);
int main(void){
    int* matArray = createMatrix();
    int** xMat = matArray[0];
    int** yMat = matArray[1];
    int dim = matArray[2];
    free(matArray);
    writeToFile(xMat, yMat, dim);
    free (xMat);
    free (yMat);
    return 0;
}

int* createMatrix(){
    //get matrix dimension
    int dim;
    printf("type the dimension of the vector: ");
    scanf("%d",&dim);
    printf("%d\n",dim);
    //memory allocation for the matrix
    int** xMat = malloc(sizeof(int*) * dim);
    for(int i=0; i<dim; i++) {
        xMat[i] = malloc(sizeof(int) * dim);
    }
    int** yMat = malloc(sizeof(int*) * dim);
    for(int i=0; i<dim; i++) {
        yMat[i] = malloc(sizeof(int) * dim);
    }
    //filling matrix
    srand(time(NULL));
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            xMat[i][j] = rand();
            yMat[i][j] = rand();
        }
    }
    //allocating array to return
    int* array = malloc(sizeof(int**) * 3);
    array[0] = xMat;
    array[1] = yMat;
    array[2] = dim;
    return array;
}
void writeToFile(int** xMat, int** yMat, int dim){
    FILE *fpointer = fopen("matrices.txt", "w");
    fprintf(fpointer, "%d\n\n", dim);
    printf("Gerando primeira matriz\n");
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
          fprintf(fpointer, "%02d ", xMat[i][j]);
        }
        fprintf(fpointer,"\n");
//        printf("%d ",i);
    }
    printf("Gerenado segunda matriz\n");
    fprintf(fpointer,"\n");
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            fprintf(fpointer, "%02d ", yMat[i][j]);
        }
        fprintf(fpointer,"\n");
//        printf("%d ",i);
    }
//    printf("\n\n\n");
    fclose(fpointer);

    //freeing the arrays
    for(int i=0; i<dim; i++){
        free(xMat[i]);
        free(yMat[i]);
    }
}
