# the build target executable:
CC = gcc
CFLAGS = -w
  
default: all
	
all: server client
	
client.o: http_client.c
	$(CC) $(CFLAGS) -c http_client.c -o client.o
  
server.o: team3_server.c 
	$(CC) $(CFLAGS) -c team3_server.c -o server.o
	
cache.o: cache.c 
	$(CC) $(CFLAGS) -c cache.c -o cache.o
	
server: server.o cache.o
	$(CC) $(CFLAGS) server.o cache.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

cache: cache.o
	$(CC) $(CFLAGS) cache.o -o cache
	
clean: 
	rm -f server client cache *.o 
	
cleancache:
	rm -f cachefile*.txt tempfile.txt