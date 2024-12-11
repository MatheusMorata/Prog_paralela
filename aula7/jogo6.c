// JOGO DA VIDA EM PARALELO USANDO MPI (2)

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
typedef unsigned char cell_t; 

cell_t ** allocate_board (int size) {
    cell_t ** board = (cell_t **) malloc(sizeof(cell_t*) * size);
    int i;
    for (i = 0; i < size; i++)
        board[i] = (cell_t *) malloc(sizeof(cell_t) * size);
    return board;
}

void free_board (cell_t ** board, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(board[i]);
    free(board);
}

int adjacent_to (cell_t ** board, int size, int i, int j) {
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

void play (cell_t ** board, cell_t ** newboard, int size, int start_row, int end_row) {
    int i, j, a;
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < size; j++) {
            a = adjacent_to(board, size, i, j);
            if (a == 2) newboard[i][j] = board[i][j];
            if (a == 3) newboard[i][j] = 1;
            if (a < 2) newboard[i][j] = 0;
            if (a > 3) newboard[i][j] = 0;
        }
    }
}

void print (cell_t ** board, int size) {
    int i, j;
    for (j = 0; j < size; j++) {
        for (i = 0; i < size; i++) 
            printf("%c", board[i][j] ? 'x' : ' ');
        printf("\n");
    }
}

void read_file (FILE * f, cell_t ** board, int size) {
    int i, j;
    char *s = (char *) malloc(size + 10);
    for (j = 0; j < size; j++) {
        fgets(s, size + 10, f);
        for (i = 0; i < size; i++)
            board[i][j] = s[i] == 'x';
    }
    free(s);
}

int main (int argc, char *argv[]) {
    int size, steps;
    FILE *f = stdin;
    fscanf(f, "%d %d", &size, &steps);
    cell_t ** prev = allocate_board(size);
    read_file(f, prev, size);
    fclose(f);
    cell_t ** next = allocate_board(size);
    cell_t ** tmp;

    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int rows_per_proc = size / nprocs;
    int start_row = rank * rows_per_proc;
    int end_row = (rank == nprocs - 1) ? size : start_row + rows_per_proc;

    double start_time, end_time;
    MPI_Status stat;

    start_time = MPI_Wtime();

    for (int step = 0; step < steps; step++) {
        play(prev, next, size, start_row, end_row);

        if (rank > 0)
            MPI_Send(next[start_row], size, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
        if (rank < nprocs - 1)
            MPI_Send(next[end_row - 1], size, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);

        if (rank > 0)
            MPI_Recv(next[start_row - 1], size, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, &stat);
        if (rank < nprocs - 1)
            MPI_Recv(next[end_row], size, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, &stat);

        tmp = next;
        next = prev;
        prev = tmp;
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        print(prev, size);
        printf("Execution time: %f seconds\n", end_time - start_time);
    }

    free_board(prev, size);
    free_board(next, size);

    MPI_Finalize();
    return 0;
}
