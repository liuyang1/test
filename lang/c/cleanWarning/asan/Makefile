CFLAGS=-static-libstdc++ -static-libasan -O -g -fsanitize=address -fno-omit-frame-pointer
gcc_static:
	g++ -o gcc_static_test $(CFLAGS) test.c
	ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) ./gcc_static_test

clean:
	rm -rf gcc_static_test
