#/usr/bin/bash

# memory usage by the example gls_all_measures.cpp
# courtesy http://stackoverflow.com/questions/7998302/graphing-a-processs-memory-usage

while true; do
	ps -C a.out -o pid=,%mem=,vsz= >> mem.log
	gnuplot ./gnuplot.script
	sleep 1
done
