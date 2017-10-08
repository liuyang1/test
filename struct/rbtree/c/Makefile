CFLAGS = -Wall

test_rbtree: test_rbtree.o rbtree.o
	gcc -o $@ $^


test_rbtree.o: test_rbtree.c rbtree.h
	gcc $(CFLAGS) -o $@ -c $<

rbtree.o: rbtree.c rbtree.h
	gcc $(CFLAGS) -o $@ -c $<

test: test_rbtree
	./test_rbtree

clean:
	rm -rf *.o test_rbtree
