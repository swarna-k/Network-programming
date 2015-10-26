#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

CacheNode Cache[10] = {[0 ...9 ]={"Empty","Empty",0,0,0,0}};
void cacheItem(char* domainName, char* page){
	FILE * tempfile; 
	char * line = NULL;
	ssize_t read;
	size_t len = 0; 
	time_t current; 
	struct tm tm;
	int evictIndex;
	struct CacheNode newnode;
	
	strcpy(newnode.domainName, domainName);
	strcpy(newnode.page, page);
	
	
	
	
	tempfile = fopen ("tempfile.txt", "r");
	if (tempfile == NULL){
		puts("No Item Cached");
		return;
	}
	
	puts("Reading file...");
	while((read = getline(&line, &len, tempfile)) != -1){
		//printf("%s",line);
		if ('\n' == line[0] || '\r'== line[0]){
			break;
		} 
		else if (strstr(line, "Expires:") != NULL){
			newnode.expire_flag = 1; 
			strptime(line+strlen("Expires: "), "%a, %d %b %Y %H:%M:%S", &tm);
			newnode.expires = mktime(&tm); 
			
		}
		
		else if (strstr(line, "Last-Modified") != NULL){
			strptime(line+strlen("Last-Modified: "), "%a, %d %b %Y %H:%M:%S", &tm);
			newnode.lastModified = mktime(&tm); 

		}
	}
	newnode.lastAccessed = time(&current);
	puts("Creating new cache entry");
	
	evictIndex = itemToEvict();
	
	evictItem(evictIndex, &newnode);
	
}


//Returns Index of least used data
int itemToEvict(){
	int i;
	int evictIndex = 0;
	for(i = 0; i<10; i++){ //sizeof(Cache) = 10 (Doesn't work for static variable)
	if(Cache[i].lastAccessed < Cache[evictIndex].lastAccessed){
		evictIndex = i; 
	}
}
puts("Evicted Index selected");
return evictIndex;
}

void evictItem(int cacheIndex, CacheNode* newNode){
char filename[255] = "cachefile*.txt";
char tempname[255] = "tempfile.txt";
filename[9] = cacheIndex + '0';
printf("Evicting filename %s\n", filename);
	
	
// Check for access
if( access( filename, F_OK ) != -1 ) {
	if(remove(filename)!=0){ //If accessed remove
		puts("Cache file failed to evict");
		return;
	}
	if(rename(tempname,filename)!=0){ //Insert new
		puts("Cache failed to insert new file");
		return;
	}
} else {
	if(rename(tempname,filename)!=0){ //If no acces, insert new
		puts("Cache failed to insert new file");
		return;
	}
}
Cache[cacheIndex] = *newNode;
puts("New Item Successfully Cached");
	
	
}

int checkCache(char* domainName, char* page){
int i;
int cacheIndex = -1;
for(i = 0; i<10; i++){ //sizeof(Cache) = 10 (Doesn't work for static variable)
printf("Argument = %s\n", domainName);
printf("Cache [i] = %s\n", Cache[i].domainName);

if(strcmp(domainName,Cache[i].domainName) == 0 && strcmp(page, Cache[i].page) == 0){
	cacheIndex = i; 
}
	
}
printf("%d\n",cacheIndex); 
return cacheIndex; 
}

void getCachedItem(char* filename, int cacheIndex){
	char fname[255] = "cachefile*.txt";
	fname[9] = cacheIndex + '0';
	strcpy(filename,fname); 
	return;
}

/*
int main(){
struct tm tm;
char domain[255] = "DomainName";
char page[255] = "PageName";
char filename[255];
cacheItem(domain,page);
int y =checkCache(domain, page);
if ( y >= 0){
getCachedItem(filename, y);
printf("The file is cached at %s\n", filename);
}
else {
puts("Item not in cache");
}



}
*/