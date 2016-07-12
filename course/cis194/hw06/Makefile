hw06: hw06.hs
	ghc hw06.hs -rtsopts -main-is slow

run:hw06
	echo ">>> simple run"
	time ./hw06 +RTS -K100M -RTS
	echo ">>> run to check memory using"
	./hw06 +RTS -K100M -s
	echo ">>> run to check heap profile"
	./hw06 +RTS -K100M -h -i0.001
	hp2ps -c hw06.hp

clean:
	rm -rf *.hi *.o hw06 *.aux *.hp *.ps
