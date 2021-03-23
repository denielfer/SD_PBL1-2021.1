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

int** multiplyMat(int** xMat, int** yMat, int dim);
int** multiplyMatASM(int** xMat, int** yMat, int dim);
int* createMatrix(int dim);

// Esse arquivo tem um loop que vai gerando matrizes dos tamanhos 10,100,1000,10000 e a mutiplicação 
//dessas matrizes 5 vezes para cada função e guarda os tempos de execução em um arquivo txt 
int main(void){
        int num_testes=5;
        //cria um arquivo sem nada excrito, tambem limpa os dados salvos em testes anteriores
        FILE *fp = fopen("dados_dos_testes.txt","w");
        fclose(fp);
        for(int dim=10; dim<10001; dim=dim*10 ){
                double tempos[num_testes*2];
                printf("gerando matriz de 10 elementos por linha\n");
                int* matArray = createMatrix(dim);
                int** xMat = matArray[0];
                int** yMat = matArray[1];
                printf("mutiplicando matriz de %d elementos por linha\n",dim);
                printf("mutiplicação em assembly\n");
                for( int i=0; i<num_testes; i++){
                        clock_t start_t, end_t;
                        double total;
                        start_t = clock();
                        int** multMat = multiplyMatASM(xMat, yMat, dim);
                        end_t = clock();
                        total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                        printf("temp da mutiplicação %d foi de %lf s\n",i+1, total);
                        tempos[i]= total;
                }
                printf("mutiplicação em C\n");
                for( int i=0; i<num_testes; i++){
                        clock_t start_t, end_t;
                        double total;
                        start_t = clock();
                        int** multMat = multiplyMat(xMat, yMat, dim);
                        end_t = clock();
                        total = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                        printf("temp da mutiplicação %d foi de %lf s\n",i+1, total);
                        tempos[i+num_testes] = total;
                }
                free(matArray);
                for(int i=0;i<dim;i++){
                        free(xMat[i]);}
                free(xMat);
                for(int i=0;i<dim;i++){
                        free(yMat[i]);}
                free(yMat);
                FILE *fpa;
                fpa = fopen("dados_dos_testes.txt","a");
                fprintf(fpa,"matriz de tamanho %d \n",dim);
                for(int i=0; i<num_testes; i++){
                        fprintf(fpa,"Teste %d codigo em assembly: %lf s\n",i+1,tempos[i]);}
                for(int i=0; i<num_testes; i++){
                        fprintf(fpa,"Teste %d codigo em C: %lf s\n",i+1,tempos[i+num_testes]);}
                fclose(fpa);
        }
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
//        printf("entrando no asm\n");
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
//        printf("%d %d %d | %d %d | %d\n",i,j,k,lx,ly,temp);
        return multMat;
}

int* createMatrix(int dim){
    //get matrix dimension
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
    int* array = malloc(sizeof(int**) * 2);
    array[0] = xMat;
    array[1] = yMat;
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
//                printf("%d ",i);
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