SRC=math.c
DST=a.out
LIB=-lm

$(DST):$(SRC)
	gcc -Wall -Werror $^ $> $(LIB)

run:
	./$(DST)

clean:
	rm -rf $(DST)
