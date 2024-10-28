/**
 * CS3210 - Collective communication in MPI.
 */

#include <mpi.h>
#include <cstdio>
#include <iostream>
#define SIZE 4

int main(int argc, char *argv[])
{
	int numtasks, rank, sendcount, recvcount, source;
	float sendbuf[SIZE][SIZE] = 
	{
		{1.0, 2.0, 3.0, 4.0},
		{5.0, 6.0, 7.0, 8.0},
		{9.0, 10.0, 11.0, 12.0},
		{13.0, 14.0, 15.0, 16.0}  
	};
	float recvbuf[SIZE];
	
	
	
	

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	// notice that sendbuf == ptr to contigous 16 element array
	// i.e sendbuf == sendbuf[0], and the 2d array is stored as a contigous 1d array
	// i.e sendbuf[i + 1] == sendbuf[i] + 4 * 4
	if (rank == 0) {
		std::cout << sendbuf << std::endl;
		std::cout << sendbuf[0] << " " << sendbuf[1] << " " << sendbuf[2] << " " << sendbuf[3] << std::endl;
	}

	if (numtasks == SIZE) {
		source = 0;
		sendcount = SIZE;
		recvcount = SIZE;

		// thats why even though senBuf is of type char**, this still work
		MPI_Scatter(sendbuf,sendcount,MPI_FLOAT,recvbuf,recvcount,
			MPI_FLOAT,source,MPI_COMM_WORLD);
		printf("rank= %d  Results: %f %f %f %f\n",rank,recvbuf[0],
			recvbuf[1],recvbuf[2],recvbuf[3]);

		// TODO: Exercise 3
		float store[4];
		float sum = 0.0;
		for (int i = 0; i < SIZE; i ++) sum += recvbuf[i];
		MPI_Gather(&sum, 1, MPI_FLOAT, store, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
		
		if (rank == 0) {
			sum = 0;
			for (int i = 0; i < SIZE; i ++) sum += store[i];
			printf("Sum is %f\n", sum);
		}
		
		
	} else {
		printf("Must specify %d processes. Terminating.\n",SIZE);
	}

	MPI_Finalize();
	
	return 0;
	
}

