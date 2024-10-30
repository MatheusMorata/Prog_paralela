/*
Desenvolvido em MPI
*/
#include <stdio.h>
#include <mpi.h>

void main(int argc, char **argv){
    int numtasks, rank;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Numero de tarefas = %d Meu Rank = %d\n", numtasks, rank);
    MPI_Finalize();
}
