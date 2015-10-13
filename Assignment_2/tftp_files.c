#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include "encoding.h"


//int block_number=0;

void get_file_data(char* filename, Data_msg* data)//previous data struct
{
	FILE* f1 = fopen(filename,"r");
	long offset = (data->block_number*512);
	int block_send=data->block_number+1;
	int opcode=1,num_bytes, worked=1;
	char file_data_read[512];

	worked = fseek(f1, offset, SEEK_SET);
		
		if(worked!=0)
		{
			perror("Error with fseek");
		}
		else 
		{
			num_bytes=fread(file_data_read,1,512,f1);
			printf("Number of bytes = %d\n",num_bytes);
			if(num_bytes==0)
			{
				perror("Error with fread!");
				data->block_size = -1;
				
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

int main()
{

	Data_msg prev_data={.opcode = 1, .block_number = 9, .block_size = 512};
	char filename[255] = "datafile.txt";
	
	get_file_data(filename, &prev_data);
	
	printf("%s\n", prev_data.data);
	
	/*
	while(prev_data.block_size >= 512)
	{
		get_file_data(&prev_data);
		
		printf("%s\n", prev_data.data);
	}
*/

	return 0;
}