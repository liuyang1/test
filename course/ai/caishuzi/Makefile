drop: 1.png
	mv 1.png ${HOME}
e: e.hs
	ghc e.hs

1.dot: e
	./e > 1.dot
1.png: 1.dot
	dot -Tpng 1.dot > 1.png


a.out:main.cpp
	g++ -O3 -march=i686 -Wall -fopenmp main.cpp
clean:
	rm -f a.out *.out *.hi *.exe *.o
