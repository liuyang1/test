CXXFLAGS=-Wall -g -Og #-fsanitize=address -lasan
test: a.out
	./a.out

grind: a.out
	valgrind ./a.out

a.out: buddyString.859.cpp
	g++ $(CXXFLAGS) $^ -std=c++11

clean:
	rm -rf *.o a.out core vgcore*
