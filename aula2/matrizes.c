#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define N 4 // Tamanho das matrizes 

// Função para povoar as matrizes
void preencher(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1; // Populando as matrizes
        }
    }
}

// Função que multiplica as matrizes sequencialmente
void multiplicarMatrizesSequencial(int a[N][N], int b[N][N], int c[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; 
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Função que multiplica matrizes a e b em paralelo
void multiplicarMatrizesParalelo(int a[N][N], int b[N][N], int c[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; // Resetar o valor
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}


int main() {
    int a[N][N];
    int b[N][N];
    int c[N][N]; 
    clock_t inicio, fim;
    double resultado;

    preencher(a);
    preencher(b);

    inicio = clock();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; 
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    fim = clock();

    resultado = fim - inicio;
    printf("Tempo de execucao sequencial: %.f",(double)resultado/ CLOCKS_PER_SEC);
    return 0;
}