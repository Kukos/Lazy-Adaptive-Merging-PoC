set terminal png size 800, 600
set datafile separator "\t"

set output "ex1.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment1.txt' using 0:col with lines title columnheader

set output "ex2.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment2.txt' using 0:col with lines title columnheader

set output "ex3.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment3.txt' using 0:col with lines title columnheader