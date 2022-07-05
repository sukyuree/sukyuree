#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 17
#define VALUE_MAX 40

/*int main(int argc, char *argv[]){
	int repeat = 10;

	//Enable GPIO pins
	if(-1==GPIOExport(POUT))
		return(1);

	//Set GPIO directions
	if(-1==GPIODirection(POUT,OUT))
		return(2);

	do{
		if(-1==GPIOWrite(POUT,repeat %2))
			return(3);
		usleep(500*1000);
	}
	while(repeat--);

	//Disable GPIO pins
	if(-1==GPIOUnexport(POUT))
		return(4);
	
	return(0);
}
*/
static int GPIOExport(int pin){
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir){
	static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35
	//char path[DIRECTION_MAX]="/sys/class/gpio/gpio24/direction";
	char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction",pin);

	fd = open(path, O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if(-1==write(fd, &s_directions_str[IN==dir ? 0 : 3], IN == dir ? 2 : 3)){
		fprintf(stderr, "Failed to set directions!\n");
		close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIOWrite(int pin, int value){
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW==value ? 0 : 1], 1)){
		fprintf(stderr, "Failed to write value!\n");
		close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIOUnexport(int pin){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

int main(int argc, char *argv[]){
	int repeat = 10;

	if(-1==GPIOExport(POUT))
		return(1);

	if(-1==GPIODirection(POUT,OUT))
		return(2);

	do{
		if(-1==GPIOWrite(POUT,repeat %2))
			return(3);
		usleep(500*1000);
	}

	while(repeat--);

	if(-1==GPIOUnexport(POUT))
		return(4);

	return(0);
}

