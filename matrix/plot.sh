#!/bin/bash

PERF_CACHE_MISS_EVENT_NAME=cache-misses

N=128

make
rm -f plot.dat

for B in 1 2 4 8 16
do
	if (($B <= 8)); then
		echo -n "$B `bc <<< "$N*$N*$N/$B"` " >> plot.dat
	else
		echo -n "$B `bc <<< "$N*$N*$N/8"` " >> plot.dat
	fi
	perf stat -e $PERF_CACHE_MISS_EVENT_NAME ./main <<< "$B" 2>&1 \
		| awk 'NR==4 {print $1}' >> plot.dat
done


	WIDTH=80
	HEIGHT=25

gnuplot <<< "set term png size 1920,1080; \
            set output '../bench_results/matrix.png'; \
            plot 'plot.dat' u 1:2 title 'cache-misses(matrix size)' w linespoints"
