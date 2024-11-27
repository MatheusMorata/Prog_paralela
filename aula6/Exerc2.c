#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MESSAGES 100

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificar se o nome do arquivo foi fornecido
    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <output_filename>\n", argv[0]);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const char* filename = argv[1];

    if (rank == 0) {
        // Processo zero: Recebe mensagens e escreve no arquivo de saída
        FILE* output = fopen(filename, "w");
        if (output == NULL) {
            printf("Error opening file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < (size - 1) * NUM_MESSAGES; i++) {
            int message;
            MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            fprintf(output, "Received message %d from process %d\n", message, stat.MPI_SOURCE);
        }

        fclose(output);
    } else {
        // Outros processos: Enviam 100 mensagens para o processo zero
        for (int i = 0; i < NUM_MESSAGES; i++) {
            int message = rank * 100 + i; // Mensagem pode ser qualquer valor
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
