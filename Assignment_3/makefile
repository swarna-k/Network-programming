# the build target executable:
CC = gcc
CFLAGS = -w
  
default: all
	
all: server client
	
	
client.o: client.c
	$(CC) $(CFLAGS) -c client.c -o client.o
  
server.o: team3_server.c 
	$(CC) $(CFLAGS) -c team3_server.c -o server.o
	
server: server.o cache.o
	$(CC) $(CFLAGS) server.o cache.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

cache: cache.c 
	$(CC) $(CFLAGS) cache.c -o cache.o
	
clean: 
	rm -f server client *.o 
	
cleancache:
	rm -f cachefile*.txt tempfile.txt