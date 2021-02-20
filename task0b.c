#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
int main (int argc, char* argv[])
{
	char* filepath = argv[1];
	int fd;

	//create file passed as argument and give 777 permission
	fd = open(filepath, 
			O_CREAT | O_RDWR, 
			S_IRWXU | S_IRWXG | S_IRWXO);

	if (fd == -1) 
	{ 
		// print which type of error have in a code 
		printf("Error Number % d\n", errno);  

		// print program error details 
		perror("Program");                 
		return -1;
	}



	if(close(fd) < 0)  
	{ 
		perror("c1"); 
		return -1; 
	}  
	return 0;
}
