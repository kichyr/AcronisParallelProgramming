#!/bin/bash

PERF_CACHE_MISS_EVENT_NAME=cache-misses

N=128

make
rm -f plot.dat

for B in 1 2 4 8 16
do
	echo -n "$B  " >> plot.dat
	perf stat -e $PERF_CACHE_MISS_EVENT_NAME ./main "`bc <<< "2^10"`" "$B" 2>&1 \
		| awk 'NR==6 || NR==4 {print $1}' | tr \\n ' ' >> plot.dat
	echo "" >> plot.dat
done


	WIDTH=80
	HEIGHT=25

#gnuplot <<< "set term png size 1920,1080; \
#            set output '../bench_results/c_miss_matrix.png'; \
#            plot 'plot.dat' u 1:3 title 'cache-misses(matrix size)' w linespoints"
gnuplot <<< "set term png size 1920,1080; \
            set output '../bench_results/matrix.png'; \
            plot 'plot.dat' u 1:3 title 'time(thread number)' w linespoints"
