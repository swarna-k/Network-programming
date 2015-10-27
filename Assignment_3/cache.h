#ifndef ENCODING_H
#define ENCODING_H
#include <time.h>

//Read request message


typedef struct CacheNode{
	char domainName[255];
	char page[255];
	int expire_flag;
	time_t lastAccessed;
	time_t lastModified;
	time_t expires;
}CacheNode;

void initilizeCache();

//Stores item in cache
//Call after saving temporary file tempfile.txt
void cacheItem(char* domainName, char* page);

//Checks if item is in cache and not stale
//Returns -1 if not in cache
//Returns index in Cache on success
int checkCache(char* domainName, char* page);


//Populates Buffer with filename from index in Cache
void getCachedItem(char* filename, int index);



int itemToEvict();

void evictItem(int cacheIndex, CacheNode* newNode);


#endif