#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sockfd,portnum,j,i,n,readbytes=1;
	struct sockaddr_in serv_addr;
	struct hostent *servname;
	//char *dom_name;
	//char *res_name;
	char recv_buff[250];
	char url_buff[256];
	char message_buff[280];
	FILE *f1;

	if(argc<4)
	{
		fprintf(stderr,"usage : client hostname portno url\n" );
		exit(0);
	}

	portnum=atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM,0);

	if(sockfd==-1)
	{
		perror("error opening socket!");
		exit(0);
	} 

	servname=gethostbyname(argv[1]);

	if(servname==NULL)
	{
		fprintf(stderr,"cannot find host");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char *)servname->h_addr,(char *)&serv_addr.sin_addr.s_addr,servname->h_length);

	serv_addr.sin_port = htons(portnum);

	if(connect(sockfd,(struct sockaddr_in*)&serv_addr,sizeof(serv_addr))<0)
	{
		perror("Error during connection.");
		exit(0);
	}

	bzero(url_buff,256);
	strcpy(url_buff,argv[3]);

	
	sprintf(message_buff, "GET HTTP/1.0 %s",url_buff);
	printf("Sending URL %s to the server.\n",url_buff);
	n = write(sockfd,message_buff,280);

	if(n==-1)
	{
		perror("Error writing request to server\n");
		exit(0);
	}

	f1=fopen("recvfile.txt","w");
	readbytes = recv(sockfd, recv_buff, 250, 0);
	fprintf(f1,"%s",recv_buff);
	printf("Pritning data to revcfile.txt\n");

	while(readbytes>=250)
	{
		bzero(recv_buff,250);
		readbytes = recv(sockfd, recv_buff, 250, 0);
		fprintf(f1,"%s",recv_buff);
	}

	printf("Received all the data. Closing connection.\n");

	fclose(f1);
}