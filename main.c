// OpenMP program to print Hello World
// using C language

// OpenMP header
#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h> /* For MPI functions, etc */
#pragma comment(lib, "msmpi.lib")
const int MAX_STRING = 100;

void Hello( int rank,int comsz)
{
	int my_thread = omp_get_thread_num();
	

	printf("Hello Najm from thread %d with rank %d of %d\n", my_thread,rank,comsz );
}

int main(int argc, char *argv[])
{

	// Beginning of parallel region
	char greeting[MAX_STRING]; /* String storing message */
	int comm_sz;			   /* Number of processes    */
	int my_rank;			   /* My process rank        */

	/* Start up MPI */
	MPI_Init(NULL, NULL);

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

#pragma omp parallel num_threads(5)
		{
			Hello(my_rank,comm_sz);
		}
	
	/* Shut down MPI */
	MPI_Finalize();

	// Ending of parallel region
}
