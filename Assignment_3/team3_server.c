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
#include <netdb.h>
#include "cache.h"

#define TRUE   1
#define FALSE  0
#define PORT 8888

int main(int argc , char *argv[])
{
        int opt = TRUE;
        int master_socket , addrlen , new_socket,web_socket[30] , client_socket[30] , max_clients , activity, i , j , valread , sd, sdd, sockfd1;
        int portno;
        int max_sd;
        struct hostent* host;
        struct sockaddr_in address;
        struct sockaddr_in web_address[30];
        char t1[300],t2[10],t3[300],filename[30];
    char cache_buffer[250];  //data buffer of 1K

    char send_buffer[250];  //data buffer of 1K


    char recv_buffer[1024];

    
    max_clients=30;
    //set of socket descriptors
    fd_set readfds;

    //a message
    char *message = "You are now connected \r\n";

    //initialise all client_socket[] to 0 so not checked
    if (argc < 2)
    {

        fprintf(stderr,"ERROR, type in port no.\n");
        exit(1);
    }
	//max_clients = atoi(argv[3]);

    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
    for (i = 0; i < max_clients; i++) 
    {
        web_socket[i] = 0;
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

             char* temp=NULL;
             int flag=0;
             int port=0;
             int n;
             bzero((char*)recv_buffer,500);
             int index;

                
                if((valread=recv( sd , recv_buffer, 250, 0))!=0){

                //puts("got data");
                sscanf(recv_buffer,"%s %s %s",t1,t2,t3);
                //puts("split data");

                if(((strncmp(t1,"GET",3)==0))&&((strncmp(t3,"HTTP/1.1",8)==0)||(strncmp(t2,"HTTP/1.0",8)==0))&&(strncmp(t3,"http://",7)==0)){

                    strcpy(t1,t3);

                    flag=0;

                    for(i=7;i<strlen(t3);i++)
                    {
                        if(t3[i]==':')
                        {
                            flag=1;
                            break;
                        }
                    }

                    temp=strtok(t3,"//");
                    if(flag==0)
                    {
                        port=80;
                        temp=strtok(NULL,"/");
                    }
                    else
                    {
                        temp=strtok(NULL,":");
                    }

                    sprintf(t3,"%s",temp);
                    printf("host = %s",t3);
                    host=gethostbyname(t3);

                    if(flag==1)
                    {
                        temp=strtok(NULL,"/");
                        port=atoi(temp);
                    }


                    strcat(t1,"^]");
                    temp=strtok(t1,"//");
                    temp=strtok(NULL,"/");
                    if(temp!=NULL)
                        temp=strtok(NULL,"^]");
                    
                    printf("\npage = %s",temp);

               if((index=checkCache(t3,temp))==-1){

                    bzero((char*)&web_address[i],sizeof(web_address[i]));
                    web_address[i].sin_port=htons(port);
                    web_address[i].sin_family=AF_INET;
                    bcopy((char*)host->h_addr,(char*)&web_address[i].sin_addr.s_addr,host->h_length);

                    sockfd1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
                    sdd=connect(sockfd1,(struct sockaddr*)&web_address[i],sizeof(struct sockaddr));
                    sprintf(send_buffer,"\nConnected to %s  IP - %s\n",t2,inet_ntoa(web_address[i].sin_addr));
                    if(sdd<0)
                        perror("Error in connecting to remote server");

                    //sprintf(send_buffer,"path = %s   Port = %d",t3,port);
                    //send(sd,send_buffer,strlen(send_buffer),0);
                    
                    printf("\n%s\n",send_buffer);
    //send(newsockfd,buffer,strlen(buffer),0);
                    bzero((char*)send_buffer,sizeof(send_buffer));
                    if(temp!=NULL)
                        sprintf(send_buffer,"GET /%s %s\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n",temp,t2,t3);
                    else
                        sprintf(send_buffer,"GET / %s\r\nHost: %s\r\nConnection: Keep-Alive\r\n\r\n",t2,t3);


                    n=send(sockfd1,send_buffer,strlen(send_buffer),0);
                    //printf("\n%s\n",buffer);
                    if(n<0)
                        perror("Error writing to socket");
                    else{
                        FILE *ftemp;
                        ftemp = fopen("tempfile.txt","w");
                        do
                        {
                            bzero((char*)send_buffer,500);
                            n=recv(sockfd1,send_buffer,500,0);
                            if(!(n<=0)){
                                fwrite(send_buffer , 1 , sizeof(send_buffer) , ftemp );
                                send(sd,send_buffer,n,0);
                            }    
                        }while(n>0);
                        fclose(ftemp);
                        bzero((char*)send_buffer,500);
                       // sprintf(send_buffer,"end of file");
                        cacheItem(t3,temp); //MO
                        //send(sd,send_buffer,strlen(send_buffer),0);
                    }
                }
                else{
                    
                            FILE *fp;
                            getCachedItem(filename,index);
                            
                            fp = fopen(filename,"r");
                            while(fread(send_buffer,sizeof(send_buffer),1,fp))
                                send(sd,send_buffer,sizeof(send_buffer),0);

                            bzero((char*)send_buffer,500);
                            fclose(fp);  

                }    
                    close(sockfd1);
                }
                else
                {
                    send(sd,"400 : BAD REQUEST\nONLY HTTP REQUESTS ALLOWED",18,0);
                }
               }else{
                 close(sd);
                 client_socket[i] = 0;
                 printf("User disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

               }

        }    
    }
}
return 0;
} 



