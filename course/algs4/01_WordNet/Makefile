TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint
TEST=test/kdtree

SRC=WordNet.java SAP.java Outcast.java

target: $(patsubst %.java,%.class,$(SRC))

test: WordNet.test

%.class: %.java
	@printf "javac %-20s => %s\n" $^ $@
	@javac -cp $(JARS) $(OPTS) $^

%.test: %.class
	@printf "run $@\n"
	@java -cp $(JARS) $(basename $^)

clean:
	rm -rf *.class *.zip *.log

download:
	[[ -d test ]] || mkdir test
	cd test && \
	wget http://coursera.cs.princeton.edu/algs4/testing/wordnet-testing.zip && \
	unzip wordnet-testing.zip && rm wordnet-testing.zip

wordnet.zip: WordNet.java SAP.java Outcast.java
	zip wordnet.zip $^
