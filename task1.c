#define DEBUG 0
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
int main (int argc, char* argv[])
{
	char* filepath = argv[1];
	int returnval;

	if(argc != 2){
		printf("\nUsage: ./a.out <name-of-file>\n");
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
	int fd = open(filepath, O_RDONLY);
	if(fd == -1){
		printf("\nopen() failed with error [%s]\n", strerror(errno));	
		return 1;	
	}else{
		if(DEBUG) printf("\nOpen() successful\n");

		//get file length using stat
		struct stat stbuf;
		stat(filepath, &stbuf);
		const unsigned long filesize = stbuf.st_size;

		if(DEBUG) printf("%lu\n", filesize);

		//create buffer filesize bytes large
		char* bf = (char*) calloc(filesize, 1);
		//read file to filesize bytes
		int sz = read(fd, bf, filesize);
		if(DEBUG) printf("called read(%d, c, %lu). returned that %d bytes  were read.\n", fd, filesize, sz); 
		//add zero sentinel because printf()
		//uses a 0 or null terminating c string
		bf[sz] = '\0'; 
		//print the contents
		printf("\n%s\n", bf); 

		//close the file
		if(close(fd) != 0){
			strerror(errno);
			return 1;
		}

	}
	return 0;
}

