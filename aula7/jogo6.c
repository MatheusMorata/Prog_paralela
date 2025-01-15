#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef unsigned char cell_t; 

struct slice{
	int tam;
	int ini;
	int fim;
};

cell_t ** allocate_board (int size) {
	cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
	int	i;
	for (i=0; i<size; i++)
		board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
	return board;
}

void free_board (cell_t ** board, int size) {
        int     i;
        for (i=0; i<size; i++)
                free(board[i]);
	free(board);
}


/* return the number of on cells adjacent to the i,j cell */
int adjacent_to (cell_t ** board, int size, int i, int j) {
	int	k, l, count=0;
	
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
	int	i, j, a;
	/* for each cell, apply the rules of Life */
	for (i=0; i<size; i++)
		for (j=0; j<size; j++) {
			a = adjacent_to (board, size, i, j);
			if (a == 2) newboard[i][j] = board[i][j];
			if (a == 3) newboard[i][j] = 1;
			if (a < 2) newboard[i][j] = 0;
			if (a > 3) newboard[i][j] = 0;
		}
}

/* print the life board */
void print (cell_t ** board, int size) {
	int	i, j;
	/* for each row */
	for (j=0; j<size; j++) {
		/* print each column position... */
		for (i=0; i<size; i++) 
			printf ("%c", board[i][j] ? 'x' : ' ');
		/* followed by a carriage return */
		printf ("\n");
	}
}

/* read a file into the life board */
void read_file (FILE * f, cell_t ** board, int size) {
	int	i, j;
	char	*s = (char *) malloc(size+10);
	char c;
	for (j=0; j<size; j++) {
		/* get a string */
		fgets (s, size+10,f);
		/* copy the string to the life board */
		for (i=0; i<size; i++)
		{
		 	//c=fgetc(f);
			//putchar(c);
			board[i][j] = s[i] == 'x';
		}
		//fscanf(f,"\n");
	}
}

int main (int argc, char **argv) {
	int size, steps, rank, num_procs, i, j;
	FILE    *f;
  	//f = stdin;
	//fscanf(f,"%d %d", &size, &steps);
	//cell_t ** prev = allocate_board (size);
	//read_file (f, prev,size);
	//fclose(f);
	//cell_t ** next = allocate_board (size);
	//cell_t ** tmp;
	int tag =0;
	MPI_Status status;
    int tam, resto, tamanho, inicio, fim;
	struct slice *pt;


	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    

	// Enviando para size e steps para todos os processos, a partir do zero 
	if (rank == 0){
		f = stdin;
		fscanf(f,"%d %d", &size, &steps);
		for (i=0; i< num_procs; ++i) {
		    MPI_Send(&size, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		    MPI_Send(&steps, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
		}
		cell_t ** prev = allocate_board (size);
	    read_file (f, prev,size);
		fclose(f);
	}
	else {
		MPI_Recv(&size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	    MPI_Recv(&steps, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

	}


    pt = (struct slice *) calloc (num_procs, sizeof (struct slice));
	if (pt == (struct slice *)  NULL){
		printf("Nao alocou o ponteiro da estrutura slice");
		exit (1);
	}

	/*
	rank < resto 
		tamanho = tamanho + 1
		inicio = tamanho * rank 
		fim = inicio + tamanho - 1
	else
		inicio = rank * tam + resto 
		fim = inicio + tam - 1
	*/

	tam = size / num_procs;
	resto = size % num_procs;

	if(rank == 0){
		for (i = 0; i < num_procs; ++i) {
			if (i < resto) {
				tamanho = tam+1;
				inicio = tamanho * rank;
				fim = (inicio + tamanho) -1;
				printf("tam: %d ini: %d fim: %d\n", tamanho, inicio, fim);
				(pt+i)->tam = tamanho;
				(pt+i)->ini = inicio;
				(pt+i)->fim = fim;
			}
			else {
				tamanho = tam;
				inicio = tamanho * rank + resto;
				fim = inicio + tamanho -1;
				printf("tam: %d ini: %d fim: %d\n", tamanho, inicio, fim);
				(pt+i)->tam = tamanho;
				(pt+i)->ini = inicio;
				(pt+i)->fim = fim;
			}
			printf(" i:%d tam: %d ini: %d fim: %d \n", i, (pt+i)->tam, (pt+i)->ini, (pt+i)->fim);
		}
	}

/*	
  if( rank == 0) {
	printf(" ----------------SO O ZERO\n");
    for (i = 0; i < num_procs; ++i) {
		printf (" i:%d tam: %d ini: %d fim: %d \n", i, (pt+i)->tam, (pt+i)->ini, (pt+i)->fim);
	}
	
  }

  */

	#ifdef DEBUG
	printf("Initial \n");
	print(prev,size);
	printf("----------\n");
	#endif

/*
	for (i=0; i<steps; i++) {
		play (prev,next,size);
                #ifdef DEBUG
		printf("%d ----------\n", i);
		print (next,size);
		#endif
		tmp = next;
		next = prev;
		prev = tmp;
	}
*/
	//print (prev,size);
	//free_board(prev,size);
	//free_board(next,size);

	MPI_Finalize();
	return 0;
}
