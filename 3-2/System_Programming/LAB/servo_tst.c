#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define PIN 26
#define POUT 17
#define POUT2 21
#define VALUE_MAX 40

int main(int argc, char *argv[])
{
	int repeat = 100;
	int state = 1;
	int prev_state = 1;
	int light = 0;
	
	if(-1 == GPIOExport(POUT2) || -1 == GPIOExport(PIN))
		return(1);
		
	if(-1 == GPIODirection(POUT2, OUT) || -1 == GPIODirection(PIN, IN))
		return(2);
		
	do{
		if(-1 == GPIOWrite(POUT2, 1))
			return(3);
		state = GPIORead(PIN);
		
		printf("GPIORead : %d from pin %d\n", GPIORead(PIN), PIN);
		
		if(prev_state == 0 && state == 1)
			light++;
			
		printf("%d\n", state);
			
		/*if(light%2 == 1)
		{
			if(-1 == GPIOWrite(POUT, 1))
				return(3);
		}
		else
		{
			if(-1 == GPIOWrite(POUT, 0))
				return(3);
		}*/
			
		prev_state = GPIORead(PIN);
		usleep(100000);
	}
	while(repeat--);
	
	if(-1 == GPIOUnexport(POUT2) || -1 == GPIOUnexport(PIN))
		return(4);
		
	return 0;
}

int GPIOExport(int pin)
{
	#define BUFFER_MAX 35
	
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
	
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(-1 == fd)
	{
		fprintf(stderr, "Failed to open export fo writing!\n");
		return(-1);
	}
	
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

int GPIODirection(int pin, int dir)
{
	static const char s_directions_str[] = "in\0out";
	
#define DIRECTION_MAX 50

	char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
	int fd;
	
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	
	fd = open(path, O_WRONLY);
	if(-1 == fd)
	{
		fprintf(stderr, "Failed to open gpio direction fo writing!\n");
		return(-1);
	}
	
	if(-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3))
	{
		fprintf(stderr, "Failted to set direction!\n");
		close(fd);
		return(-1);
	}
	
	close(fd);
	return(0);
}

int GPIORead(int pin)
{
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
	
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if(-1 == fd) {
			fprintf(stderr, "Failed to open gpio value for reading!\n");
			return(-1);
	}
	
	if(-1 == read(fd, value_str, 3))
	{
		fprintf(stderr, "Failed to read value!\n");
		close(fd);
		return(-1);
	}
	
	close(fd);
	
	return(atoi(value_str));
	
}

int GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
	
	char path[VALUE_MAX];
	int fd;
	
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if(-1 == fd)
	{
		fprintf(stderr, "Faited to open gpio value for writing!\n");
		return(-1);
	}
	
	if(1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1))
	{
		fprintf(stderr, "Failed to write value!\n");
		close(fd);
		return(-1);
	}
	
	close(fd);
	return(0);
}

int GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
	
	fd = open("/sys/class/gpio/gpio%d/unexport", O_WRONLY);
	if(-1 == fd)
	{
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
	
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}




