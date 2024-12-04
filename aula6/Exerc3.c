#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// Função para inicializar e preencher o vetor com o valor -1.0
void inicializar_vetor(double *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = -1.0;
    }
}

// Função para alterar todos os valores do vetor para 2.0
void alterar_vetor_process0(double *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = 2.0;
    }
}

// Função para alterar todos os valores do vetor para 4.0
void alterar_vetor_process1(double *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = 4.0;
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    double *vetor;
    double tempo_total = 0.0;
    MPI_Status status;
    int NUMBER_OF_TESTES = atoi(argv[1]);
    int tamanho = atoi(argv[2]);
    const char* filename = argv[3];

    // Inicializa o MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Certifique-se de que temos pelo menos dois processos
    if (size < 2) {
        fprintf(stderr, "Este programa requer pelo menos 2 processos.\n");
        MPI_Finalize();
        return 1;
    }

    // Aloca dinamicamente o vetor
    vetor = (double *)malloc(tamanho * sizeof(double));
    if (vetor == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor.\n");
        MPI_Finalize();
        return 1;
    }

    // Inicializa o vetor
    inicializar_vetor(vetor, tamanho);

    // O processo 0 altera o vetor para 2.0 e envia para o processo 1
    if (rank == 0) {
        alterar_vetor_process0(vetor, tamanho);
            
        // Mede o tempo de envio e recebimento
        double inicio = MPI_Wtime();

        for (int j = 0; j < NUMBER_OF_TESTES; j++) {
            MPI_Send(vetor, tamanho, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(vetor, tamanho, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
        }

        double fim = MPI_Wtime();
        tempo_total += (fim - inicio);  // Acumula o tempo total
        FILE* output = fopen(filename, "w");
        fprintf(output, "Tempo total de envio e recebimento para o vetor de tamanho %d: %f segundos\n", tamanho, fim - inicio);
        fclose(output);
    }

    // O processo 1 recebe o vetor, altera para 4.0 e envia de volta para o processo 0
    else if (rank == 1) {
        for (int j = 0; j < NUMBER_OF_TESTES; j++) {
            MPI_Recv(vetor, tamanho, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            alterar_vetor_process1(vetor, tamanho);
            MPI_Send(vetor, tamanho, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    // Libera a memória do vetor
    free(vetor);
    // Finaliza o MPI
    MPI_Finalize();
    return 0;
    
}
