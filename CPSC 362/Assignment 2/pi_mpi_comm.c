/* 
 * Joshua Hull (jhull@clmeson.edu)
 * CPSC 3620-001 Distributed Computing
 * Assignment 2 : Monte Carlo Simulation to esitmate Pi.
 * pi_mpi_comm.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define WORKTAG 123
#define TIMETAG 234 

int main(int argc, char** argv){
	int numpoints, rank, size;
	double pi = 0;
	double min_c, max_c, min_r, max_r, tmp, compute_time, comm_time = 0;
	int i;
	MPI_Status status;

	// Check our invocation
	if(argc != 2){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Check output of atoi for a valid number of points.
	if((numpoints = atoi(argv[1])) <= 0){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// MPI setup
	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

  	// Seed random number generator.
	srand(time(NULL));

	// Figure out where in the square we as a process are.
	int column = rank % (int)sqrt(size);
	int row = rank / (int)sqrt(size);
	double gridspacing = 2.0 / sqrt(size);

	min_c = (gridspacing * column) - 1;
	max_c = (gridspacing * (column + 1)) -1;

	min_r = (gridspacing * row) - 1;
	max_r = (gridspacing * (row + 1)) - 1;

	// Set up compute timer
	compute_time = MPI_Wtime();
	for(i = 0; i< numpoints/size; i++){
		// Get a point in our subsquare.
		double x = min_c + rand() /(RAND_MAX/(max_c-min_c));
		double y = min_r + rand() /(RAND_MAX/(max_r-min_r));

		// Calcualte the radius of the point.
		double radius = sqrt(x*x+y*y);

		// If within the unit circle add to the probability.
		if(radius < 0.5){
			pi++;
		}
	}
	// Get the probability for this process.
	pi /= numpoints;

	// Stop the compute timer.
	compute_time = MPI_Wtime() - compute_time;

	// Send our info to the root process.
	if(size != 1){
		if (rank != 1){
			// Send both our work for pi and how long it took.
	    	MPI_Send(&pi,1,MPI_DOUBLE,1,WORKTAG,MPI_COMM_WORLD);
	    	MPI_Send(&compute_time,1,MPI_DOUBLE,1,TIMETAG,MPI_COMM_WORLD);
	  	}
	  	if (rank == 1){
	  		// Start the communication timer.
	  		comm_time = MPI_Wtime();

	  		// Get everyone's data.
	    	for (i = 1; i < size; i++){
		      	MPI_Recv(&tmp,1,MPI_DOUBLE,MPI_ANY_SOURCE,WORKTAG,MPI_COMM_WORLD,&status);
		      	pi += tmp;
		      	MPI_Recv(&tmp,1,MPI_DOUBLE,MPI_ANY_SOURCE,TIMETAG,MPI_COMM_WORLD,&status);
		      	compute_time += tmp;
	    	}
	    	// Stop the comm timer.
	    	comm_time = MPI_Wtime() - comm_time;

	    	// Calcualte the average compute time across all the processes.
		    compute_time /= size;

		    // This was what I needed to get correct values for pi.
			// I now it's supposed to four but that didn't work.
	    	pi *= 16.0;

	    	// Output the data.
	  		printf("PI = %.50f\n", pi);
	  		printf("compute_time: %.15f seconds\n", compute_time);
	  		printf("comm_time: %.15f seconds\n", comm_time);
    	}
	}

	// Work around for only one process.
	else {
		    	compute_time /= size;

		    	// This was what I needed to get correct values for pi.
				// I now it's supposed to four but that didn't work.
	    		pi *= 16.0;
	  			printf("PI = %.50f\n", pi);
	  			printf("compute_time: %.15f seconds\n", compute_time);
	  			printf("comm_time: %.15f seconds\n", comm_time);
	}

	MPI_Finalize();
	return(EXIT_SUCCESS);
}