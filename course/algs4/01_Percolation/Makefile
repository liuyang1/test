TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8
TEST=test/percolation

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

draw: PercolationVisualizer.class
	# java -cp $(JARS) PercolationVisualizer $(TEST)/input2.txt
	java -cp $(JARS) PercolationVisualizer $(TEST)/sedgewick60.txt
	# java -cp $(JARS) PercolationVisualizer $(TEST)/input7.txt
	# java -cp $(JARS) PercolationVisualizer $(TEST)/greeting57.txt

dr14: PercolationVisualizer.class
	java -cp $(JARS) PercolationVisualizer test14.txt


check: PercolationStats.class
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

perf: PercolationStats.class
	java -cp $(JARS) PercolationStats 200 100
	java -cp $(JARS) PercolationStats 200 200
	java -cp $(JARS) PercolationStats 400 100

zip:
	zip percolation.zip Percolation.java PercolationStats.java && mv percolation.zip $(HOME)

regression: PercolationVisualizer.class
	java -cp $(JARS) PercolationVisualizer $(TEST)/input6.txt
	java -cp $(JARS) PercolationVisualizer $(TEST)/input10.txt
	java -cp $(JARS) PercolationVisualizer $(TEST)/input20.txt
	java -cp $(JARS) PercolationVisualizer $(TEST)/input2.txt
	make run

except:
	# java -cp $(JARS) PercolationStats -23 42
	-java -cp $(JARS) PercolationStats 23 0
	-java -cp $(JARS) PercolationStats -42 0
	-java -cp $(JARS) PercolationStats 42 -1

ref: PercolationStats.class
	java -cp $(JARS) PercolationStats 20 10
