#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	int fd;
	int i, buf;
	fd = open("num.dat", 0_RDWR|0_CREAT);

	if(fd<0)
	{
	perror("error : ");
	exit(0);
	}
	
	for(i=1000; i < 1010; i++)
	{
		write(fd, (void*)&i, sizeof(i));
	}
	
	lseek(fd, 0, SEEK_SET);
	read(fd, (void*)&buf, sizeof(i));
	printf("%d\n", buf);

	lseek(fd, 4*sizeof(i), SEEK_SET);
	read(fd, (void*)&buf, sizeof(i));
	printf("%d\n", buf);
}
