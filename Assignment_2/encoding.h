#ifndef ENCODING_H
#define ENCODING_H
#include <string.h>

typedef struct RRQ_msg {
	unsigned short opcode;
	char filename[255];
	char mode[255];
}RRQ_msg;

typedef struct Data_msg {
	unsigned short opcode;
	unsigned short block_number;
	int block_size;
	char* data;
}Data_msg;

typedef struct ACK_msg {
	unsigned short opcode;
	unsigned short block_number;
}ACK_msg;

typedef struct Error_msg {
	unsigned short opcode;
	unsigned short error_number;
	char error_data[255];
}Error_msg;

int nullchar;
int zerobyte;

nullchar = 1;
zerobyte =1;

size_t get_error_size(Error_msg *error);

char* decode_string(char* msg);
unsigned short decode_opcode(char* msg);
RRQ_msg decode_RRQ(char* msg);
ACK_msg decode_ACK(char * msg);
void encode_Data(Data_msg *data, char* msg);
void encode_Error(Error_msg *error, char* msg);



#endif