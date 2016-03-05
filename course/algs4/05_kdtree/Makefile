TOP=../
JARS=.:$(HOME)/.local/jar/algs4.jar
OPTS=-g -encoding UTF-8 -Xlint
TEST=test/kdtree

target: PointSET.class \
	KdTree.class \
	KdTreeVisualizer.class \
	NearestNeighborVisualizer.class \
	RangeSearchVisualizer.class


KdTreeVisualizer.class: KdTreeVisualizer.java
	javac -cp $(JARS) $(OPTS) $^

NearestNeighborVisualizer.class: NearestNeighborVisualizer.java
	javac -cp $(JARS) $(OPTS) $^

RangeSearchVisualizer.class: RangeSearchVisualizer.java
	javac -cp $(JARS) $(OPTS) $^

PointSET.class: PointSET.java
	javac -cp $(JARS) $(OPTS) $^

KdTree.class: KdTree.java
	javac -cp $(JARS) $(OPTS) $^

KdTreeVisualizer.test: KdTreeVisualizer.class
	java -cp $(JARS) KdTreeVisualizer

test: target KdTreeVisualizer.test

clean:
	rm -rf *.class *.zip *log

check: target
	# clang-format-3.6 -i PointSET.java
	$(TOP)/bin/checkstyle-algs4 *.java
	$(TOP)/bin/findbugs-algs4 *.class

zip:
	zip kdtree.zip PointSET.java KdTree.java
