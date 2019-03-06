set terminal pdfcairo enhanced
set datafile separator "\t"

set style line 1 lw 3 lc rgb "green"
set style line 2 lw 3 lc rgb "blue"
set style line 3 lw 3 lc rgb "red"
set style line 4 lw 3 lc rgb "gold"
set style increment user

set key width -3 height 4
set key outside right top
set key samplen 1
set key spacing 1
set key font "Helvetica, 12"

set title font "Helvetica,16"
set ylabel font "Helvetica,14"
set xlabel font "Helvetica,14"

# E1 Samsung
set output "ex1_1_samsung840.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex1_2_samsung840.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_samsung840.txt' using 1:col with lines title columnheader

set output "ex1_3_samsung840.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_samsung840.txt' using 1:col with lines title columnheader

# E2 Samsung
set output "ex2_1_samsung840.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex2_2_samsung840.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_samsung840.txt' using 1:col with lines title columnheader

set output "ex2_3_samsung840.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_samsung840.txt' using 1:col with lines title columnheader

# E1 Intel
set output "ex1_1_intelDCP4511.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex1_2_intelDCP4511.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex1_3_intelDCP4511.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_intelDCP4511.txt' using 1:col with lines title columnheader


# E2 Intel
set output "ex2_1_intelDCP4511.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex2_2_intelDCP4511.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex2_3_intelDCP4511.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_intelDCP4511.txt' using 1:col with lines title columnheader

# E1 Toshiba
set output "ex1_1_toshibaVX500.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex1_2_toshibaVX500.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex1_3_toshibaVX500.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_toshibaVX500.txt' using 1:col with lines title columnheader

# E2 Toshiba
set output "ex2_1_toshibaVX500.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex2_2_toshibaVX500.pdf"
set title "Zawsze nowe dane, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_toshibaVX500.txt' using 1:col with lines title columnheader

set output "ex2_3_toshibaVX500.pdf"
set title "Sekwencyjny wzorzec, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_toshibaVX500.txt' using 1:col with lines title columnheader

set style line 1 lw 3 lc rgb "red"
set style line 2 lw 3 lc rgb "blue"
set style increment user

# E3
set output "ex3_1_samsung840.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex3_1_intelDCP4511.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex3_1_toshibaVX500.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment3_1_toshibaVX500.txt' using 1:col with lines title columnheader

# E4 TIME
set output "ex4_1_samsung840_time.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_samsung840_time.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_time.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_intelDCP4511_time.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_time.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:3] 'experiment4_1_toshibaVX500_time.txt' using 1:col with lines title columnheader

# E4 MEMORY
set output "ex4_1_samsung840_mem.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych)"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_samsung840_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_mem.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych)"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_intelDCP4511_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_mem.pdf"
set title "Losowe wyszukiwania, selektywność 5%"
set ylabel "Pamięć (ułamek całej bazy danych)"
set xlabel "Merge Treshold (ułamek całej bazy danych)"
plot for [col=2:2] 'experiment4_1_toshibaVX500_memory.txt' using 1:col with lines title columnheader

# E5

set xtics rotate out
set style data histogram
set style histogram clustered

set key width 0 height 4
set key outside right top
set key samplen 1
set key spacing 1
set key font "Helvetica, 8"

set style line 1 lw 3 lc rgb "#e21600"
set style line 2 lw 3 lc rgb "#9a0f02"
set style line 3 lw 3 lc rgb "#239fff"
set style line 4 lw 3 lc rgb "#232bff"
set style increment user
set style fill solid border

set output "ex5_1_toshibaVX500.pdf"
set title "Losowe wyszukiwania"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_toshibaVX500.txt' using col:xticlabels(1) ls col-1 title columnheader

set output "ex5_1_samsung840.pdf"
set title "Losowe wyszukiwania"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_samsung840.txt' using col:xticlabels(1) ls col-1 title columnheader

set output "ex5_1_intelDCP4511.pdf"
set title "Losowe wyszukiwania"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_intelDCP4511.txt' using col:xticlabels(1) ls col-1 title columnheader

