/*
  Hello World usando o MPI
*/
#include <stdio.h>
#include <mpi.h>

void main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    printf("Hello World! \n");
    MPI_Finalize();
}
