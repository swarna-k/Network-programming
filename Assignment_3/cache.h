#ifndef ENCODING_H
#define ENCODING_H
#include <time.h>

//Read request message


typedef CacheNode{
	char domainName[255];
	char page[255];
	int noExpiry;
	time_t lastAccessed;
	time_t lastModified;
	time_t expires;
}CacheNode;

static CacheNode Cache[10];

//Stores item in cache
void cacheItem(char* Item);

//Checks if item is in cache and not stale
//Returns -1 if not in cache
//Returns index in Cache on success
int checkCache(char* domainName, char* page);


//Populates Buffer with filename from index in Cache
void getCachedItem(char* filename, int index);





#endif