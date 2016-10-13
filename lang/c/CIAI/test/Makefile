TOP=$(shell git rev-parse --show-toplevel 2>/dev/null)
GTESTDIR=$(TOP)/ext/googletest

TARGET=ciai.test
BUILDTYPE=c++

all: gtest $(TARGET)
gtest:
	@echo "build gtest"
	cd $(GTESTDIR) && cmake CMakeLists.txt && make

CPPFLAGS:=-I$(GTESTDIR)/googletest/include
LDFLAGS:=-L$(GTESTDIR)/googlemock/gtest/ -lgtest -lpthread

include ../../template.mk
