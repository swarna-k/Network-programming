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
//#include "encoding.h"
 
#define TRUE   1
#define FALSE  0
void error(char *msg)
{
perror(msg);
exit(0);
}
int check_client(int client_port[10],int cli_temp);

int main(int argc, char *argv[])
{
int sock, length, fromlen, nready, maxfdp1;
struct sockaddr_in server;
struct sockaddr_in cli_addr[10],cli_temp;
char buf[1024];
int clients = 0;
int client_port[10];
socklen_t slen[10], slen_temp;
int i;
int client_num;
fd_set  rset;
ssize_t n;
socklen_t len;

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
              if (clients <= 10) {
                client_num = check_client(&client_port[0],(int)ntohs(cli_temp.sin_port));
                if(client_num==-1){
                  //printf("%d",clients);
                  cli_addr[clients] = cli_temp;

                  client_port[clients] = (int)ntohs(cli_temp.sin_port);
                  clients=clients+1;
                 
                  puts("Client added\n");
                  //printf("%hu\n",client_port[clients-1] );
                  i=clients-1;
                  sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr[i], sizeof(cli_addr[i]));
                }else{
                  puts("already a client\n");
                  i=client_num;
                  sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr[i], sizeof(cli_addr[i]));
                }
              }

}
}
}

int check_client(int client_port[10],int cli_temp){
  int i;
  puts("in function");
  
  for(i=0;i<10;i++){
     // printf("%d\n",client_port[i] );
      //printf("%d\n",cli_temp );
      if(client_port[i]==cli_temp)
        return i;
  }

  return -1;
}