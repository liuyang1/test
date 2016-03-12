TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint

target: RedBlackBST.class

RedBlackBST.class: RedBlackBST.java
	javac -cp $(JARS) $(OPTS) $^

RedBlackBST.test: RedBlackBST.class
	java -cp $(JARS) RedBlackBST

test: RedBlackBST.test

clean:
	rm -rf *.class
