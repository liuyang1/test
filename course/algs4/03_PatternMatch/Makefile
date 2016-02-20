TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8
TEST=test/colinear

target: Point.class

Point.class: Point.java
	javac -cp $(JARS) $(OPTS) $^

Point.test: Point.class
	java -cp $(JARS) Point

test: Point.test

clean:
	rm -rf *.class *.zip

check: target
	# clang-format-3.6 -i *.java
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class
