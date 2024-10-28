/**
 * CS3210 - Cartesian virtual topology for MPI processes.
 */

#include <mpi.h>
#include <cstdio>

#define SIZE 16

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int main(int argc, char *argv[])
{
	int numtasks, rank, source, dest, outbuf, i, tag=1, 
		inbuf[4]={MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,}, 
		nbrs[4], dims[2]={4,4}, 
		periods[2]={0,0}, reorder=0, coords[2];

	MPI_Request reqs[8];
	MPI_Status stats[8];
	MPI_Comm cartcomm;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	if (numtasks == SIZE) {
		
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm); // creates a cart comm
		MPI_Comm_rank(cartcomm, &rank); // get rank
		MPI_Cart_coords(cartcomm, rank, 2, coords); // get the process 'coors'
		
		// get the rank of the node that is up and down, if does not exist result is MPI_PROC_NULL , which is -2
		// 0 means the 1st index, i.e arr[r + 1][c], arr[r - 1][c], so is 1st index that changes
		MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN]); 
		MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT]);

	  	outbuf = rank;

		for (i=0; i<4; i++)	{
			dest = nbrs[i];
			source = nbrs[i];
			MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &reqs[i]);
			MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &reqs[i+4]);
		}

		MPI_Waitall(8, reqs, stats);
	   
		for (i = 0; i < SIZE; i++) {
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == i) {
				printf("rank= %d coords= %d %d  neighbors(u,d,l,r)= %d %d %d %d\n",
					rank,coords[0],coords[1],nbrs[UP],nbrs[DOWN],nbrs[LEFT], 
					nbrs[RIGHT]);
				printf("rank= %d                 inbuf(u,d,l,r)= %d %d %d %d\n\n",
					rank,inbuf[UP],inbuf[DOWN],inbuf[LEFT],inbuf[RIGHT]);
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
	} else {
		printf("Must specify %d processes. Terminating.\n",SIZE);
	}
   
	MPI_Finalize();
	
	return 0;
	
}

// what this does is to get the rank of the 4 neigbours, up down left right
// if neighbour does not exist, the rank is -2

