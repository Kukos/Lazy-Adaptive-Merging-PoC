set terminal png size 800, 600
set datafile separator "\t"

# E1 Samsung
set output "ex1_1_samsung840.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex1_2_samsung840.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_samsung840.txt' using 1:col with lines title columnheader

set output "ex1_3_samsung840.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_samsung840.txt' using 1:col with lines title columnheader

# E2 Samsung
set output "ex2_1_samsung840.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex2_2_samsung840.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_samsung840.txt' using 1:col with lines title columnheader

set output "ex2_3_samsung840.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_samsung840.txt' using 1:col with lines title columnheader

# E1 Intel
set output "ex1_1_intelDCP4511.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex1_2_intelDCP4511.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex1_3_intelDCP4511.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_intelDCP4511.txt' using 1:col with lines title columnheader


# E2 Intel
set output "ex2_1_intelDCP4511.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex2_2_intelDCP4511.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex2_3_intelDCP4511.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_intelDCP4511.txt' using 1:col with lines title columnheader

# E1 Toshiba
set output "ex1_1_toshibaVX500.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex1_2_toshibaVX500.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex1_3_toshibaVX500.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_toshibaVX500.txt' using 1:col with lines title columnheader

# E2 Toshiba
set output "ex2_1_toshibaVX500.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex2_2_toshibaVX500.png"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex2_3_toshibaVX500.png"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_toshibaVX500.txt' using 1:col with lines title columnheader

# E3
set output "ex3_1_samsung840.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex3_1_intelDCP4511.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex3_1_toshibaVX500.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_toshibaVX500.txt' using 1:col with lines title columnheader

# E4 TIME
set output "ex4_1_samsung840_time.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_samsung840_time.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_time.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_intelDCP4511_time.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_time.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_toshibaVX500_time.txt' using 1:col with lines title columnheader

# E4 MEMORY
set output "ex4_1_samsung840_mem.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych) [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_samsung840_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_mem.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych) [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_intelDCP4511_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_mem.png"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych) [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_toshibaVX500_memory.txt' using 1:col with lines title columnheader

# E5

set xtics rotate out
set style data histogram
set style fill solid border
set style histogram clustered


set output "ex5_1_toshibaVX500.png"
set title "Losowe wyszukiwania"
plot for [col=2:5] 'experiment5_1_toshibaVX500.txt' using col:xticlabels(1) title columnheader

set output "ex5_1_samsung840.png"
set title "Losowe wyszukiwania"
plot for [col=2:5] 'experiment5_1_samsung840.txt' using col:xticlabels(1) title columnheader

set output "ex5_1_intelDCP4511.png"
set title "Losowe wyszukiwania"
plot for [col=2:5] 'experiment5_1_intelDCP4511.txt' using col:xticlabels(1) title columnheader

