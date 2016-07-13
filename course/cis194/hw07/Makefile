pre:
	cabal update
	cabal install MonadRandom
	cabal install vector

test:
	doctest hw07.hs

doc:hw07.hs
	haddock -h hw07.hs -o doc

clean:
	rm -rf doc
