#include <stdlib.h>
#include <stdio.h>
#include <omp.h> // Incluir OpenMP

#define N 2 // Definição da constante N

void preencher(int matriz[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 1; // Populando a matriz
        }
    }
}

int main() {
    int a[N][N];
    int b[N][N];
    int c[N][N]; 

    preencher(a);
    preencher(b);

    // Multiplicar matrizes a e b em paralelo
    #pragma omp parallel  // Diretriz para paralelismo
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            c[i][j] = 0; // Resetar o valor
            for (int k = 0; k < N; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Imprimir o resultado
    printf("Matriz resultante c:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}
