#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

FILE *f1;
int block_number=0;
int endfile=0;

char* get_file_data()
{
	long offset=(block_number*512);
	int block_send=block_number+1;
	int opcode=1,num_bytes, worked=1;
	char file_data_read[512];


	//worked = lseek(f1,512,SEEK_SET);
	//worked=fseek(f1, offset, SEEK_SET);

	//if(worked!=0)
	//{
	//	perror("Error with fseek operation!");
	//	exit(0);
	//}
	//else
	//{
		num_bytes=fread(file_data_read,1,512,f1);
		if(num_bytes<0)
		{
			perror("Error with fread!");
			exit(0);
		}
		else if(num_bytes<512)
		{
			printf("Last packet sent!");
			//ENCODE Complete message
			endfile=1;
			block_number=0;
		}
		else
		{
			printf("Sending %d packet\n", &block_send);
			//ENCODE Complete Message
			block_number=block_number+1;
		}
	//}

	return file_data_read;

}

int main()
{
	char file_name[]="datafile.txt";
	char *op;
	f1=fopen(file_name,"r");
	while(endfile!=1)
	{
		op=get_file_data();
		printf("%s\n", op);
	}

	endfile=0;
	fclose(f1);
	return 0;
}