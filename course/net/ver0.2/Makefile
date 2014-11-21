a.out:np_server np_client
	
np_client:np_client.c
	gcc -O2 -Wall -lpthread np_client.c -o np_client
np_server:np_server.c
	gcc -O2 -Wall -lpthread np_server.c -o np_server
clean:
	rm -rf np_server np_client
