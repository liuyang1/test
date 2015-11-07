LIBS=-lpthread
sem: sem.c
	gcc $^ -o $@ $(LIBS)
semsrv:
	./sem server
semcln:
	./sem client

clean:
	rm -f sem
