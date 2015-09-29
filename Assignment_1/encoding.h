#ifndef ENCODING_H
#define ENCODING_H

typedef struct msg_header {
	unsigned short vrsn : 9 ; //9 bits
	unsigned short type : 7 ; //7 bits
	unsigned short length : 16 ; //16 bits
}msg_header;

typedef struct attr_header {
	unsigned short type : 16;
	unsigned short length : 16; //length for attribute has to accomodate null terminating character. Set length as length +1
}attr_header;

void msg_header_hton(msg_header *msg_hdr);
void msg_header_ntoh( msg_header *msg_hdr);
void attr_header_hton(attr_header *attr_hdr);
void attr_header_ntoh(attr_header *attr_hdr);
char* encode_msg(msg_header *msg_hdr, attr_header *attr_hdr, attr_header *attr_hdr2, char payload[], char payload2[]); // 2 can be NULL
void decode_msg_header(char *msg, msg_header *msg_hdr);
void decode_msg(char *msg, int *msg_type, attr_header *attr_hdr, attr_header *attr_hdr2, char payload[], char payload2[]); //msg minus header;


#endif