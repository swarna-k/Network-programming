#Team 3 Assignment 1: SBCP Client and Server

##Compile and Run
To compile simply navigate to the directory with the source files and type 
 
>make 
 
The default make compiles both the client and server.
 
>make client 
 
Can be used to create just the client
 
>make server 
 
Can be used to compile only the server
##Code Description
 
###Server


###Client - Aditya Sharma

USAGE
1. Open terminal.
2. Login to the server. Login format is <client username server_IP port_number>
3. Username should be 16 characters long at most(including enter key).
4. Once you have logged in, you will be able to send and receive messages.
5. To disconnect from the chatroom terminate the connection(Ctrl+C).

The client code takes in four arguments during connection. The format of the joining message to the chatroom is <client username server_IP port_number >. The code then creates a socket and connects to the server. Using this connection, a JOIN message is sent to the server which will then add the client to the chat room. Once the client is in the chatroom, we are 
able to see whether there is any data to be read using the ISSET functionality of the select command. If there is data to be read from the keyboard, the data is read and packed and sent to the server. If there is data to be read from the server, the client reads it and displays it on the terminal.

###Encoding - Mandel Oats

The Server and Client use external encoding and decoding functions and structs for processing messages as defined by the SBCP. 
These functions were developed seperate of the Client and Server in order test the pointer arithmetic, 
and track down pesky segfault errors in the development process, and to prevent redundant code in the server and client. 

In the encoding.h file you will find function definitions for encoding a message, decoding a message header, and decoding a message.
You will also find structs defining the message headers and attribute headers. 

The implementation in the encoding.c file is fairly straightforward. 
For encoding a message, the necessary pointers are passed into the fuction, and the size of the message is calculated. This size is then allocated in heap memory, 
and the pointer values are copied into one seamless address space which is returned. The only drawback to this design is that the msg pointer must be freed after calling this function in order to prevent a memory leak.
The decode message function was orignially one function, but it came to our attention we would need the size of the message from the header before being able to read the message. 
So, a decode message header function was created to decode the header seperately. The decode message checks the message type in order to see if 2 attributes are available (an array of pointers instead of more arguments would have been a better design).

