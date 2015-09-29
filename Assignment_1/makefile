# the build target executable:
CC = gcc
CFLAGS = -w
  
default: all
	
all: server client

encoding.o: encoding.c encoding.h
	$(CC) $(CFLAGS) -c encoding.c -o encoding.o
  
server.o: select_server.c encoding.h
	$(CC) $(CFLAGS) -c select_server.c -o server.o
  	 
client.o: client_code.c
	$(CC) $(CFLAGS) -c client_code.c -o client.o
	
server: server.o encoding.o
	$(CC) $(CFLAGS) server.o encoding.o -o server
	
client: client.o encoding.o
	$(CC) $(CFLAGS) client.o encoding.o -o client
	
clean: 
	rm -f server client *.o