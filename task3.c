#define DEBUG 0
#define NUMBEROFBYTES 50
#define REPLACE '5'
#define REPLACEWITH 'A'
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

void replaceChars(char*, char, char, size_t);

int main (int argc, char* argv[])
{
	/* DECLARE & INITIALIZE VARIABLES */
	//SOURCE FILEPATH
	char* filepath = argv[1];
	//DESTINATION FILEPATH
	char* filepath2 = argv[2];
	//FD -- RETURN VALUE FROM OPEN(SOURCE)
	//FD2 -- RETURN VALUE FROM OPEN(DESTINATION)
	//RETURNVAL -- RETURN VALUE TO CHECK FROM VARIOUS SYS CALLS
	//I -- COUNTER FOR LOOP
	int fd, fd2, returnval, i = 0;
	//GET FILESIZE OF SOURCE USING STAT AND STORE IT INTO FILESIZE
	struct stat stbuf;
	stat(filepath, &stbuf);
	const unsigned long filesize = stbuf.st_size;
	//CREATE BUFFER FOR READING
	char* bf = (char*) calloc(NUMBEROFBYTES, 1);
	//CREATE SPECIAL XYZ CHAR* FOR WRITING TO DESTINATION
	char* arr = (char*) calloc(3, 1);
	arr[0] = 'X';
	arr[1] = 'Y';
	arr[2] = 'Z';
	//GET LEFTOVER BYTES
	int lps = filesize % NUMBEROFBYTES;
	//CHARACTER TO REPLACE IN SOURCE
	char r1 = REPLACE;
	//CHARACTER TO REPLACE WITH IN DESTINATION
	char r2 = REPLACEWITH;
	
	if(DEBUG) printf("\ncharacter to replace: %c\n", r1);
	if(DEBUG) printf("\ncharacter to replace with: %c\n", r2);
	if(DEBUG) printf("\nsource filesize: %lu\n", filesize);
	
	//check number of args
	if(argc != 3){
		printf("\nUsage: ./a.out <file-to-copy> <destination>\n");
		return 1;
	}
	// Check file existence
	returnval = access(filepath, F_OK);
	if (returnval == 0){
		if(DEBUG) printf ("%s exists\n", filepath);
	}else{
		if (errno == ENOENT)
			printf ("%s does not exist\n", filepath);
		else if (errno == EACCES)
			printf ("%s is not accessible\n", filepath);
		return 0;
	}
	/* Check read access. */
	returnval = access (filepath, R_OK);
	if (returnval == 0){
		if(DEBUG) printf("%s is readable\n", filepath);
	}else{
		printf ("%s is not readable (access denied)\n", filepath);
	}
	/* Check write access. */
	returnval = access (filepath, W_OK);
	if (returnval == 0){
		if(DEBUG) printf("%s is writable\n", filepath);
	}else if (errno == EACCES){
		printf ("%s is not writable (access denied)\n", filepath);
	}else if (errno == EROFS){
		printf ("%s is not writable (read-only filesystem)\n", filepath);
	}
	//OPEN FIRST FILE
	fd = open(filepath, O_RDONLY);
	if(fd == -1){
		printf("\nopen() failed with error [%s]\n", strerror(errno));	
		return 1;	
	}		
	if(DEBUG) printf("\nOpen() successful\n");
	//OPEN SECOND FILE OR CREATE SECOND FILE
	fd2 = open(filepath2, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd2 == -1) 
	{ 
		// print which type of error have in a code 
		printf("\nError Number %d\n", errno);  
		// print program error details 
		perror("Program");                 
		return -1;
	}
	//START LOOP
	do{
		//read source file to 50 bytes
		returnval = read(fd, bf, NUMBEROFBYTES);
		replaceChars(bf, r1, r2, NUMBEROFBYTES);
		//WRITE TO FILE2
		returnval = write(fd2, bf, NUMBEROFBYTES);
		returnval = write(fd2, arr, 3);
		i += NUMBEROFBYTES;
	}while(i < filesize - lps);
	//LEFTOVER BYTES
	if(lps > 0){
		returnval = read(fd, bf, lps);
		replaceChars(bf, r1, r2, lps);
		returnval = write(fd2, bf, lps);
		returnval = write(fd2, arr, 3);
	}
	//close the second file
	if(close(fd2) != 0){
		strerror(errno);
		return 1;
	}
	//close the first file
	if(close(fd) != 0){
		strerror(errno);
		return 1;
	}
	return 0;
}

void replaceChars(char* bf, char a, char b, size_t s){
	int i = 0;
	do{
		if(bf[i] == a){
			bf[i] = b;
		}
		++i;
	}while(i < s);
}
