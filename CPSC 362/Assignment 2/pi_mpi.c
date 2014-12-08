/* 
 * Joshua Hull (jhull@clmeson.edu)
 * CPSC 3620-001 Distributed Computing
 * Assignment 2 : Monte Carlo Simulation to esitmate Pi.
 * pi_mpi.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char** argv){
	int numpoints, rank, size, tag;
	double pi = 0;
	double min_c, max_c, min_r, max_r, tmp, exec_time;
	int i;
	MPI_Status status;

	// Check our invocation
	if(argc != 2){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Check the output of atoi for a valid number of points.
	if((numpoints = atoi(argv[1])) <= 0){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Set up MPI enviornment and timing.
	MPI_Init(&argc,&argv);
	exec_time = MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

  	// Seed the ranom numnber generator.
	srand(time(NULL));

	// Figure out where in the square we as a MPI process live
	// and what our limits are.
	int column = rank % (int)sqrt(size);
	int row = rank / (int)sqrt(size);
	double gridspacing = 2.0 / sqrt(size);

	min_c = (gridspacing * column) - 1;
	max_c = (gridspacing * (column + 1)) -1;

	min_r = (gridspacing * row) - 1;
	max_r = (gridspacing * (row + 1)) - 1;

	// Each process calculate only part of the points.
	for(i = 0; i< numpoints/size; i++){
		// Get a point within our subsquare
		double x = min_c + rand() /(RAND_MAX/(max_c-min_c));
		double y = min_r + rand() /(RAND_MAX/(max_r-min_r));

		// Calculate the point's radius.
		double radius = sqrt(x*x+y*y);

		// If it's within the unit circle then add to the probability.
		if(radius < 0.5){
			pi++;
		}
	}

	// Get the porbability for this process.
	pi /= numpoints;

	// Send the value to the root process if there is more then one process running.
	if (size != 1){
		if (rank != 1){
			if (rank == 0){
				tag = 1000;
			}
			else tag = rank;
			// Send to root process.
			MPI_Send(&pi,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
		}
		if (rank == 1){
			for (i = 1; i < size; i++){
				// Get the values of pi from everyone else.
				MPI_Recv(&tmp,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
				pi += tmp;
			}

		// This was what I needed to get correct values for pi.
		// I now it's supposed to four but that didn't work.
		pi *= 16.0;

		// Stop the execution timer.
		exec_time = MPI_Wtime() - exec_time;
		
		// Output values.
		printf("PI = %.50f\n", pi);
		printf("Execution time: %.15f seconds\n", exec_time);
		}
	}
	// If p = 1 then save some work.
	else{    
		// This was what I needed to get correct values for pi.
		// I now it's supposed to four but that didn't work.
		pi *= 16.0;
		exec_time = MPI_Wtime() - exec_time;
		printf("PI = %.50f\n", pi);
		printf("Execution time: %.15f seconds\n", exec_time);
	}

	MPI_Finalize();
	return(EXIT_SUCCESS);
}