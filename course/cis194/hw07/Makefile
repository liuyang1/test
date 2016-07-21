all: hw07

pre:
	cabal update
	cabal install MonadRandom
	cabal install vector
	cabal install doctest
	wget http://www.seas.upenn.edu/~cis194/extras/07-monads/Cards.hs
	wget http://www.seas.upenn.edu/~cis194/extras/07-monads/HW07.hs

hw07:
	ghc hw07.hs -main-is hw07

run:hw07
	./hw07

test:
	doctest hw07.hs

doc:
	haddock -h hw07.hs -o doc

web:
	cd doc; python2 -m SimpleHTTPServer

clean:
	rm -rf doc hw07 *.hi *.o
