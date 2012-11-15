
CC=mips64-octeon-linux-gnu-g++
INC=-I/root/OCTEON-SDK/target/include -I/root/liuy/Xing-src/SendtoDataInfo/RtpModule -I/root/liuy/Xing-src/SendtoDataInfo/include
LIB=-L/opt/Xing-sendto/lib -linterface-linux_64
RPATH=-Wl,-rpath,/opt/Xing-sendto/lib

reID.out:main.o
	$(CC) $< $(RPATH) $(LIB) -o reID.out
main.o:main.cpp
	$(CC) -c $< $(INC) -o main.o
#DataInfo.o:DataInfo.cpp
#	$(CC) -c $< $(INC) -o DataInfo.o

clean:
	rm -f ./*.o *.out
