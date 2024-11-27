#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MENSAGENS 100

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verificar se o nome do arquivo foi fornecido
    if (argc != 2) {
        if (rank == 0) {
            printf("Uso: %s <nome_do_arquivo_saida>\n", argv[0]);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const char* nome_arquivo = argv[1];

    if (rank == 0) {
        // Processo zero: Recebe mensagens e escreve no arquivo de saída
        FILE* output = fopen(nome_arquivo, "w");
        if (output == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < (size - 1) * NUM_MENSAGENS; i++) {
            int mensagem;
            MPI_Recv(&mensagem, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            fprintf(output, "Mensagem recebida %d do processo %d\n", mensagem, stat.MPI_SOURCE);
        }

        fclose(output);
    } else {
        // Outros processos: Enviam 100 mensagens para o processo zero
        for (int i = 0; i < NUM_MENSAGENS; i++) {
            int mensagem = i; // Mensagem começa do zero
            MPI_Send(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
