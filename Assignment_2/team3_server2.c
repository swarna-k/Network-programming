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
#include <sys/time.h> 
#include <sys/select.h>//use select() for multiplexing
#include <sys/fcntl.h> // for non-blocking//FD_SET, FD_ISSET, FD_ZERO macros
#include "encoding.h"
 
#define TRUE   1
#define FALSE  0
void error(char *msg)
{
	perror(msg);
	exit(0);
}
int check_client(int client_port[10],int cli_temp);
void get_file_data(char* filename, Data_msg* data);

int main(int argc, char *argv[])
{
	int sock, length, fromlen, nready, maxfdp1;
	struct sockaddr_in server;
	struct sockaddr_in cli_addr[10],cli_temp;
	char buf[1024];
	int clients = 0;
	int client_port[10];
	char filenames[10][255];
	socklen_t slen[10], slen_temp;
	int i;
	int client_num;
	fd_set  rset;
	ssize_t n;
	socklen_t len;
	unsigned short opcode;


	slen_temp = sizeof(cli_temp);


	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(0);
	}

	sock=socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)
	{
		error("Opening socket");
	}

	for(i=0;i<10;i++){
		client_port[i]=0;
	}

	length = sizeof(server);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argv[1]));

	if (bind(sock,(struct sockaddr *)&server,length)<0)
	{
		error("binding");
	}


	//fromlen = sizeof(struct sockaddr_in);

	int num_clients = 0;

	fcntl(sock, F_SETFL, O_NONBLOCK);
	maxfdp1 = sock + 1;
	while (1)
	{
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
			if  (errno!=EINTR)
			{
				printf("select error");
			}
		}

		if (FD_ISSET(sock, &rset)) {
			len = sizeof(cli_temp);
			n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &cli_temp, &len);
			puts("sending data");
			printf("clients = %d\n", clients);
			
			client_num = check_client(&client_port[0],(int)ntohs(cli_temp.sin_port));
			printf("Client_num = %d\n", client_num);
			int moveon = 0;
				
			if (client_num ==-1) {
				
				while(client_port[clients] != 0){
					moveon++;
					clients=(clients+1)%10;
				}
				
				if(moveon == 10) continue;  //If there are already 10 clients, ignore

				cli_addr[clients] = cli_temp;
				client_port[clients] = (int)ntohs(cli_temp.sin_port);
				i=clients;
				clients=(clients+1)%10; //max 10 clients
				opcode = decode_opcode(buf);	
				opcode = ntohs(opcode);
				
				if(opcode == 1){ //If Read Request
					puts("Making RRQ");
					RRQ_msg	rrq = decode_RRQ(buf);
					puts("RRQ made");
					strcpy(filenames[i],rrq.filename);
					//filenames[clients] = rrq.filename;
					puts("Made Data Msg");
					Data_msg data = {.opcode = 3, .block_number = 0, .block_size = 512};
					get_file_data(filenames[i],&data);
					char msg[get_data_size(&data)]; //Exact sized buffer
					encode_Data(&data,msg);				
					sendto(sock,msg,sizeof(msg), 0, (struct sockaddr*)&cli_addr[i], sizeof(cli_addr[i])); //Sends first block of data back
					puts("Message Sent");
						
				}
				else{
					puts("SEND ERROR MSG");
				}
				puts("Client added\n");
					
				//sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr[i], sizeof(cli_addr[i]));
			}else{
				i=client_num;
				opcode = decode_opcode(buf);
				opcode = ntohs(opcode);
				if(opcode == 4){ //If Ack
					ACK_msg ack = decode_ACK(buf);
					Data_msg data = {.opcode = 3, .block_number = ack.block_number, .block_size = 512}; //current block number
					get_file_data(filenames[i],&data);
					if(data.block_size == -1){ //If fread fails zero out arrays
						bzero(filenames[i],255);
						memset(&cli_addr[i],0,sizeof(cli_addr[i]));
						client_port[i] = 0;
					}
					else
					{
						char msg[get_data_size(&data)]; //if fread success
						encode_Data(&data,msg);
						sendto(sock,msg,sizeof(msg), 0, (struct sockaddr*)&cli_addr[i], sizeof(cli_addr[i])); //send next block
					}
				}
			}
		}

	}
}


int check_client(int client_port[10],int cli_temp){
	int i;
	//puts("ClientCheck");
  
	for(i=0;i<10;i++){
		// printf("%d\n",client_port[i] );
		//printf("%d\n",cli_temp );
		if(client_port[i]==cli_temp)
			return i;
	}

	return -1;
}
void get_file_data(char* filename, Data_msg* data)
{
	
	FILE* f1 = fopen(filename,"r");
	long offset = (data->block_number*512);
	int block_send=data->block_number+1; //next block number
	int num_bytes, worked=1; // number of bytes, and fseek success flag
	char file_data_read[512];
	

	worked = fseek(f1, offset, SEEK_SET);
		
	
	if(worked!=0)
	{
		perror("Error with fseek");
	}
	else
	{
		num_bytes=fread(file_data_read,1,512,f1);
		if(num_bytes==0)
		{
			perror("Error with fread!");
			data->block_size = -1; //set blocksize to -1 if fread fails ( seek past end of file);
				
		}
		else if(num_bytes<512)
		{
			printf("Last packet sent!");
			
			data->block_number = block_send;
			data->block_size = num_bytes;
			memcpy(&data->data,file_data_read,num_bytes);
			

		}
		else
		{
			printf("Sending %d packet\n", block_send);
			
			data->block_number = block_send;
			data->block_size = num_bytes;
			memcpy(&data->data,file_data_read,num_bytes);
			
		}
	}
		
	fclose(f1);
		
}
