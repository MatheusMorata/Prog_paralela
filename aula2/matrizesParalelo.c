#include <stdlib.h>
#include <stdio.h>
#include <omp.h> // Importando OPENMP

#define N 2 // Tamanho das matrizes

void preencher(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1; // Populando a matriz
        }
    }
}

// Multiplicar matrizes a e b em paralelo
void multiplicarMatrizesParalelo(int a[N][N], int b[N][N], int c[N][N]) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; 
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void main() {
    int a[N][N];
    int b[N][N];
    int c[N][N]; 

    preencher(a);
    preencher(b);

    multiplicarMatrizesParalelo(a, b, c);

}