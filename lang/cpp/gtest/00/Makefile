######################################################################
# @author      : lyz (lyz@$HOSTNAME)
# @file        : Makefile
# @created     : Saturday Oct 12, 2024 13:52:49 CST
######################################################################

CXX=g++
OPT=-Wall -Wextra
STD=-std=c++17
INC=-I /opt/homebrew/Cellar/googletest/1.15.2/include/
LIB=-L /opt/homebrew/Cellar/googletest/1.15.2/lib -lgtest -lgtest_main

a.out: sample00.cpp
	${CXX} ${OPT} ${STD} ${INC} ${LIB} $^

run:a.out
	./a.out

clean:
	rm -rf a.out
