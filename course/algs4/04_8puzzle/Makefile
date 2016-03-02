TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint
TEST=test/8puzzle

target: Board.class Solver.class

Board.class: Board.java
	javac -cp $(JARS) $(OPTS) $^

Board.test: Board.class
	java -cp $(JARS) Board

Solver.class: Solver.java
	javac -cp $(JARS) $(OPTS) $^

Solver.test: Solver.class
	java -cp $(JARS) Solver $(TEST)/input8.txt

# test: Board.test
test: Solver.test

clean:
	rm -rf *.class *.zip *log

check: target
	# clang-format-3.6 -i *.java
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

zip:
	zip 8puzzle.zip Solver.java Board.java
