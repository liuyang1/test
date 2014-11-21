TARGET=load
TYPE=cpp
CPPFLAGS= -Wall 
${TARGET}.out:${TARGET}.${TYPE}
	g++ ${CPPFLAGS} $< -o $@
	
clean:
	rm -rf ./*.o ${TARGET}.out
run:
	./${TARGET}.out
