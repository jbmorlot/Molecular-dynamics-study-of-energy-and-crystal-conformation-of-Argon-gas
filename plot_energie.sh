#!/bin/bash

cd $1

gnuplot << EOF
set term png size 1600,900
set grid
set output "$2.png"

set multiplot

set key right bottom box spacing 1.5
set origin 0.0,0.0
set size 1,0.5  
set xrange[0:$5]
set yrange [0:]
set xlabel "Temps (en ua)"
set ylabel "Ecart-type (en ua)"
set title "$4"
plot "$2.txt" using 1:3 with line ti "$7"

set origin 0.0,0.5
set key right bottom box spacing 1.5
set xrange[0:$5]
set yrange [0:]
set xlabel "Temps (en ua)"
set ylabel "Energie (en ua)"
set title "$3"
plot "$2.txt" using 1:2 with line ti "$6"

unset multiplot






EOF

exit