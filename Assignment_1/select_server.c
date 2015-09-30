/**
	Handle multiple socket connections with select and fd_set on Linux
	
	Silver Moon ( m00n.silv3r@gmail.com)
*/
 
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "encoding.h"
 
#define TRUE   1
#define FALSE  0
#define PORT 8888
int checkname(char usernames[30][16], char username[16]);
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients , activity, i , j , valread , sd, sdd;
    int portno;
	int max_sd;
    struct sockaddr_in address;
    struct msg_header msg_hdr_recv;
    struct attr_header attr_hdr_recv; 
    char buffer[250];  //data buffer of 1K
    char buffer1[250];
    char buffer2[250];
    char reason[32];
    char tempbuffer[250];  //data buffer of 1K
    char usernames[30][16]; //list of names 
    char exit_name[16];
    char* msg;

	char recv_buffer[1024];
    struct msg_header msg_hdr_send = {.vrsn = 3, .type = 4, .length = 4}; //Type is 3 or 7

    struct attr_header attr_hdr_send1 = {.type = 2, .length = 25};
    struct attr_header attr_hdr_send2 = {.type = 4, .length = 26};
    

    //set of socket descriptors
    fd_set readfds;
     
    //a message
    char *message = "You are now connected \r\n";
 
    //initialise all client_socket[] to 0 so not checked
    if (argc < 4)
    {

        fprintf(stderr,"ERROR, type in address portnum max_clients\n");
        exit(1);
    }
	max_clients = atoi(argv[3]);

    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
    //initialise all usernames[] to '0' so not checked
     for (i = 0; i < max_clients; i++) 
    {
        strcpy(usernames[i],"0 \n");
    } 
    

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    //set master socket to allow multiple connections 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
 
    bzero((char *) &address, sizeof(address));

    portno = atoi(argv[2]);

    //printf("%s \n", argv[2]);
	


    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons( portno );
     
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	printf("Listener on port %d \n", portno);
	
    //specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
     
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    
	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
 
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
		
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
			sd = client_socket[i];
            
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }
 
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
         
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
         
            
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
       
           
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
					
					break;
                }
            }
        }
         
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
            
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read message header

				
				
                
                if ((valread = read( sd , recv_buffer, 4) )== 0)

                {
                    //Somebody disconnected , get his details and print , free all the resources
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    close( sd );
                  
                    printf("%d",i);
                    puts("closing\n");
                    client_socket[i] = 0;
                    strcpy(exit_name,usernames[i]);
                    strcpy(usernames[i],"0 \n");
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    
                    //OFFLINE

                    //send offline meassage to all other clients
                     for (j = 0; j < max_clients; j++) 
                        {
                            if(j!=i){
                                sdd = client_socket[j];
                                strcpy(buffer1,exit_name); 
                              //  strcpy(buffer2,buffer);
                                msg_hdr_send.vrsn = 3; 
                                msg_hdr_send.type = 6; 
                                msg_hdr_send.length = 4; //Type is 3 or 7
                                attr_hdr_send1.type = 2; 
                                attr_hdr_send1.length = 25;
                               // attr_hdr_send2.type = 4; 
                               // attr_hdr_send2.length = 26;
    
    
                                msg = encode_msg(&msg_hdr_send, &attr_hdr_send1, NULL, &buffer1, NULL);
    
                                send(sdd , msg , msg_hdr_send.length , 0 );
                                free(msg);
                            }                    
                        } 
                    //Close the socket and mark as 0 in list for reuse
                    
                    
                    break;

                    
                }
                
               
                else
                {
                    printf("%d \n",strlen(recv_buffer));
                    //decode message
					decode_msg_header(recv_buffer,&msg_hdr_recv);
					bzero(recv_buffer,1024);
					
                    valread = read( sd , recv_buffer, msg_hdr_recv.length-4);

					int type = (int)msg_hdr_recv.type;
					
                    
                    decode_msg(recv_buffer,&type,&attr_hdr_recv, NULL, &buffer, NULL);
					
                   if(msg_hdr_recv.type==4 && attr_hdr_recv.type==4)
                   {
                        //FORWARD
                        printf("%d \n",i);
                        puts("forwarding message");
                        
                        printf("%s : %s \n",usernames[i], buffer); 
                        for (j = 0; j < max_clients; j++) 
                        {
                            if(j!=i){
                                sdd = client_socket[j];
                                strcpy(buffer1,usernames[i]); 
                                strcpy(buffer2,buffer);
                                msg_hdr_send.vrsn = 3; 
                                msg_hdr_send.type = 3; 
                                msg_hdr_send.length = 4; 
                                attr_hdr_send1.type = 2; 
                                attr_hdr_send1.length = 25;
                                attr_hdr_send2.type = 4; 
                                attr_hdr_send2.length = 26;
    
    
                                msg = encode_msg(&msg_hdr_send, &attr_hdr_send1, &attr_hdr_send2, &buffer1, &buffer2);
    
                                send(sdd , msg , msg_hdr_send.length , 0 );
                                free(msg);
                            }                    
                        }
                    }else{
                        
                        //JOIN
                        if(checkname(usernames,buffer)==TRUE){
                        
                        puts("user joining");
                        printf("%lu",strlen(usernames[0]));
                       
                        strcpy(usernames[i],buffer);
                        printf("%s joined the chat room \n",buffer); 

                        //ONLINE

                        for (j = 0; j < max_clients; j++) 
                        {
                            if(j!=i){
                                sdd = client_socket[j];
                                strcpy(buffer1,usernames[i]); 
                              //  strcpy(buffer2,buffer);
                                msg_hdr_send.vrsn = 3; 
                                msg_hdr_send.type = 8; 
                                msg_hdr_send.length = 4; 
                                attr_hdr_send1.type = 2; 
                                attr_hdr_send1.length = 25;
                               // attr_hdr_send2.type = 4; 
                               // attr_hdr_send2.length = 26;
    
    
                                msg = encode_msg(&msg_hdr_send, &attr_hdr_send1, NULL, &buffer1, NULL);
    
                                send(sdd , msg , msg_hdr_send.length , 0 );
                                free(msg);
                            }                    
                        }
                       
                        
                        }else{
                                //if adding to chatroom failed, close the socket
                                close( sd );
                               
                        }

                    }
                }
 
            }
        }
    }
     
    return 0;
} 

int checkname(char usernames[30][16],char username[16]){
    int i;
    for(i=0; i<30 ; i++){
        if(strcmp(usernames[i],username)==FALSE){
            return FALSE;
        }       
    }   
    return TRUE;
}

