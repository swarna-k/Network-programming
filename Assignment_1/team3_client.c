
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "encoding.h"
int main(int argc, char *argv[])
{
	int sockfd,portnum,n,i,flag=0;
	struct sockaddr_in serv_addr;
	struct hostent *servname;
	struct msg_header mess_hdr;
	struct attr_header atri_hdr;
	struct attr_header atri_hdr2;
	char *buff;
	char recv_buff[1024];
	fd_set readfiledesc;
	char username_buff[16];
	char message_buff[512];
	FD_ZERO (&readfiledesc);

	if(argc<4)
	{
		fprintf(stderr,"usage : client username hostname portno\n" );
		exit(0);
	}

	portnum=atoi(argv[3]);

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd==-1)
	{
		perror("error opening socket!");
		exit(0);
	} 

	servname=gethostbyname(argv[2]);

	if(servname==NULL)
	{
		fprintf(stderr,"cannot find host");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char *)servname->h_addr,(char *)&serv_addr.sin_addr.s_addr,servname->h_length);

	serv_addr.sin_port = htons(portnum);

	if(connect(sockfd,&serv_addr,sizeof(serv_addr))<0)
	{
		perror("Error during connection.");
		exit(0);
	}

	bzero(username_buff,16);
	strcpy(username_buff,argv[1]);
	mess_hdr.vrsn=3;
	mess_hdr.type=2;
	atri_hdr.type=2;
	atri_hdr.length=(strlen(username_buff)+1);
	
	buff = encode_msg(&mess_hdr, &atri_hdr, NULL, username_buff, NULL);
	n = write(sockfd,buff,mess_hdr.length);
	free(buff);
	//SEND USERNAME AS JOIN FUNCTION
	//n = write(sockfd,username_buff,strlen(username_buff));
	if(n==-1)
	{
		perror("Error writing username to server\n");
		exit(0);
	}

	FD_SET(0, &readfiledesc);
	FD_SET(sockfd, &readfiledesc);

	while(1)
	{
		
		if (select(sockfd+1, &readfiledesc, NULL, NULL, NULL) == -1)
		{
			perror("Error in select function.");
			exit(0);
		}
		for(i = 0; i <= sockfd; i++) 
		{
				
			if (FD_ISSET(i, &readfiledesc)) 
			{
				if (i == 0) 
				{
					//THIS IS FOR READING FROM KEYBOARD
					fgets(message_buff, sizeof(message_buff), stdin);
					mess_hdr.vrsn=3;
					mess_hdr.type=4;
					atri_hdr.type=4;
					atri_hdr.length=(strlen(message_buff)+1);
					buff = encode_msg(&mess_hdr, &atri_hdr, NULL, message_buff, NULL);
					if (buff != NULL){
						n = write(sockfd,buff,mess_hdr.length);
						free(buff);
						message_buff[0]='\0';
					}
					else {
						n=-1;
					}
	                //SEND MESSAGE TO SERVER
	                //n = write(sockfd,message_buff,strlen(message_buff));
	                
					if (n == -1) 
					{
						perror("Error sending the message");
						exit(0);
					}
					flag=1;
				}
				
				if ((i == sockfd)&&(flag==0))
				{
					
					//THIS IS FOR READING FROM THE SERVER
					n = recv(sockfd, recv_buff, 4, 0);
				
					if (n == -1) 
					{
						perror("Error during receive");
						exit(0);
					}
					
					decode_msg_header(recv_buff,&mess_hdr);
					
					bzero(recv_buff, 1024);
					
					n = recv(sockfd, recv_buff,mess_hdr.length-4,0);
					
					int type = (int) mess_hdr.type;
					decode_msg(recv_buff, &type, &atri_hdr, &atri_hdr2, username_buff, message_buff);
				
					if(mess_hdr.type==3)
					{
						printf("%s: ",username_buff);
						printf("%s\n",message_buff);
					}
					
					if(mess_hdr.type==8)
					{
						printf("%s is ONLINE!\n",username_buff);
					}
					
					if(mess_hdr.type==6)
					{
						printf("%s is OFFLINE\n",username_buff);
					}
					//buff[n] = '\0';
					//printf("%s\n",buff);


				}
			
				FD_SET(0, &readfiledesc);
		        FD_SET(sockfd, &readfiledesc);	
			}
			
			
		}
		flag=0;
	}
	close(sockfd);

    return 0;
}