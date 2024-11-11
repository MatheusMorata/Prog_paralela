#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_MESSAGES 100

int main(int argc, char *argv[]) {
    int rank, size;
    char message[100];  // A mensagem enviada por cada processo será uma string simples.
    FILE *output_file;

    // Inicializa o MPI
    MPI_Init(&argc, &argv);

    // Obtém o número de processos e o identificador do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Processo 0 abre o arquivo para escrever as mensagens recebidas
        output_file = fopen("output.txt", "w");
        if (output_file == NULL) {
            perror("Não foi possível abrir o arquivo para escrita");
            MPI_Finalize();
            return 1;
        }

        // Processo 0 espera pelas mensagens de todos os outros processos
        int count = 0;
        MPI_Status status;
        
        // Recebe 100 mensagens de cada processo, exceto o processo 0
        while (count < (size - 1) * NUM_MESSAGES) {
            // Recebe a mensagem de qualquer processo (qualquer source, qualquer tag)
            MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // Registra a mensagem recebida no arquivo
            fprintf(output_file, "Mensagem recebida de processo %d, tag %d: %s\n",
                    status.MPI_SOURCE, status.MPI_TAG, message);
            count++;
        }

        // Fecha o arquivo após ter registrado todas as mensagens
        fclose(output_file);
    } else {
        // Processos diferentes de 0 enviam 100 mensagens para o processo 0
        for (int i = 0; i < NUM_MESSAGES; i++) {
            // Cria uma mensagem personalizada para enviar
            snprintf(message, sizeof(message), "Mensagem %d do processo %d", i, rank);
            // Envia a mensagem para o processo 0 com a tag igual ao índice da mensagem
            MPI_Send(message, 100, MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }
    }

    // Finaliza o MPI
    MPI_Finalize();
    return 0;
}
