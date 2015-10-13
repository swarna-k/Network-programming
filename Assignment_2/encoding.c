
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include "encoding.h"
#include <strings.h>

unsigned short decode_opcode(char* msg){
	unsigned short opcode;
	memcpy(&opcode, msg, 2);
	return opcode;
}

RRQ_msg decode_RRQ(char* msg){
	RRQ_msg RRQ;
	unsigned short opcode;
	char filename[255];
	char mode[255];
	char tmp;
	tmp = msg; 
	
	opcode = 1;

	strncpy(filename,tmp,255);
	filename[255] = '\0';
	tmp = tmp + strlen(filename) + nullchar + zerobyte; //1 for null char 1 for 0
	strncpy(mode,tmp, 255);
	mode[255] = '\0';
	
	
	RRQ.opcode = opcode;
	strncpy(RRQ.filename,filename,255);
	strncpy(RRQ.mode, mode, 255);
	
	return RRQ;
	
	
}

ACK_msg decode_ACK(char * msg){
	ACK_msg ACK;
	unsigned short opcode;
	unsigned short blocknumber;
	opcode = 4;
	memcpy(blocknumber,msg,sizeof(blocknumber));
	ACK.opcode = opcode;
	ACK.block_number = blocknumber;
	return ACK;
	
}

void encode_Data(Data_msg *data, char* msg){
	char* itr;
	itr = msg; 
	memcpy(itr,&data->opcode,sizeof(data->opcode));
	itr = itr + sizeof(data->opcode);
	memcpy(itr,&data->block_number,sizeof(data->block_number));
	itr = itr + sizeof(data->block_number);
	memcpy(itr,&data->data,data->block_size);
	
	
}
size_t get_error_size(Error_msg *error){
	size_t total_size;
	total_size =sizeof(error->opcode)+sizeof(error->error_number)+strlen(error->error_data)+nullchar+zerobyte;
	return total_size;
}
size_t get_data_size(Data_msg *data){
	size_t size;
	size = data->block_size + sizeof(data->opcode)+sizeof(data->block_number);
	return size;
}

void encode_Error(Error_msg *error, char* msg){
	
	unsigned char zero = 0; 
	char* itr;
	itr = msg; 
	
	puts("Got to 1\n");
	memcpy(itr,&(error->opcode),sizeof(error->opcode));
	
	itr = itr + sizeof(error->opcode);
	puts("Got to 2\n");
	memcpy(itr,&(error->error_number),sizeof(error->error_number));
	itr = itr + sizeof(error->error_number);
	memcpy(itr, &error->error_data, strlen(error->error_data)+nullchar);
	itr = itr + strlen(error->error_data)+nullchar;
	*itr = zero;
	
	
}
/*

int main (){
	unsigned short opcode;
	unsigned short testcode; 
	int i; 
	Error_msg error = {.opcode = 5, .error_number = 7, .error_data = "This is an error"};
	
	
	testcode = 3;
	opcode = decode_opcode((char*)&testcode);
	printf("Opcode = %d\n",opcode );
	
	i = get_error_size(&error);
	char msg[i];
	
	printf("msg size =%d\n",sizeof(msg));
	encode_Error(&error,msg);
	
	puts("encoded");
	
	printf("msg size =%d\n",sizeof(msg));
	
	for(i = 0; i < sizeof(msg); i++){
			printf("msg [%d] = %c\n",i,msg[i]);
		}
		
		
	
	
}
*/


