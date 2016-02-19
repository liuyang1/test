TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8
TEST=test/colinear

target: Point.class

Point.class: Point.java
	javac -cp $(JARS) $(OPTS) $^

clean:
	rm -rf *.class *.zip
