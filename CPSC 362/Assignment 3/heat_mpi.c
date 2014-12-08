#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

void print_usage(){
	printf("mpirun -n [number of processces] heat_serial.exe [# of iterations] [room dimension]\n");
	exit(EXIT_FAILURE);
}

char* print_color(double temperature){
	if (temperature > 250.0) return RED;
	if (temperature > 180.0) return ORANGE;
	if (temperature > 120.0) return YELLOW;
	if (temperature > 80.0) return LTGREEN;
	if (temperature > 60.0) return GREEN;
	if (temperature > 50.0) return LTBLUE;
	if (temperature > 40.0) return BLUE;
	if (temperature > 30.0) return DARKTEAL;
	if (temperature > 20.0) return BROWN;
	return BLACK;
}

int main(int argc, char **argv) {

	int i, j, k, left, right, size, rank, my_size;
	double* old;
	double* new;
    MPI_Status status;

	if(argc != 3) print_usage();

	int iterations = atoi(argv[1]);
	int dim = atoi(argv[2]);
	if((iterations < 0) || (dim < 0)) print_usage();

	MPI_Init (&argc, &argv);
  	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  	MPI_Comm_size (MPI_COMM_WORLD, &size);

  	if(dim%size != 0){
  		if(rank == 0) printf("Please make sure the dimesnion is divisible by the number of processes.\n");
  		MPI_Finalize();
  		exit(EXIT_SUCCESS);
  	}

	my_size = (dim/size);

	old = calloc(((my_size+2) * (dim+2)), sizeof(double));
	new = calloc(((my_size+2) * (dim+2)), sizeof(double));

	for (i = 0; i < my_size + 2; i++) {
		for (j = 0; j < dim + 2; j++) {
			old[i*(dim+2)+j] = 20;
			new[i*(dim+2)+j] = 20;
		}
	}

	if (rank == 0) {
		left = (int) dim * .30;
		right = (int) dim * .70;

		for(i = 0; i < left + 1; i++){
			old[i] = 20;
			new[i] = 20;
		}

		for(i = left + 1; i <= right + 1; i++){
			old[i] = 300;
			new[i] = 300;
		}

		for(i=right+2; i <= dim +1; i++){
			old[i] = 20;
			new[i] = 20;
		}
	}

	for(k=1; k<= iterations; k++){
		for (i = 1; i <= my_size; i++) {
			for (j = 1; j <= dim; j++) {
				new[i*(dim+2)+j] = ((old[(i+1)*(dim+2)+j] + old[(i-1)*(dim+2)+j] + old[i*(dim+2)+(j+1)] + old[i*(dim+2)+(j-1)])/4);
			}
		}

		for (i = 1; i <= my_size; i++) {
			for (j = 0; j <= dim + 1; j++) {
				old[i*(dim+2)+j] = new[i*(dim+2)+j];
			}
		}

        if (rank == 0){
	      MPI_Send(&old[(my_size)*(dim+2)],dim+2,MPI_DOUBLE,rank+1,0,MPI_COMM_WORLD);
	      MPI_Recv(&old[(my_size+1)*(dim+2)],dim+2,MPI_DOUBLE,rank+1,0,MPI_COMM_WORLD,&status);
	    }
	    else if (rank != size - 1){
	      MPI_Send(&old[dim+2],dim+2,MPI_DOUBLE,rank-1,0,MPI_COMM_WORLD);
	      MPI_Recv(&old[0],dim+2,MPI_DOUBLE,rank-1,0,MPI_COMM_WORLD,&status);

	      MPI_Recv(&old[(my_size+1)*(dim+2)],dim+2,MPI_DOUBLE,rank+1,0,MPI_COMM_WORLD,&status);
	      MPI_Send(&old[(my_size)*(dim+2)],dim+2,MPI_DOUBLE,rank+1,0,MPI_COMM_WORLD);
	    }
	    else if (rank == size - 1){
	      MPI_Send(&old[dim+2],dim+2,MPI_DOUBLE,rank-1,0,MPI_COMM_WORLD);
	      MPI_Recv(&old[0],dim+2,MPI_DOUBLE,rank-1,0,MPI_COMM_WORLD,&status);
	    }
	    MPI_Barrier(MPI_COMM_WORLD);
	}

	// double total_room = calloc((dim+2)*(dim+2), sizeof(double));
	// MPI_Gather(&old[dim+2], my_size*(dim+2), MPI_DOUBLE &total_room[0], (dim+2)*(dim+2), MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
    if(rank == 0){
        char filename[32];
	    sprintf(filename,"output.pnm");
	    FILE* fp;
	    
	    fp = fopen(filename,"w");
	    fprintf(fp, "P3\n%d %d\n15\n",dim, dim);
	    fclose(fp);
    }

    for (i = 0; i < size; i++){
    	if(rank == i){
	    	char filename[32];
		    sprintf(filename,"output.pnm");
		    FILE* fp;
		    fp = fopen(filename,"a");
			for (i = 1; i <= my_size; i++) {
	        	for (j = 1; j <= dim; j++) {
	            	fprintf(fp, "%s ", print_color(old[i*(dim+2)+j]));
	        	}
	        	fprintf(fp,"\n");
	    	}
		    fclose(fp);
    	}
    	// MPI_Barrier(MPI_COMM_WORLD);
    }

	free(old);
	free(new);
	MPI_Finalize();
	return EXIT_SUCCESS;
}
