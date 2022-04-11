#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define N 3

MPI_Status status;

double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N];


int main(int argc, char **argv)
{

    int numerodeprocessos, IDProcessos, Trabalhadores, Origem, Destino, Linhas, Deslocamento;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &IDProcessos);       // Dominio padrao
    MPI_Comm_size(MPI_COMM_WORLD, &numerodeprocessos); // Tamanho do mundo com size == numero de processos

    Trabalhadores = numerodeprocessos - 1; // Quantidade de trabalhadores sera um a menos que o numero de processos

    matrix_a[0][0] = 2;
    matrix_a[0][1] = 2;
    matrix_a[0][2] = 2;
    matrix_a[1][0] = 2;
    matrix_a[1][1] = 2;
    matrix_a[1][2] = 3;
    matrix_a[2][0] = 2;
    matrix_a[2][1] = 2;
    matrix_a[2][2] = 2;

    matrix_b[0][0] = 2;
    matrix_b[0][1] = 2;
    matrix_b[0][2] = 2;
    matrix_b[1][0] = 2;
    matrix_b[1][1] = 2;
    matrix_b[1][2] = 2;
    matrix_b[2][0] = 2;
    matrix_b[2][1] = 2;
    matrix_b[2][2] = 2;

    if (IDProcessos == 0)
    { // Esta no proceso Filho

        printf("\nMatrix A\n\n"); // Printa matriz A

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%.0f\t", matrix_a[i][j]);
            }
            printf("\n");
        }

        printf("\nMatrix B\n\n"); // Printa Matriz Filho

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%.0f\t", matrix_b[i][j]);
            }
            printf("\n");
        }

        Linhas = N / Trabalhadores; // Vai definir o numero de linhas da matriz A para enviar para os processos trabalhadores
        Deslocamento = 0;           // Variavel para determinar a posicao da linha que ira ser enviada para o processor trabalhador

        for (Destino = 1; Destino <= Trabalhadores; Destino++)
        { // Vai enviar as tarefas para os processos trabalhadores com a tag 1 para essa tarefa

            MPI_Send(&Deslocamento, 1, MPI_INT, Destino, 1, MPI_COMM_WORLD);
            MPI_Send(&Linhas, 1, MPI_INT, Destino, 1, MPI_COMM_WORLD);
            MPI_Send(&matrix_a[Deslocamento][0], Linhas * N, MPI_DOUBLE, Destino, 1, MPI_COMM_WORLD); // Manda a posicao que sera calculada no processo trabalhador
            MPI_Send(&matrix_b, N * N, MPI_DOUBLE, Destino, 1, MPI_COMM_WORLD);                       // Manda a matriz B para realizar o calculo

            Deslocamento = Deslocamento + Linhas; // Incrementar o deslocamento de acordo com o numero de linhas que foram mandadas para cada processo
        }

        for (int i = 1; i <= Trabalhadores; i++)
        { // Processo principal vai aguarda para que cada processo trabalhador termine sua execucao para assim comecar a executar!

            Origem = i;

            MPI_Recv(&Deslocamento, 1, MPI_INT, Origem, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&Linhas, 1, MPI_INT, Origem, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&matrix_c[Deslocamento][0], Linhas * N, MPI_DOUBLE, Origem, 2, MPI_COMM_WORLD, &status);
        }

        printf("\nResultado Matrix C = Matrix A * Matrix B:\n\n");

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                printf("%.0f\t", matrix_c[i][j]);
                
            printf("\n");
        }
        printf("\n");
    }

    if (IDProcessos > 0)
    { // Esta no proceso MAIN

        Origem = 0;

        MPI_Recv(&Deslocamento, 1, MPI_INT, Origem, 1, MPI_COMM_WORLD, &status); //Recebe o deslocamento enviado pelo pai
        MPI_Recv(&Linhas, 1, MPI_INT, Origem, 1, MPI_COMM_WORLD, &status); //recebe a linha enviada pelo pai
        MPI_Recv(&matrix_a, Linhas * N, MPI_DOUBLE, Origem, 1, MPI_COMM_WORLD, &status); //recebe a multiplicacao a ser feita
        MPI_Recv(&matrix_b, N * N, MPI_DOUBLE, Origem, 1, MPI_COMM_WORLD, &status);// recebe a matriz B

        for (int k = 0; k < N; k++) //Realiza multiplicacao
        {
            for (int i = 0; i < Linhas; i++)
            {
                matrix_c[i][k] = 0.0;
                for (int j = 0; j < N; j++)
                    matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
            }
        }

        MPI_Send(&Deslocamento, 1, MPI_INT, 0, 2, MPI_COMM_WORLD); //Envia o deslocamento atual
        MPI_Send(&Linhas, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix_c, Linhas * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD); //envia o resultado da matriz C
    }

    MPI_Finalize();

    return 0;

}
