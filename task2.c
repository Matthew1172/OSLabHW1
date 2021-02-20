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
	char* filepath2 = argv[2];
	int returnval;

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
	int fd = open(filepath, O_RDONLY);
	if(fd == -1){
		printf("\nopen() failed with error [%s]\n", strerror(errno));	
		return 1;	
	}else{
		if(DEBUG) printf("\nOpen() successful\n");

		//get size of first file using stat and store it into filesize
		struct stat stbuf;
		stat(filepath, &stbuf);
		const unsigned long filesize = stbuf.st_size;

		if(DEBUG) printf("%lu\n", filesize);

		//create buffer filesize bytes large
		char* bf = (char*) calloc(filesize, 1);
		//read first file to filesize bytes
		int sz = read(fd, bf, filesize);

		//CREATE SECOND FILE
		int fd2 = open(filepath2, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
		if (fd2 == -1) 
		{ 
			// print which type of error have in a code 
			printf("Error Number % d\n", errno);  

			// print program error details 
			perror("Program");                 
			return -1;
		}
		//WRITE CONTENTS OF FIRST FILE INTO SECOND FILE
		int sfw = write(fd2, bf, filesize);
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

	}
	return 0;
}

