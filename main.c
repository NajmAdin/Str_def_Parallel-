// OpenMP program to print Hello World
// using C language

// OpenMP header
#define _CRT_SECURE_NO_WARNINGS
#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h> /* For MPI functions, etc */
#pragma comment(lib, "msmpi.lib")
const int MAX_STRING = 16;

static int distance(char *word1, int len1, char *word2, int len2)
{
	int matrix[len1 + 1][len2 + 1];
	int i;
	for (i = 0; i <= len1; i++)
	{
		matrix[i][0] = i;
	}
	for (i = 0; i <= len2; i++)
	{
		matrix[0][i] = i;
	}
	for (i = 1; i <= len1; i++)
	{
		int j;
		char c1;

		c1 = word1[i - 1];
		for (j = 1; j <= len2; j++)
		{
			char c2;

			c2 = word2[j - 1];
			if (c1 == c2)
			{
				matrix[i][j] = matrix[i - 1][j - 1];
			}
			else
			{
				int delete;
				int insert;
				int substitute;
				int minimum;

				delete = matrix[i - 1][j] + 1;
				insert = matrix[i][j - 1] + 1;
				substitute = matrix[i - 1][j - 1] + 1;
				minimum = delete;
				if (insert < minimum)
				{
					minimum = insert;
				}
				if (substitute < minimum)
				{
					minimum = substitute;
				}
				matrix[i][j] = minimum;
			}
		}
	}
	return matrix[len1][len2];
}

char data[131072][16];
char quary[128][16];
void Gen()
{
	for (int i = 0; i < 131072; i++)
	{

		for (int j = 0; j < 16; j++)
		{
			unsigned int ch = (rand() * rand());
			data[i][j] = (char)((ch % 26) + 'a');
		}
	}
	for (int i = 0; i < 128; i++)
	{

		for (int j = 0; j < 16; j++)
		{
			unsigned int ch = (rand() * rand());
			quary[i][j] = (char)((ch % 26) + 'a');
		}
	}
}

int main(int argc, char *argv[])
{

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
	int local_rank;

	if (my_rank % 4 == 0)
	{
		int group_a_ranks[4] = {0, 4, 8, 12};
		MPI_Group group_a;
		MPI_Group_incl(world_group, 4, group_a_ranks, &group_a);
		MPI_Comm_create(MPI_COMM_WORLD, group_a, &local_comm);
		MPI_Comm_rank(local_comm, &local_rank);
	}
	else if (my_rank % 4 == 1)
	{
		int group_b_ranks[4] = {1, 5, 9, 13};
		MPI_Group group_b;
		MPI_Group_incl(world_group, 4, group_b_ranks, &group_b);
		MPI_Comm_create(MPI_COMM_WORLD, group_b, &local_comm);
		MPI_Comm_rank(local_comm, &local_rank);
	}
	else if (my_rank % 4 == 2)
	{
		int group_c_ranks[4] = {2, 6, 10, 14};
		MPI_Group group_c;
		MPI_Group_incl(world_group, 4, group_c_ranks, &group_c);
		MPI_Comm_create(MPI_COMM_WORLD, group_c, &local_comm);
		MPI_Comm_rank(local_comm, &local_rank);
	}
	else if (my_rank % 4 == 3)
	{
		int group_d_ranks[4] = {3, 7, 11, 15};
		MPI_Group group_d;
		MPI_Group_incl(world_group, 4, group_d_ranks, &group_d);
		MPI_Comm_create(MPI_COMM_WORLD, group_d, &local_comm);
		MPI_Comm_rank(local_comm, &local_rank);
	}

	/*	if (my_rank < 4)
		{
			int group_s_ranks[4] = {0, 1, 2, 3};
			MPI_Group group_s;
			MPI_Group_incl(world_group, 4, group_s_ranks, &group_s);
			MPI_Comm_create(MPI_COMM_WORLD, group_s, &Super_comm);
		}
	*/
	// call gen data & quary

	if (my_rank == 0)
	{
		Gen();
		/*	for (int i = 0; i < 128; i++)
			{
				printf("%d- %s\n", i, quary[i]);
			}*/
		printf("////////////////////////\n");
	}

	// specify data  32

	char my_quary[32][16];
	char my_data[32768][16];

	// send quary
	MPI_Barrier(MPI_COMM_WORLD);
	if (my_rank == 0)
	{
		for (int i = 32; i < 128; i++)
		{
			int to = 1;
			if (i >= 64 && i < 96)
				to = 2;
			if (i >= 96 && i < 128)
				to = 3;
			MPI_Ssend(quary[i], 16, MPI_CHAR, to, 0, MPI_COMM_WORLD);
		}
		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				my_quary[i][j] = quary[i][j];
			}
		}
	}
	else if (my_rank == 1 || my_rank == 2 || my_rank == 3)
	{
		for (int i = 0; i < 32; i++)
		{
			MPI_Recv(my_quary[i], 16, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	if (local_rank == 0)
	{
		for (int i = 0; i < 32; i++)
		{
			MPI_Ssend(my_quary[i], 16, MPI_CHAR, 1, 0, local_comm);
			MPI_Ssend(my_quary[i], 16, MPI_CHAR, 2, 0, local_comm);
			MPI_Ssend(my_quary[i], 16, MPI_CHAR, 3, 0, local_comm);
		}
	}
	else
	{
		for (int i = 0; i < 32; i++)
		{
			MPI_Recv(my_quary[i], 16, MPI_CHAR, 0, 0, local_comm, MPI_STATUS_IGNORE);
		}
	}

	// send data

	MPI_Barrier(MPI_COMM_WORLD);
	if (my_rank == 0)
	{
		for (int i = 0; i < 131072; i++)
		{

			MPI_Ssend(data[i], 16, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Ssend(data[i], 16, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
			MPI_Ssend(data[i], 16, MPI_CHAR, 3, 0, MPI_COMM_WORLD);
		}
	}
	else if (my_rank == 1 || my_rank == 2 || my_rank == 3)
	{
		for (int i = 0; i < 131072; i++)
		{
			MPI_Recv(data[i], 16, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	// data distribution
	MPI_Barrier(MPI_COMM_WORLD);
	if (local_rank == 0)
	{
		for (int i = 32768; i < 131072; i++)
		{
			int to = 1;
			if (i >= 65536 && i < 98304)
				to = 2;
			if (i >= 98304 && i < 131072)
				to = 3;
			MPI_Ssend(data[i], 16, MPI_CHAR, to, 0, local_comm);
		}
		for (int i = 0; i < 32768; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				my_data[i][j] = data[i][j];
			}
		}
	}
	else
	{
		for (int i = 0; i < 32768; i++)
		{
			MPI_Recv(my_data[i], 16, MPI_CHAR, 0, 0, local_comm, MPI_STATUS_IGNORE);
		}
	}

	// printf("////////\n");

	int my_ed[32][32768];
	int idx = 0;

	const char *word1;
	const char *word2;
	int len1;
	int len2;

#pragma omp parallel for num_threads(8) default(none) shared(my_ed, my_data, my_quary) private(word1, word2)
	for (int i = 0; i < 32; i++)
	{

		for (int j = 0; j < 32768; j++)
		{
			int d = distance(my_quary[i], 16, my_data[j], 16);
			
#pragma omp critical
			my_ed[i][j] = d;
		}
	}

	/* Shut down MPI */
	MPI_Finalize();

	// Ending of parallel region

	return 0;
}
