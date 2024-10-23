/* 
    Cálculo do produto escalar em paralelo
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void main(){
    double sum = 0;
    double a[99999], b[99999];
    int n = 99999;
    int i;

    for(i = 0; i<n; i++){
        a[i] = i * 0.5;
        b[i] = i * 2;
    }

    double inicio = omp_get_wtime();
    #pragma omp parallel for private(i) reduction(+:sum)
        for(i = 1; i<n; i++){
            sum += a[i] * b[i];
        }
    double resultado = omp_get_wtime() - inicio; 

    printf("Soma = %.2f\n",sum);
    printf("Tempo = %.8f\n",resultado);
}
