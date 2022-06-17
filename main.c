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

struct string data[131072];
struct string quary[128];
void Gen()
{
	for (int i = 0; i < 131072; i++)
	{
		char s[MAX_STRING];
		int x;
		x = ((rand() % 14) + 2);
		for (int j = 0; j < x; j++)
		{
			unsigned int ch = (rand() * rand());
			s[j] = (char)((ch % 26) + 'a');
		}
		data[i].val = s;
	}
	for (int i = 0; i < 128; i++)
	{
		char s[MAX_STRING];
		int x;
		x = ((rand() % 14) + 2);
		for (int j = 0; j < x; j++)
		{
			unsigned int ch = (rand() * rand());
			s[j] = (char)((ch % 26) + 'a');
		}
		quary[i].val = s;
	}
}

int main(int argc, char *argv[])
{
	Gen();
	// Beginning of parallel region

	int comm_sz; /* Number of processes    */
	int my_rank; /* My process rank        */

	/* Start up MPI */
	MPI_Init(NULL, NULL);

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	/* Get my rank among all the processes */

	if (comm_sz != 16)
	{
		if (my_rank == 0)
		{
			printf("Please run this application with 16 MPI processes.\n");
		}
		MPI_Finalize();
		return 0;
	}

	// Get the group or processes of the default communicator
	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	// Create the new communicators
	MPI_Comm local_comm;
    MPI_Comm Super_comm;

	if (my_rank % 4 == 0)
	{
		int group_a_ranks[4] = {0, 4, 8, 12};
		MPI_Group group_a;
		MPI_Group_incl(world_group, 4, group_a_ranks, &group_a);
		MPI_Comm_create(MPI_COMM_WORLD, group_a, &local_comm);
	}
	else if (my_rank % 4 == 1)
	{
		int group_b_ranks[4] = {1, 5, 9, 13};
		MPI_Group group_b;
		MPI_Group_incl(world_group, 4, group_b_ranks, &group_b);
		MPI_Comm_create(MPI_COMM_WORLD, group_b, &local_comm);
	}
	else if (my_rank % 4 == 2)
	{
		int group_c_ranks[4] = {2, 6, 10, 14};
		MPI_Group group_c;
		MPI_Group_incl(world_group, 4, group_c_ranks, &group_c);
		MPI_Comm_create(MPI_COMM_WORLD, group_c, &local_comm);
	}
	else if (my_rank % 4 == 3)
	{
		int group_d_ranks[4] = {3, 7, 11, 15};
		MPI_Group group_d;
		MPI_Group_incl(world_group, 4, group_d_ranks, &group_d);
		MPI_Comm_create(MPI_COMM_WORLD, group_d, &local_comm);
	}

	if(my_rank<4){
		int group_s_ranks[4] = {0, 1, 2,3};
		MPI_Group group_s;
		MPI_Group_incl(world_group, 4, group_s_ranks, &group_s);
		MPI_Comm_create(MPI_COMM_WORLD, group_s, &local_comm);
	}





#pragma omp parallel num_threads(2)
	{
		Hello(my_rank, comm_sz);
	}

	/* Shut down MPI */
	MPI_Finalize();

	// Ending of parallel region

	return 0;
}
