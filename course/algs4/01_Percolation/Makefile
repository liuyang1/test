TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8

PercolationStats.class: PercolationStats.java Percolation.java
	javac -cp $(JARS) $(OPTS) PercolationStats.java Percolation.java

run:
	make && java -cp $(JARS) PercolationStats 2 10000

test:
	java -cp $(JARS) PercolationStats 2 1000
	java -cp $(JARS) PercolationStats 2 1
	java -cp $(JARS) PercolationStats 1 1

clean:
	rm -rf *.class *.zip

PercolationVisualizer.class: PercolationVisualizer.java Percolation.java
	javac -cp $(JARS) $(OPTS) PercolationVisualizer.java Percolation.java

draw:
	java -cp $(JARS) PercolationVisualizer greeting57.txt

check:
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

perf:
	java -cp $(JARS) PercolationStats 200 1000

zip:
	zip percolation.zip Percolation.java PercolationStats.java && mv percolation.zip $(HOME)
