#include <stdio.h>
#include <omp.h>

void main(){
    int fatorial = 1;
    int n = 10;

    // Captura o tempo de início
    double start_time = omp_get_wtime();
    for(int i = 2; i <= n; i++){
        fatorial *= i;
    }
    // Captura o tempo de fim
    double sequencial_time = omp_get_wtime() - start_time;


    printf("Fatorial de %d = %d\n",n,fatorial);
    printf("Tempo de execucao: %.8f segundos\n", sequencial_time);

}