/*
Usando send e recieve(Troca de mensagens)
*/
#include <stdio.h>
#include <mpi.h>

void main(int argc, char *argv[]){
    int numtasks, rank, dest, source, rc, count, tag=1;
    char inmsg, outmsg;
    MPI_Status Stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0){
        dest = 1;
        source = 1;
        outmsg = 'x';
        rc = MPI_Send(&outmsg,1,MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        rc = MPI_Recv(&inmsg,1,MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
    }else{
        if(rank == 1){
            outmsg = 'y';
            dest = 0;
            source = 0;
            rc = MPI_Recv(&inmsg,1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&outmsg,1,MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        }
    }
    rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
    printf("Tarefa %d: Recebeu %d char(s) da tarefa %d com a tag %d inmsg = %c outmsg = %c \n", rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG, inmsg, outmsg);

    MPI_Finalize();

}
