PATH_AFL=${HOME}/git/pub/AFL
run: a.out
	AFL_SKIP_CPUFREQ=1 ${PATH_AFL}/afl-fuzz -i input -o output -- ./a.out

a.out: test_afl.c
	${PATH_AFL}/afl-gcc test_afl.c

clean:
	rm -rf a.out
