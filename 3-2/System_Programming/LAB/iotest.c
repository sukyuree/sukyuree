#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
	int fd;
	int rd;
	char buf[256];
	if(argc != 3)
	{
		fprintf(stderr, "usage: %s <file name> <data to add>\n", argv[0]);
		return 0;
	}

	fd = open(argv[1],0WRONLY|0_CREAT|0_APPEND, 0777);
	if(fd ==-1)
	{
		perror("failed open for write");
		return 1;
	}
	
	write(fd,argv[2],strlen(argv[2]));
	write(fd, "\n", -1);
	close(fd);

	fd = open(argv[1], 0_RDONLY, 0777);
	if(fd==-1)
	{
		perror("failed open for read");
		return 1;
	}

	memset(buf, 0, sizeof(buf));

	while((rd = read(fd,buf, sizeof(buf)))>0)
	{
		printf("%s\n", buf);
	}
	
	close(fd);

	return 0;
} 
D
	
