#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef unsigned char cell_t;

cell_t **allocate_board(int size) {
    cell_t **board = (cell_t **) malloc(sizeof(cell_t *) * size);
    for (int i = 0; i < size; i++)
        board[i] = (cell_t *) malloc(sizeof(cell_t) * size);
    return board;
}

void free_board(cell_t **board, int size) {
    for (int i = 0; i < size; i++)
        free(board[i]);
    free(board);
}

/* return the number of on cells adjacent to the i,j cell */
int adjacent_to(cell_t **board, int size, int i, int j) {
    int count = 0;
    int sk = (i > 0) ? i - 1 : i;
    int ek = (i + 1 < size) ? i + 1 : i;
    int sl = (j > 0) ? j - 1 : j;
    int el = (j + 1 < size) ? j + 1 : j;

    for (int k = sk; k <= ek; k++) {
        for (int l = sl; l <= el; l++) {
            count += board[k][l];
        }
    }
    count -= board[i][j];
    return count;
}

void play(cell_t **board, cell_t **newboard, int size, int start_col, int end_col, int rank) {
    for (int i = 0; i < size; i++) {
        for (int j = start_col; j < end_col; j++) {
            int a = adjacent_to(board, size, i, j);
            if (a == 2) newboard[i][j] = board[i][j];
            if (a == 3) newboard[i][j] = 1;
            if (a < 2) newboard[i][j] = 0;
            if (a > 3) newboard[i][j] = 0;

            // Exibe o rank e as coordenadas
            printf("Rank %d trabalhando na célula [%d, %d]\n", rank, i, j);
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
        for (int i = 0; i < size; i++) {
            board[i][j] = s[i] == 'x';
        }
    }
    free(s);
}

int main(int argc, char **argv) {
    int size, steps;
    FILE *f = stdin;
    fscanf(f, "%d %d", &size, &steps);

    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    cell_t **prev = allocate_board(size);
    read_file(f, prev, size);
    fclose(f);

    cell_t **next = allocate_board(size);
    cell_t **tmp;

    int cols_per_process = size / num_procs;
    int start_col = rank * cols_per_process;
    int end_col = (rank == num_procs - 1) ? size : (rank + 1) * cols_per_process;

    for (int i = 0; i < steps; i++) {
        play(prev, next, size, start_col, end_col, rank);
        MPI_Barrier(MPI_COMM_WORLD); // Garantir que todos os processos terminaram

        tmp = next;
        next = prev;
        prev = tmp;
    }

    // Coleta os resultados de todos os processos (opcional, se necessário)
    MPI_Gather(*prev, size * size / num_procs, MPI_UNSIGNED_CHAR,
               *next, size * size / num_procs, MPI_UNSIGNED_CHAR,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        print(next, size); // Imprime o resultado final
    }

    free_board(prev, size);
    free_board(next, size);

    MPI_Finalize();
    return 0;
}
