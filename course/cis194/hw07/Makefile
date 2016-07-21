pre:
	cabal update
	cabal install MonadRandom
	cabal install vector
	wget http://www.seas.upenn.edu/~cis194/extras/07-monads/Cards.hs

test:
	doctest hw07.hs

doc:hw07.hs
	haddock -h hw07.hs -o doc

clean:
	rm -rf doc
