max_subarray: max_subarray.c
	gcc -Wall -Werror max_subarray.c -o max_subarray

perf: max_subarray
	./max_subarray | tee perf.txt
	python plot.py

run: max_subarray
	./max_subarray

clean:
	rm -rf *.hi *.o MaxSubarray max_subarray core vgcore* a.out

MaxSubarray: MaxSubarray.hs
	ghc MaxSubarray
