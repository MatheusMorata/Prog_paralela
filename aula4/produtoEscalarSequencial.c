#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

/*
Calculando produto escalar de forma sequencial
*/
int main() {
    double sum = 0;
    double a[100000], b[100000];
    int n = 100000;
    struct timeval start, end;

  
    for (int i = 0; i < n; i++) {
        a[i] = i * 0.5;
        b[i] = i * 2;
    }

    // Captura o tempo de início
    gettimeofday(&start, NULL);

    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }

    // Captura o tempo de fim
    gettimeofday(&end, NULL);

    // Calcula a diferença em microsegundos
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;


    printf("Soma = %.2f\n", sum);
    printf("Tempo de execução: %.8f segundos\n", elapsed);

    return 0;
}
