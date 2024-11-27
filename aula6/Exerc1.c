#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_FILENAME_LENGTH 100

int main(int argc, char *argv[]) {
    int rank, size, data, received_data;
    char filename[MAX_FILENAME_LENGTH];
    FILE *output_file;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Processo 0 lê o nome do arquivo e o valor inicial da linha de comando
        if (argc != 3) {
            fprintf(stderr, "Uso: %s <nome-do-arquivo> <valor-inicial>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Nome do arquivo com extensão .txt
        snprintf(filename, MAX_FILENAME_LENGTH, "%s.txt", argv[1]);
        data = atoi(argv[2]);
    }

    // Transmissão do dado do processo 0 para os demais
    if (rank == 0) {
        // Envia o dado para o próximo processo
        MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    } else {
        // Recebe o dado do processo anterior
        MPI_Recv(&received_data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Atualiza o valor do dado
        data = received_data + rank;
        
        // Envia o dado ao próximo processo, se não for o último
        if (rank < size - 1) {
            MPI_Send(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    // Todos os processos enviam o dado calculado de volta ao processo 0
    MPI_Send(&data, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    if (rank == 0) {
        // Processo 0 coleta e imprime os valores de todos os processos
        output_file = fopen(filename, "w");
        if (!output_file) {
            fprintf(stderr, "Erro ao abrir o arquivo de saída.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fprintf(output_file, "Identificação = %d valor %d\n", rank, data);

        for (int i = 1; i < size; i++) {
            MPI_Recv(&received_data, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(output_file, "Identificação = %d valor %d\n", i, received_data);
        }

        fclose(output_file);
    }

    MPI_Finalize();
    return 0;
}
