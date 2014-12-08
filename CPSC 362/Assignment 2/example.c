#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
  int rank, size;
  int tmp;
  int tag;
  int i;
  double start,end;
  MPI_Status status;

  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  start = MPI_Wtime();

  if (rank != 0){
    MPI_Recv(&tmp,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    printf("Process %d receives %d from rank %d\n",rank,tmp,status.MPI_SOURCE);
  }
  if (rank == 0){
    for (i = 1; i < size; i++){
      MPI_Send(&rank,1,MPI_INT,i,0,MPI_COMM_WORLD);
//      printf("I receive value %d from process %d",tmp,status.MPI_SOURCE);
    }
  }

  printf("Hello Clemson from %d out of %d \n", rank, size);

  end = MPI_Wtime();

  printf("The run time for process %d is %f \n",rank,end - start);
  MPI_Finalize();
  return 0;
}