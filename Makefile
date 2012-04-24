all: compile random sorted reversed plot plot_compare

random:
	./qsort_random 262144 qsort_random.data

sorted:
	./qsort_sorted 32768 qsort_sorted.data

reversed:
	./qsort_reversed 32768 qsort_reversed.data

compile:
	gcc -o qsort_random testsort.c qsort.c time.c
	gcc -o qsort_sorted testsort_sorted.c qsort.c time.c
	gcc -o qsort_reversed testsort_reversed.c qsort.c time.c

plot_compare:
	gnuplot sortperf_compare.plot
	gv sortperf_compare.ps

plot:
	gnuplot sortperf_qsort.plot
	gv sortperf_qsort.ps

clean:
	rm -f *.o
	rm -f *~
	rm -f ./qsort_random ./qsort_sorted ./qsort_reversed
	rm -f ./qsort_random.data ./qsort_sorted.data ./qsort_reversed.data
	rm -f *.ps
