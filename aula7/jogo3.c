// JOGO DA VIDA EM PARALELO OPENMP (1)

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef unsigned char cell_t; 

cell_t ** allocate_board (int size) {
    cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
    int i;
    for (i=0; i<size; i++)
        board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
    return board;
}

void free_board (cell_t ** board, int size) {
    int i;
    for (i=0; i<size; i++)
        free(board[i]);
    free(board);
}

int adjacent_to (cell_t ** board, int size, int i, int j) {
    int k, l, count=0;
    
    int sk = (i>0) ? i-1 : i;
    int ek = (i+1 < size) ? i+1 : i;
    int sl = (j>0) ? j-1 : j;
    int el = (j+1 < size) ? j+1 : j;

    for (k=sk; k<=ek; k++)
        for (l=sl; l<=el; l++)
            count+=board[k][l];
    count-=board[i][j];
    
    return count;
}

void play (cell_t ** board, cell_t ** newboard, int size) {
    int i, j, a;
    #pragma omp parallel for private(i, j, a) shared(board, newboard, size) collapse(2)
    for (i=0; i<size; i++) {
        for (j=0; j<size; j++) {
            a = adjacent_to (board, size, i, j);
            if (a == 2) newboard[i][j] = board[i][j];
            if (a == 3) newboard[i][j] = 1;
            if (a < 2) newboard[i][j] = 0;
            if (a > 3) newboard[i][j] = 0;
        }
    }
}

void print (cell_t ** board, int size) {
    int i, j;
    for (j=0; j<size; j++) {
        for (i=0; i<size; i++) 
            printf ("%c", board[i][j] ? 'x' : ' ');
        printf ("\n");
    }
}

void read_file (FILE * f, cell_t ** board, int size) {
    int i, j;
    char *s = (char *) malloc(size+10);
    for (j=0; j<size; j++) {
        fgets (s, size+10, f);
        for (i=0; i<size; i++)
            board[i][j] = s[i] == 'x';
    }
    free(s);
}

int main (int argc, char **argv) {
    int size, steps;
    FILE *f;
    f = stdin;
    fscanf(f,"%d %d", &size, &steps);
    cell_t ** prev = allocate_board(size);
    read_file(f, prev, size);
    fclose(f);
    cell_t ** next = allocate_board(size);
    cell_t ** tmp;

    double start_time = omp_get_wtime();
    
    for (int i = 0; i < steps; i++) {
        play(prev, next, size);
        tmp = next;
        next = prev;
        prev = tmp;
    }

    double end_time = omp_get_wtime();
    printf("Tempo de execução da função play: %f segundos\n", end_time - start_time);

    print(prev, size);
    free_board(prev, size);
    free_board(next, size);

    return 0;
}
