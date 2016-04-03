TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint
TEST=test/wordnet

SRC=SAP.java WordNet.java Outcast.java

target: $(patsubst %.java,%.class,$(SRC))

test: Outcast.test

%.class: %.java
	@printf "javac %-20s => %s\n" $^ $@
	@javac -cp $(JARS) $(OPTS) $^

%.test: %.class
	@printf "run $@\n"
	@java -cp $(JARS) $(basename $^) $(TEST)/synsets.txt $(TEST)/hypernyms.txt $(TEST)/outcast5.txt $(TEST)/outcast8.txt $(TEST)/outcast11.txt

clean:
	rm -rf *.class *.zip *.log

download:
	[[ -d test ]] || mkdir test
	cd test && \
	wget http://coursera.cs.princeton.edu/algs4/testing/wordnet-testing.zip && \
	unzip wordnet-testing.zip && rm wordnet-testing.zip

style:
	clang-format -i *.java

check:
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class


wordnet.zip: WordNet.java SAP.java Outcast.java
	zip $@ $^ && mv $@ $(HOME)
