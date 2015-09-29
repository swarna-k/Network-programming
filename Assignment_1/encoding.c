
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "encoding.h"
#include <strings.h>

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


char* encode_msg(msg_header *msg_hdr, attr_header *attr_hdr, attr_header *attr_hdr2, char payload[], char payload2[]){
	size_t msg_length;
	
	if(attr_hdr2 == NULL){
		msg_length = sizeof(msg_header) + sizeof(attr_header) + (size_t)(attr_hdr->length);
	}
	else{
		msg_length = sizeof(msg_header) + 2*sizeof(attr_header) + (size_t)(attr_hdr->length)+ (size_t)(attr_hdr2->length);	
	}
	
	if(msg_length >1024) return NULL;

	msg_hdr->length = msg_length;
	printf("Mssage length: %d \n" , msg_length);
	
	char* msg = (char*)malloc(msg_length);
	
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
	
	if(attr_hdr2 != NULL){
		tmp = tmp + attr_hdr->length;
		//attr_header_hton(attr_hdr2);
		memcpy(tmp,attr_hdr2, sizeof(attr_header));
		tmp = tmp + sizeof(attr_header);
		//attr_header_ntoh(attr_hdr2);
		memcpy(tmp, payload2, attr_hdr2->length);
	}
	
	return msg;

	
}

void decode_msg(char *msg, msg_header *msg_hdr, attr_header *attr_hdr, attr_header *attr_hdr2, char payload[], char payload2[]){
	char* tmp = msg;

	memcpy(msg_hdr,tmp,sizeof(msg_header));
	tmp = tmp + sizeof(msg_header);
	memcpy(attr_hdr,tmp,sizeof(attr_header));
	tmp = tmp +sizeof(*attr_hdr);
	
	//msg_header_ntoh(msg_hdr);
	//attr_header_ntoh(attr_hdr);
	
	memcpy(payload,tmp,attr_hdr->length);
	
	
	
	
	if (msg_hdr->type == 7 || msg_hdr->type == 3){
		tmp = tmp + attr_hdr->length;
		memcpy(attr_hdr2,tmp,sizeof(attr_header));
		tmp = tmp +sizeof(attr_header);
		//attr_header_ntoh(attr_hdr2);
		memcpy(payload2,tmp,attr_hdr2->length);
	}
	
	
}
/*
int main (){
	printf("Size of msg header is : %lu \n", sizeof(msg_header));
	printf("Size of attr header is : %lu \n", sizeof(attr_header));
	
	
	
	// TEST FOR 1 ATTRIBUTE ***********************************************
	char buffer[250] = "This is a sample payload"; 
	msg_header msg_hdr = {.vrsn = 3, .type = 1, .length = 4};
	attr_header attr_hdr = {.type = 6, .length = 25};//length should be +1 for null terminating char
	
	char* msg = encode_msg(&msg_hdr,&attr_hdr,NULL,&buffer,NULL);
	
	
	msg_header msg_hdr2;
	attr_header attr_hdr2;
	char buffer2[250];
	
	decode_msg(msg,&msg_hdr2,&attr_hdr2, NULL, &buffer2, NULL);
	
	
	printf("Msg length %hu \n", msg_hdr2.length);
	printf("Msg type %hu \n", msg_hdr2.type);
	printf("Attr length %hu \n", attr_hdr2.length);
	printf("Buffer: %s \n", buffer2);
	free(msg);
	
	// TEST FOR 2 ATTRIBUTE *****************************************
	
	char buffer3[250] = "This is a sample payload"; 
	char buffer4[250] = "This is a sample payload2";
	msg_header msg_hdr3 = {.vrsn = 3, .type = 3, .length = 4}; //Type is 3 or 7
	attr_header attr_hdr3 = {.type = 6, .length = 25};
	attr_header attr_hdr4 = {.type = 6, .length = 26};
	
	
	msg = encode_msg(&msg_hdr3, &attr_hdr3, &attr_hdr4, &buffer3, &buffer4);
	
	bzero(buffer3, 250);
	bzero(buffer4, 250);
	msg_header msg_hdr4;
	attr_header attr_hdr5;
	attr_header attr_hdr6;
	
	decode_msg(msg,&msg_hdr4,&attr_hdr5,&attr_hdr6,&buffer3,&buffer4);
	
	
	printf("Msg length %hu \n", msg_hdr4.length);
	printf("Msg type %hu \n", msg_hdr4.type);
	printf("Attr5 length %hu \n", attr_hdr5.length);
	printf("Attr6 length %hu \n", attr_hdr6.length);
	printf("Buffer3: %s \n", buffer3);
	printf("Buffer4: %s \n", buffer4);
	
}
*/
