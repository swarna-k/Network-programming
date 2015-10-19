#ifndef ENCODING_H
#define ENCODING_H
#include <string.h>

//Read request message
typedef struct RRQ_msg {
	unsigned short opcode;
	char filename[255];
	char mode[255];
}RRQ_msg;


//Data msg to send back
typedef struct Data_msg {
	unsigned short opcode;
	unsigned short block_number;
	int block_size;
	char data[512];
}Data_msg;

//Recieved ACK
typedef struct ACK_msg {
	unsigned short opcode;
	unsigned short block_number;
}ACK_msg;

//Error Msg to send
typedef struct Error_msg {
	unsigned short opcode;
	unsigned short error_number;
	char error_data[255];
}Error_msg;

//For Code readability 
static const int nullchar = 1;
static const int zerobyte = 1;



size_t get_error_size(Error_msg *error);
size_t get_data_size(Data_msg *data);
char* decode_string(char* msg);
unsigned short decode_opcode(char* msg);
RRQ_msg decode_RRQ(char* msg);
ACK_msg decode_ACK(char * msg);
void encode_Data(Data_msg *data, char* msg);
void encode_Error(Error_msg *error, char* msg);



#endif