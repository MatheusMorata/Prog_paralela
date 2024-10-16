#include <stdio.h>
#include <stdlib.h>
#define SIZE 10000

void main(){
    int i, max, a[SIZE];

    for(i = 0; i < SIZE; i++){
        a[i] = rand();
    }

    max = a[0];
    for(i = 1; i < SIZE; i++){
        if(a[i] > max){
            max = a[i];
        }
    }
    printf("max = %d\n", max);
}
