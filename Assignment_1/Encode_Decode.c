//Encode Decode and Structs
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct msg_header {
	unsigned short vrsn : 9 ; //9 bits
	unsigned short type : 7 ; //7 bits
	unsigned short length : 16 ; //16 bits
}msg_header;

typedef struct attr_header {
	unsigned short type : 16;
	unsigned short length : 16;
}attr_header;

void msg_header_hton(msg_header *msg_hdr){ 
	msg_hdr->vrsn = (unsigned short)htons(msg_hdr->vrsn);
	msg_hdr->type = (unsigned short)htons(msg_hdr->type);
	msg_hdr->length = (unsigned short)htons(msg_hdr->length);
}

void msg_header_ntoh( msg_header *msg_hdr){
	msg_hdr->vrsn = (unsigned short)ntohs(msg_hdr->vrsn);
	msg_hdr->type = (unsigned short)ntohs(msg_hdr->type);
	msg_hdr->length = (unsigned short)ntohs(msg_hdr->length);
}
void attr_header_hton(attr_header *attr_hdr){
	attr_hdr->type = (unsigned short)htons(attr_hdr->type);
	attr_hdr->length = (unsigned short)htons(attr_hdr->length);
}
void attr_header_ntoh(attr_header *attr_hdr){
	attr_hdr->type = (unsigned short)ntohs(attr_hdr->type);
	attr_hdr->length = (unsigned short)ntohs(attr_hdr->length);
}


char* encode_msg(struct msg_header *msg_hdr, struct attr_header *attr_hdr, char payload[]){
	
	
	size_t msg_length = sizeof(msg_header) + sizeof(attr_header) + (size_t)(attr_hdr->length);
	msg_hdr->length = msg_length;
	
	printf("Mssage length: %d \n" , msg_length);
	
	char* msg = (char*)malloc(msg_length);
	
	printf("Pointer addy 1: %u \n" , msg);
	char* tmp = msg;
	
	
	//msg_header_hton(msg_hdr);
	memcpy(msg, msg_hdr, sizeof(msg_header));
	tmp = tmp + sizeof(msg_header);
	//msg_header_ntoh(msg_hdr);
	
	//attr_header_hton(attr_hdr);
	memcpy(tmp,attr_hdr, sizeof(attr_header));
	tmp = tmp + sizeof(attr_header);
	//attr_header_ntoh(attr_hdr);
	
	memcpy(tmp, payload, attr_hdr->length);
	int i;
	for(i = 0; i <16; i++){
	printf("Pointer addy 1 value: %u \n" , *(msg+i));
	}
	
	return msg;

	
}

void decode_msg(char *msg, msg_header *msg_hdr, attr_header *attr_hdr, char payload[]){
	char* tmp = msg;
	
	
	

	memcpy(msg_hdr,tmp,sizeof(msg_header));
	tmp = tmp + sizeof(msg_header);
	memcpy(attr_hdr,tmp,sizeof(attr_header));
	tmp = tmp +sizeof(*attr_hdr);
	

	
	//msg_header_ntoh(msg_hdr);
	//attr_header_ntoh(attr_hdr);
	
	memcpy(payload,tmp,attr_hdr->length);
	
	
}

int main (){
	printf("Size of msg header is : %lu \n", sizeof(msg_header));
	printf("Size of attr header is : %lu \n", sizeof(attr_header));
	
	char buffer[250] = "This is a sample payload"; 
	msg_header msg_hdr = {.vrsn = 3, .type = 5, .length = 4};
	attr_header attr_hdr = {.type = 6, .length = 25};//length should be +1 for null terminating char
	
	char* msg = encode_msg(&msg_hdr,&attr_hdr,&buffer);
	
	
	msg_header msg_hdr2;
	attr_header attr_hdr2;
	char buffer2[250];
	
	decode_msg(msg,&msg_hdr2,&attr_hdr2,&buffer2);
	
	
	
	printf("Msg length %hu \n", msg_hdr2.length);
	printf("Attr length %hu \n", attr_hdr2.length);
	printf("Buffer: %s \n", buffer2);
	free(msg);
	
	
}