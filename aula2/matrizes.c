#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <omp.h> // Importando OPENMP

// Função para multiplicar duas matrizes de forma sequencial
void multiplicacao_sequencial(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Função para multiplicar duas matrizes de forma paralela com OPENMP
void multiplicacao_parallel(int **A, int **B, int **C, int N) {
    #pragma omp parallel for collapse(2) num_threads(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Função para alocar memória para uma matriz NxN
int** alocar_matriz(int N) {
    int **matriz = (int**) malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        matriz[i] = (int*) malloc(N * sizeof(int));
    }
    return matriz;
}

// Função para liberar a memória alocada para uma matriz NxN
void liberar_matriz(int **matriz, int N) {
    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função para inicializar uma matriz com valor 1
void inicializar_matriz(int **matriz, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1;
        }
    }
}

void main() {
    int N;
    printf("Digite o tamanho da matriz: ");
    scanf("%d", &N);

    int **A = alocar_matriz(N);
    int **B = alocar_matriz(N);
    int **C = alocar_matriz(N);

    // Inicializa matrizes A e B 
    srand(time(NULL));
    inicializar_matriz(A, N);
    inicializar_matriz(B, N);

    // Medindo tempo de execução da multiplicação sequencial
    double start_time = omp_get_wtime();
    multiplicacao_sequencial(A, B, C, N);
    double sequencial_time = omp_get_wtime() - start_time;

    // Medindo tempo de execução da multiplicação paralela
    start_time = omp_get_wtime();
    multiplicacao_parallel(A, B, C, N);
    double parallel_time = omp_get_wtime() - start_time;

    // Calculando o speedup
    double speedup = sequencial_time / parallel_time;

    printf("Tempo sequencial: %f segundos\n", sequencial_time);
    printf("Tempo paralelo: %f segundos\n", parallel_time);
    printf("Speedup: %f\n", speedup);

    // Liberando memória
    liberar_matriz(A, N);
    liberar_matriz(B, N);
    liberar_matriz(C, N);


}