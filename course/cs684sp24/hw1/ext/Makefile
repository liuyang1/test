######################################################################
# @author      : lyz (lyz@dev-dsk-lyz-2c-30cffada.us-west-2.amazon.com)
# @file        : Makefile
# @created     : Friday May 17, 2024 21:38:28 CST
######################################################################

stmt: ./my*.avl
	coverage run --append avl.py simple1.avl
	coverage run --append avl.py simple2.avl
	for file in $^ ; do \
		coverage run --append avl.py $$file; \
	done
	coverage run --append avl.py
	coverage report -m

br: ./my*.avl
	coverage run --append --branch avl.py simple1.avl
	coverage run --append --branch avl.py simple2.avl
	for file in $^ ; do \
		coverage run --append --branch avl.py $$file; \
	done
	coverage run --append --branch avl.py
	coverage report -m

one: ./my12.avl
	coverage erase
	coverage run --append avl.py $^
	coverage report -m

clean:
	coverage erase
