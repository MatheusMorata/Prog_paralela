#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void main() {
    int i;
    int n = 170;
    double fac = 1;

    // Captura o tempo de início
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(*:fac)
    for (i = 2; i <= n; i++) {
        fac *= i;
    }

    // Captura o tempo de fim
    double sequencial_time = omp_get_wtime() - start_time;

    printf("Fatorial de %d = %.0f", n, fac);
    printf("\nTempo de execucao: %.8f\n", sequencial_time);
}
