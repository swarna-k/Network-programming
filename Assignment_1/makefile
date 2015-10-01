# the build target executable:
CC = gcc
CFLAGS = -w
  
default: all
	
all: server client

encoding.o: encoding.c encoding.h
	$(CC) $(CFLAGS) -c encoding.c -o encoding.o
  
server.o: team3_server.c encoding.h
	$(CC) $(CFLAGS) -c team3_server.c -o server.o
  	 
client.o: team3_client.c
	$(CC) $(CFLAGS) -c team3_client.c -o client.o
	
server: server.o encoding.o
	$(CC) $(CFLAGS) server.o encoding.o -o server
	
client: client.o encoding.o
	$(CC) $(CFLAGS) client.o encoding.o -o client
	
clean: 
	rm -f server client *.o