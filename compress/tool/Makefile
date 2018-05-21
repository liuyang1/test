FLAGS=-Wall -Werror

all: fv entropy

fv: fv.cpp
	g++ $(FLAGS) fv.cpp -o fv

entropy: entropy.c
	gcc $(FLAGS) entropy.c -o entropy -lm

clean:
	rm -rf fv entropy
