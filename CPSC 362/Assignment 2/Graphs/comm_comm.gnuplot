set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5
set pointintervalbox 3
set title 'Joshua Hull (jhull@clemson.edu) CPSC 3620-001 Assignment 2 pi_mpi_comm.c Communication Time'
set xlabel 'Number of Processors (p)'
set ylabel 'Communication Time (s)'
set logscale x
set mxtics
set mytics
set output 'comm_comm.ps'
set term postscript
plot 'comm_comm_time.dat' with linespoints ls 1 title 'Communication Time'