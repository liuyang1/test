Dice:Dice.hs
	ghc $< -rtsopts
	./Dice +RTS -sstderr

test:
	./Dice +RTS -p -K100M -sstderr

dragon:dragon.hs
	ghc $< -o $@
	./dragon > dragon.logo
	ucblogo dragon.logo

hilbert:HilbertC.hs
	ghc $< -o $@
	./hilbert > hilbert.logo
	ucblogo hilbert.logo

clean:
	rm -rf *.hi *.logo dragon *.o hilbert
