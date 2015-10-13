# the build target executable:
CC = gcc
CFLAGS = -w
  
default: all
	
all: server

encoding.o: encoding.c encoding.h
	$(CC) $(CFLAGS) -c encoding.c -o encoding.o
  
server.o: team3_server2.c 
	$(CC) $(CFLAGS) -c team3_server2.c -o server.o
	
server: server.o encoding.o
	$(CC) $(CFLAGS) server.o encoding.o -o server
	
encoding: encoding.o 
	$(CC) $(CFLAGS) encoding.o -o encoding
	
clean: 
	rm -f server *.o