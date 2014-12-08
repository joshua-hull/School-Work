set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 pi -1 ps 1.5
set pointintervalbox 3
set title 'Joshua Hull (jhull@clemson.edu) CPSC 3620-001 Assignment 2 pi_mpi.c Speedup'
set xlabel 'Speedup Factor'
set ylabel 'Speedup Factor'
set logscale x
set mxtics
set mytics
set output 'mpi_speedup.ps'
set term postscript
plot 'mpi_speedup.dat' with linespoints ls 1 title 'Speedup Factor'