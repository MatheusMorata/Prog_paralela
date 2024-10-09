/* Calculando PI por integração em sequencial*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void main(int agrc, char *argv[]){
    int n, num_threads, id;
    double mypi, h, sum, x, a;

    n = atoi(argv[1]);

    h = 1.0/(double)n;
    sum = 0.0;

    for(int i = 1; i <= n+1; i++){
        x = h*((double)i-0.5);
        sum += 4.0/(1.0 + x*x);
    }
    mypi = h * sum;

    printf("pi e aproximadamente %.16f \n", mypi);
}
