TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint
TEST=test/collinear

target: Point.class BruteCollinearPoints.class FastCollinearPoints.class

Point.class: Point.java
	javac -cp $(JARS) $(OPTS) $^

Point.test: Point.class
	java -cp $(JARS) Point

BruteCollinearPoints.class: BruteCollinearPoints.java
	javac -cp $(JARS) $(OPTS) $^

BruteCollinearPoints.test: BruteCollinearPoints.class
	java -cp $(JARS) BruteCollinearPoints $(TEST)/input8.txt
	# java -cp $(JARS) BruteCollinearPoints $(TEST)/input6.txt

FastCollinearPoints.class: FastCollinearPoints.java
	javac -cp $(JARS) $(OPTS) $^

FastCollinearPoints.test: FastCollinearPoints.class
	java -cp $(JARS) FastCollinearPoints $(TEST)/input9.txt

perf: FastCollinearPoints.class
	java -cp $(JARS) FastCollinearPoints $(TEST)/input4000.txt

basictest: Point.test

# test: basictest BruteCollinearPoints.test FastCollinearPoints.test
test: FastCollinearPoints.test

clean:
	rm -rf *.class *.zip *log

check: target
	# clang-format-3.6 -i *.java
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

zip:
	zip 000collinear.zip BruteCollinearPoints.java FastCollinearPoints.java Point.java && scp 000collinear.zip pc:~/
