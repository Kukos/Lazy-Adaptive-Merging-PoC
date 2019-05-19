# Grafika wektorowa jako pdf
set terminal pdfcairo enhanced
set datafile separator "\t"
set termoption font ',16'
# Kolory wykresow, mozna podac kod rgb w hexach po #
# Format: line [numer linii czyli kolumna w .txt $k dostanie $k kolor] lw [szerokosc linii] lc [tryb koloru u nas rgb] [nazwa predefiniowana albo kod]
set style line 1 lw 5 lc rgb "green"
set style line 2 lw 3 lc rgb "blue"
set style line 3 lw 3 lc rgb "red"
set style line 4 lw 3 lc rgb "gold"
set style increment user

# Ustawienie legendy na zewnatrzm domyslnie 0,0 ale ja chce blizej obrazka
set key width -2 height 0
set key outside right top

# Dlugosc linii legendy jako przyklad
set key samplen 1
set key spacing 1

# Rozmiar OX i OY (numerow)
set tics font ", 20"

# Fonty legendy
set key font "Helvetica, 24"

# Fonty tytulu czyli tego nad obrazkiem
set title font "Helvetica, 28"

# Fonty osi OY
set ylabel font "Helvetica, 24"

# Fonty osi OX
set xlabel font "Helvetica, 24"

# Co ile beda te kreski OX i OY
set ytics 1

# E1 Samsung
set xtics 15
set output "ex1_1_samsung840.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_samsung840.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex1_2_samsung840.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_samsung840.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex1_3_samsung840.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_samsung840.txt' using 1:col with lines title columnheader


# E1 Intel
set xtics 15
set output "ex1_1_intelDCP4511.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_intelDCP4511.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex1_2_intelDCP4511.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_intelDCP4511.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex1_3_intelDCP4511.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_intelDCP4511.txt' using 1:col with lines title columnheader

# E1 Toshiba
set xtics 15
set output "ex1_1_toshibaVX500.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_1_toshibaVX500.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex1_2_toshibaVX500.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_2_toshibaVX500.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex1_3_toshibaVX500.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:4] 'experiment1_3_toshibaVX500.txt' using 1:col with lines title columnheader

# E2 Intel
set xtics 15
set output "ex2_1_intelDCP4511.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_intelDCP4511.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex2_2_intelDCP4511.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_intelDCP4511.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex2_3_intelDCP4511.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_intelDCP4511.txt' using 1:col with lines title columnheader

# E2 Samsung
set xtics 15
set output "ex2_1_samsung840.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_samsung840.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex2_2_samsung840.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_samsung840.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex2_3_samsung840.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_samsung840.txt' using 1:col with lines title columnheader


# E2 Toshiba
set xtics 15
set output "ex2_1_toshibaVX500.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_1_toshibaVX500.txt' using 1:col with lines title columnheader

set xtics 4
set output "ex2_2_toshibaVX500.pdf"
set title "W. Nowych Kluczy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_2_toshibaVX500.txt' using 1:col with lines title columnheader

set xtics 10
set output "ex2_3_toshibaVX500.pdf"
set title "W. Sekwencyjny, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Kwerenda"
plot for [col=2:5] 'experiment2_3_toshibaVX500.txt' using 1:col with lines title columnheader

set style line 1 lw 3 lc rgb "red"
set style line 2 lw 3 lc rgb "blue"
set style increment user

# Rozmiar OX i OY (numerow)
set tics font ", 16"
set xtics 0.02
set ytics 15
set key width -1 height 0

# E3
set output "ex3_1_samsung840.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ratio)"
plot for [col=2:2] 'experiment3_1_samsung840.txt' using 1:col with lines title columnheader

set output "ex3_1_intelDCP4511.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ratio)"
plot for [col=2:2] 'experiment3_1_intelDCP4511.txt' using 1:col with lines title columnheader

set output "ex3_1_toshibaVX500.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Rozmiar bufora (ratio)"
plot for [col=2:2] 'experiment3_1_toshibaVX500.txt' using 1:col with lines title columnheader

# Rozmiar OX i OY (numerow)
set tics font ", 16"

set key width -2 height 0

# Fonty legendy
set key font "Helvetica, 22"

# Fonty tytulu czyli tego nad obrazkiem
set title font "Helvetica, 28"

# Fonty osi OY
set ylabel font "Helvetica, 22"

# Fonty osi OX
set xlabel font "Helvetica, 22"

set ytics 30

# E4 TIME
set output "ex4_1_samsung840_time.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:3] 'experiment4_1_samsung840_time.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_time.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:3] 'experiment4_1_intelDCP4511_time.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_time.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Czas [s]"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:3] 'experiment4_1_toshibaVX500_time.txt' using 1:col with lines title columnheader

set ytics 0.02
set key width -3 height 0

# E4 MEMORY
set output "ex4_1_samsung840_mem.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Pamięć (ratio)"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:2] 'experiment4_1_samsung840_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_intelDCP4511_mem.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Pamięć (ratio)"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:2] 'experiment4_1_intelDCP4511_memory.txt' using 1:col with lines title columnheader

set output "ex4_1_toshibaVX500_mem.pdf"
set title "W. Losowy, selektywność 5%"
set ylabel "Pamięć (ratio)"
set xlabel "Merge Treshold (ratio)"
plot for [col=2:2] 'experiment4_1_toshibaVX500_memory.txt' using 1:col with lines title columnheader

# E5
# Zmiana na histogram
set xtics rotate out
set style data histogram
set style histogram clustered

# Inne ustawienia bo sie nie miesci legenda
set key width -2 height 0
set key outside right top
set key samplen 1
set key spacing 1
set ytics 50

# Rozmiar OX i OY (numerow)
set tics font ", 16"

# Fonty legendy
set key font "Helvetica, 18"

# Fonty tytulu czyli tego nad obrazkiem
set title font "Helvetica, 28"

# Fonty osi OY
set ylabel font "Helvetica, 22"

# Fonty osi OX
set xlabel font "Helvetica, 22"

# Kolory - rozne odcienie dlatego w HEXACH
set style line 1 lw 3 lc rgb "#e21600"
set style line 2 lw 3 lc rgb "#9a0f02"
set style line 3 lw 3 lc rgb "#239fff"
set style line 4 lw 3 lc rgb "#232bff"
set style increment user
set style fill solid border

set output "ex5_1_toshibaVX500.pdf"
set title "W. Losowy"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_toshibaVX500.txt' using col:xticlabels(1) ls col-1 title columnheader

set output "ex5_1_samsung840.pdf"
set title "W. Losowy"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_samsung840.txt' using col:xticlabels(1) ls col-1 title columnheader

set output "ex5_1_intelDCP4511.pdf"
set title "W. Losowy"
set ylabel "Czas [s]"
set xlabel "Selektywność"
plot for [col=2:5] 'experiment5_1_intelDCP4511.txt' using col:xticlabels(1) ls col-1 title columnheader

