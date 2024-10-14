/*
	Multiplicação de matrizes sequencial
*/

#include <stdlib.h>
#include <stdio.h>
#define N 1000000000 // Tamanho das matrizes 

void preencher(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1; // Populando as matrizes
        }
    }
}

void main() {
    int a[N][N];
    int b[N][N];
    int c[N][N]; 

    preencher(a);
    preencher(b);

    // Mutliplicando as matrizes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; // Reset the value
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
