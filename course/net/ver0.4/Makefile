a.out:np_server np_client
	
np_client:np_client.o np.o
	g++ -O2 -Wall -lncursesw -lpthread np_client.o np.o -o np_client
np_client.o:np_client.c
	g++ -O2 -Wall -c np_client.c
np_server:np_server.o np.o
	g++ -O2 -Wall -levent -lpthread np_server.o np.o -o np_server
np_server.o:np_server.cpp
	g++ -O2 -Wall -c np_server.cpp
np.o:np.c
	g++ -O2 -Wall -c np.c -o np.o
clean:
	rm -rf np_server np_client *.o 
