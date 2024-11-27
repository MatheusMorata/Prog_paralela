#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size, value, received_value;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificar se o nome do arquivo e o valor inicial foram fornecidos
    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <output_filename> <initial_value>\n", argv[0]);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const char* filename = argv[1];
    value = atoi(argv[2]);

    // Broadcast do valor inicial do processo zero para todos os outros processos
    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo i recebe o valor do processo i-1, soma seu rank e envia para o processo i+1
    if (rank != 0) {
        MPI_Recv(&received_value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat);
        value = received_value + rank;
    }
    if (rank != size - 1) {
        MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    // Cada processo envia seu valor calculado de volta para o processo zero
    MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    // O processo zero recebe e imprime os valores calculados de todos os processos
    if (rank == 0) {
        FILE* output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error opening file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        for (int i = 0; i < size; i++) {
            MPI_Recv(&received_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
            fprintf(output, "Identificação = %d valor %d\n", i, received_value);
        }
        fclose(output);
    }

    MPI_Finalize();
    return 0;
}
