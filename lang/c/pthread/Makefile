LIBS=-lpthread
timedjoin: pthread_timedjoin.c
	gcc $^ $(LIBS) -o $@
run:
	./timedjoin
clean:
	rm -rf timedjoin *.out *.o
