// OpenMP program to print Hello World
// using C language

// OpenMP header
#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h> /* For MPI functions, etc */
#pragma comment(lib, "msmpi.lib")
const int MAX_STRING = 16;

void Hello(int rank, int comsz)
{
	int my_thread = omp_get_thread_num();

	printf("HI from thread %d with rank %d of %d\n", my_thread, rank, comsz);
}

struct string
{
	char *val;
};

struct string data[100000];
struct string quary[100];
void Gen(){
	for(int i=0;i<100;i++){
       char s[MAX_STRING];
	   int x;
	   x =((rand()%14)+2);
        for(int j=0;j<x;j++){
            unsigned int ch =(rand()*rand());
            s[j]=(char)((ch%26)+'a');
        }   
        data[i].val=s;
    }
	for(int i=0;i<10;i++){
       char s[MAX_STRING];
	   int x;
	   x =((rand()%14)+2);
        for(int j=0;j<x;j++){
            unsigned int ch =(rand()*rand());
            s[j]=(char)((ch%26)+'a');
        }   
        quary[i].val=s;
    }
    
}

int main(int argc, char *argv[])
{
	Gen();
	// Beginning of parallel region
	
	int comm_sz;			   /* Number of processes    */
	int my_rank;			   /* My process rank        */

	/* Start up MPI */
	MPI_Init(NULL, NULL);

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
#pragma omp parallel num_threads(2)
	{
		Hello(my_rank, comm_sz);
	}

	/* Shut down MPI */
	MPI_Finalize();

	// Ending of parallel region

	return 0;
}
