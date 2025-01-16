#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef unsigned char cell_t;

struct slice {
    int tam;
    int ini;
    int fim;
};

cell_t **allocate_board(int size) {
    cell_t **board = (cell_t **)malloc(sizeof(cell_t *) * size);
    for (int i = 0; i < size; i++)
        board[i] = (cell_t *)malloc(sizeof(cell_t) * size);
    return board;
}

void free_board(cell_t **board, int size) {
    for (int i = 0; i < size; i++)
        free(board[i]);
    free(board);
}

int adjacent_to(cell_t **board, int size, int i, int j) {
    int k, l, count = 0;

    int sk = (i > 0) ? i - 1 : i;
    int ek = (i + 1 < size) ? i + 1 : i;
    int sl = (j > 0) ? j - 1 : j;
    int el = (j + 1 < size) ? j + 1 : j;

    for (k = sk; k <= ek; k++)
        for (l = sl; l <= el; l++)
            count += board[k][l];
    count -= board[i][j];

    return count;
}

void play(cell_t **board, cell_t **newboard, int ini, int fim, int size) {
    for (int i = ini; i <= fim; i++) {
        for (int j = 0; j < size; j++) {
            int a = adjacent_to(board, size, i, j);
            if (a == 2)
                newboard[i][j] = board[i][j];
            else if (a == 3)
                newboard[i][j] = 1;
            else
                newboard[i][j] = 0;
        }
    }
}

void print(cell_t **board, int size) {
    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++)
            printf("%c", board[i][j] ? 'x' : ' ');
        printf("\n");
    }
}

void read_file(FILE *f, cell_t **board, int size) {
    char *s = (char *)malloc(size + 10);
    for (int j = 0; j < size; j++) {
        fgets(s, size + 10, f);
        for (int i = 0; i < size; i++)
            board[i][j] = s[i] == 'x';
    }
    free(s);
}

int main(int argc, char **argv) {
    int size, steps, rank, num_procs;
    FILE *f;
    cell_t **prev, **next, **tmp;
    int tam, resto, tamanho, inicio, fim_local;
    struct slice *pt;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Processo 0 lê o tamanho do tabuleiro e o número de repetições
    if (rank == 0) {
        f = stdin;
        fscanf(f, "%d %d", &size, &steps);
    }
    
    // Processo 0 envia para todos os outros processos o tamanho do tabuleiro e o número de repetições
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&steps, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Aloca as matrizes para o tabuleiro
    prev = allocate_board(size);
    next = allocate_board(size);

    if (rank == 0) {
        read_file(f, prev, size);
        fclose(f);
    }
	
    // Todos os processos recebem o tabuleiro inicial do processo 0	
    for (int i = 0; i < size; i++) {
        MPI_Bcast(prev[i], size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    }

    // Particionamento do tabuleiro entre os processos
    pt = (struct slice *)calloc(num_procs, sizeof(struct slice));
    tam = size / num_procs;
    resto = size % num_procs;

    for (int i = 0; i < num_procs; i++) {
        if (i < resto) {
            tamanho = tam + 1;
            inicio = tamanho * i;
        } else {
            tamanho = tam;
            inicio = tamanho * i + resto;
        }
        fim_local = inicio + tamanho - 1;
        pt[i].tam = tamanho;
        pt[i].ini = inicio;
        pt[i].fim = fim_local;
    }

    int ini = pt[rank].ini;
    fim_local = pt[rank].fim;

    double start_time = MPI_Wtime();
    for (int step = 0; step < steps; step++) {
        // Comunicação para troca de bordas entre processos
        if (rank != 0) {
            MPI_Recv(prev[ini - 1], size, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != num_procs - 1) {
            MPI_Send(prev[fim_local], size, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(prev[fim_local + 1], size, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank != 0) {
            MPI_Send(prev[ini], size, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
        }

        play(prev, next, ini, fim_local, size);

        tmp = prev;
        prev = next;
        next = tmp;
    }
    
    double end_time = MPI_Wtime();
    
    // O processo 0 coleta as submatrizes dos outros processos para montar o tabuleiro completo	
    if (rank == 0) {
        cell_t **final_board = allocate_board(size);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                final_board[i][j] = prev[i][j];
            }
        }

        for (int p = 1; p < num_procs; p++) {
            for (int i = pt[p].ini; i <= pt[p].fim; i++) {
                MPI_Recv(final_board[i], size, MPI_UNSIGNED_CHAR, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        
        printf("Tempo de execução: %f segundos\n", end_time - start_time);
        print(final_board, size);
        free_board(final_board, size);
    // Outros processos enviam suas submatrizes para o processo 0
    } else {
        for (int i = ini; i <= fim_local; i++) {
            MPI_Send(prev[i], size, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    free_board(prev, size);
    free_board(next, size);
    free(pt);

    MPI_Finalize();
    return 0;
}
