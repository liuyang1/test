TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8
TEST=test/percolation

target: ss

all: dq rq ss

test: dqtest rqtest sstest ssfreq

dq: Deque.class

Deque.class: Deque.java
	javac -cp $(JARS) $(OPTS) $^

rq: RandomizedQueue.class

RandomizedQueue.class: RandomizedQueue.java
	javac -cp $(JARS) $(OPTS) $^

ss: Subset.class

Subset.class: Subset.java
	javac -cp $(JARS) $(OPTS) $^

dqtest: Deque.class
	java -cp $(JARS) Deque

rqtest: RandomizedQueue.class
	java -cp $(JARS) RandomizedQueue

sstest: Subset.class RandomizedQueue.class
	echo A B C D E F G H I | java -cp $(JARS) Subset 3
	echo AA BB BB BB BB BB CC CC | java -cp $(JARS) Subset 8
	echo AA BB BB BB BB BB CC CC | java -cp $(JARS) Subset 5
	echo A A B C C C C D | java -cp $(JARS) Subset 4

ssfreq: Subset.class RandomizedQueue.class
	@echo "should 2 : 4 : 1 : 1"
	python2 test_subset.py

clean:
	rm -rf *.class *.zip
check: all
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

zip: 
	zip queues.zip Deque.java RandomizedQueue.java Subset.java && mv queues.zip $(HOME)
