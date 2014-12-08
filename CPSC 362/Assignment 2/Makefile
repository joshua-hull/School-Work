CC=clang -Wall -g
MPICC=/usr/lib64/openmpi/bin/mpicc -Wall -g

serial:
	${CC} -o pi_serial.exe -lm pi_serial.c

mpi:
	${MPICC} -o pi_mpi.exe -lm pi_mpi.c

comm:
	${MPICC} -o pi_mpi_comm.exe -lm pi_mpi_comm.c