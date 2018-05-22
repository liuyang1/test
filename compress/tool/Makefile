FLAGS=-Wall -Werror

all: fv entropy word

fv: fv.cpp
	g++ $(FLAGS) fv.cpp -o fv

entropy: entropy.c
	gcc $(FLAGS) entropy.c -o entropy -lm

word: word.c
	gcc $(FLAGS) word.c -o word

clean:
	rm -rf fv entropy word
