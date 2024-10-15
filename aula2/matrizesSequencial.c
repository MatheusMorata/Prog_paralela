/*
	Multiplicação de matrizes sequencial
*/

#include <stdlib.h>
#include <stdio.h>
#define N 3 // Tamanho das matrizes 

// Função para povoar as matrizes
void preencher(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1; // Populando as matrizes
        }
    }
}

// Função que multiplica as matrizes sequencialmente
void multiplicarMatrizes(int a[N][N], int b[N][N], int c[N][N]) {
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

    multiplicarMatrizes(a, b, c);

}