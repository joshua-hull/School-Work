/* 
 * Joshua Hull (jhull@clmeson.edu)
 * CPSC 3620-001 Distributed Computing
 * Assignment 2 : Monte Carlo Simulation to esitmate Pi.
 * pi_serial.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char** argv){
	int numpoints;
	double pi = 0;

	// Check inovcation.
	if(argc != 2){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Check if there was an error with atoi or we got a bad value for the number of points.
	if((numpoints = atoi(argv[1])) <= 0){
		fprintf(stderr,"Usage: %s <# of points>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// Seed random number generator.
	srand(time(NULL));


	// For each random point...
	int i;
	for(i = 0; i< numpoints; i++){
		// Calculate our random x and y coordinates.
		double x = ((double) rand() / (RAND_MAX/2)) - 1.0;
		double y = ((double) rand() / (RAND_MAX/2)) - 1.0;

		// Calculate the radius from the origin.
		double radius = sqrt(x*x+y*y);

		// If inside the unit circle then add to the porbability.
		if(radius < 0.5){
			pi++;
		}
	}

	// Find the probablity of being in the unit circle.
	pi /= numpoints;

	// This was what I needed to get correct values for pi.
	// I now it's supposed to four but that didn't work.
	pi *= 16.0;

	// Output pi.
	printf("PI = %f\n", pi);

	return(EXIT_SUCCESS);
}