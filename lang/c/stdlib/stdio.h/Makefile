freopen: testFreopen.c
	gcc -DFREOPEN $^
	strace ./a.out 1> 1.stdout 2> 1.strace
	gcc $^
	strace ./a.out 1> 0.stdout 2> 0.strace
	vimdiff *.strace
	vimdiff *.stdout

clean:
	rm -rf a.out
	rm -rf *.strace *.stdout
