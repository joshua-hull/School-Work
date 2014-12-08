CC=clang
mpi:
	mpicc -o heat_mpi.exe -g -Wall heat_mpi.c
serial:
	$(CC) -o heat_serial.exe -g -Wall heat_serial.c
comm:
	mpicc -o heat_mpi_comm.exe -g -Wall heat_mpi_comm.c