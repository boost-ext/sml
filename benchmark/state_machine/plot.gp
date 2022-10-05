set term png
set output "out.png"
set title "[compilation times] State Machine Language (sml) / smaller is better"
set style line 1 linetype 1 linewidth 1 pointtype 1 pointsize 0.0 lc rgb '#0072bd' # blue
set style line 2 linetype 1 linewidth 1 pointtype 1 pointsize 0.0 lc rgb '#d95319' # orange
set style line 3 linetype 1 linewidth 1 pointtype 1 pointsize 0.0 lc rgb '#edb120' # yellow
set style line 4 linetype 1 linewidth 1 pointtype 1 pointsize 0.0 lc rgb '#7e2f8e' # purple
set style line 5 linetype 1 linewidth 1 pointtype 1 pointsize 0.0 lc rgb '#77ac30' # green
set xlabel "number of states/events/transitions"
set ylabel "time [s]"

plot 'out' using 1:2 with linespoints ls 1 title "(clang-16/C++20) sml", \
     '' using 1:3 with linespoints ls 2 title "(rust-1.66/nightly) sml", \
