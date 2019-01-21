all: UDP_fork_server.o UDP_fork_client.o

UDP_fork_client.o: UDP_client.c	
	gcc UDP_client.c -o UDP_fork_client.o

UDP_fork_server.o: UDP_fork_server.c
	gcc UDP_fork_server.c -o UDP_fork_server.o