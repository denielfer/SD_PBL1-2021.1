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
