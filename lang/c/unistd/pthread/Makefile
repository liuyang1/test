LIBS=-lpthread
all: timedjoin join

timedjoin: pthread_timedjoin.c
	gcc $^ $(LIBS) -o $@
join: pthread_join.c
	gcc $^ $(LIBS) -o $@
run:
	./timedjoin
clean:
	rm -rf timedjoin join *.out *.o
