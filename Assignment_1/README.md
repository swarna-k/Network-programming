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


###Client



###Encoding

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

