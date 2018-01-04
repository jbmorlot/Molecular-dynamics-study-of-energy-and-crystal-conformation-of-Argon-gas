#!/bin/bash

cd $1

gnuplot << EOF
set term png size 1600,900
set grid
set output "$2.png"
unset key

set autoscale x
set yrange [0:]
set xlabel "Temps (en ua)"
set ylabel "Temperature (en K)"
set title "$3"
plot "$2.txt" using 1:2 w l



EOF

exit