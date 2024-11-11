#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int value;
    
    // Inicializa o MPI
    MPI_Init(&argc, &argv);

    // Obtém o número de processos e o identificador do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Processo 0 lê o valor fornecido pelo usuário
        printf("Digite o valor inicial: ");
        scanf("%d", &value);
    }

    // Distribui o valor inicial para todos os processos
    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo adiciona seu identificador ao valor
    value += rank;

    // O processo 0 coleta os resultados de todos os processos
    if (rank == 0) {
        // Abre o arquivo para escrita
        FILE *output_file = fopen("output.txt", "w");
        if (output_file == NULL) {
            perror("Não foi possível abrir o arquivo para escrita");
            MPI_Finalize();
            return 1;
        }

        // Escreve o valor calculado pelo processo 0
        fprintf(output_file, "Identificação = %d valor %d\n", rank, value);

        // Agora, o processo 0 coleta os valores dos outros processos
        int temp_value;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&temp_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(output_file, "Identificação = %d valor %d\n", i, temp_value);
        }

        // Fecha o arquivo
        fclose(output_file);
    } else {
        // Outros processos enviam seus resultados de volta para o processo 0
        MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Finaliza o MPI
    MPI_Finalize();
    return 0;
}
