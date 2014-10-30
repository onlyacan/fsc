reset
set datafile commentschars '#"'


set style line 1 lc rgb '#0060ad' lt 1 lw 2 # --- blue
set style line 2 lc rgb '#dd181f' lt 1 lw 2 # --- red
set style line 3 lc rgb '#000000' lt 1 lw 2 # --- black
set style line 4 lc rgb '#FFA500' lt 1 lw 2 # --- orange
set style line 5 lc rgb '#32CD32' lt 1 lw 2 # --- green
set style line 6 lc rgb '#000080' lt 1 lw 2 # --- navy
set style line 7 lc rgb '#D3D3D3' lt 1 lw 2 # --- Gainsboro





set terminal pngcairo size 820,800 enhanced font 'Verdana,10'
set output "mesh2.png"


#set border linewidth 0.8
set grid #back ls gridline
set key right bottom  maxrows 6

set xtics nomirror
set tics scale 1 

set ylabel 'x [m]'
set xlabel 'y [m]' 

#set label 'Tl = 1558 °C' at 0.2, Tl+20  font ',8' 
#set label 'Ts = 1462 °C' at 0.2, Ts-20     font ',8' 

plot  'normals' using 1:2:3:4 with vectors filled head ls 7 lw 1 ti 'normals',\
      'lines' using 1:2:3:4 with vectors filled head ls 2 lw 1 ti 'lines',\
      'points'  w p ls 1  pt 6 ps 1,\
      'centers' w p ls 2  pt 7 ps 1

set output "boundaries2.png"
plot  'boundaries' using 1:2:3:4 with vectors filled head ls 2 lw 1 ti 'boundaries'	

system('display mesh2.png&')
system('display boundaries2.png&')
