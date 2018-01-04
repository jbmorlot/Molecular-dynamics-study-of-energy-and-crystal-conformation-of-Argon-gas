  #!/bin/bash

cd $1

gnuplot << EOF
set term png size 1600,900
unset key
set xrange[$2:$3]
set autoscale y
set grid

set output "$4.png"
set xlabel "$6"
set style fill solid border -1
set title "$5"
plot "$4.txt" using 1:2 with boxes 

EOF

#cd $6

exit