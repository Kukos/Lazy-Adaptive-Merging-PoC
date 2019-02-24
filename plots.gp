set terminal png size 800, 600
set datafile separator "\t"

set output "ex1_1.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment1_1.txt' using 0:col with lines title columnheader

set output "ex1_2.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment1_2.txt' using 0:col with lines title columnheader

set output "ex1_3.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:4] 'experiment1_3.txt' using 0:col with lines title columnheader



set output "ex2_1.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:5] 'experiment2_1.txt' using 0:col with lines title columnheader

set output "ex2_2.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:5] 'experiment2_2.txt' using 0:col with lines title columnheader

set output "ex2_3.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Time [s]"
set xlabel "querry number"
plot for [col=2:5] 'experiment2_3.txt' using 0:col with lines title columnheader