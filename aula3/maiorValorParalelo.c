#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE 10000

void main(){
    int i, max, a[SIZE], valor[SIZE];

    for(i = 0; i < SIZE; i++){
        a[i] = rand();
    }

    for(i = 0; i <= SIZE; i++){
        valor[i] = a[0];
    }

    #pragma omp parallel private(i,max) 
    {
        int thread = omp_get_thread_num();
        for(i = 1; i < SIZE; i++){
            if(a[i] > valor[thread]){
                valor[thread] = a[i];
            }
        }
    }

    // Comparação final
    max = valor[0];
    for(i = 0; i <= SIZE; i++){
        if(max < valor[i]){
            max = valor[i];
        }
    }
    printf("max = %d\n", max);
}
