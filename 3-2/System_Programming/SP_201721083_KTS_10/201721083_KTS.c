#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define PIN 20
#define POUT2 20
#define POUT 17
#define VALUE_MAX 40

static int LED_GPIOExport(int pin){
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

static int LED_GPIODirection(int pin, int dir){
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

static int LED_GPIOWrite(int pin, int value){
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

static int LED_GPIOUnexport(int pin){
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

static int BUTTON_GPIOExport(int pin){
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export",O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d",pin);
	write(fd,buffer,bytes_written);
	close(fd);
	return (0);
}

static int BUTTON_GPIOUnexport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
	fd = open("/sys/class/gpio/unexport",O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written - snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int BUTTON_GPIODirection(int pin, int dir) {
	static const char s_directions_str[] = "in\0out";

#define DIRECTION_MAX 35

	char path[DIRECTION_MAX] = "/sys/class/gpio/gpio%d/direction";
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction",pin);

	fd - open(path,O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if(-1==write(fd,&s_directions_str[IN==dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set directions!\n");
		close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

static int BUTTON_GPIORead(int pin){
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value",pin);
	fd = open(path, O_RDONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if(-1==read(fd,value_str,3)){
		fprintf(stderr, "Failed to read value!\n");
		close(fd);
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int BUTTON_GPIOWrite(int pin, int value) {
	static const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value",pin);
	fd = open(path, O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if(1 != write(fd, &s_values_str[LOW == value ? 0 : 1],1)){
		fprintf(stderr, "Failed to write value!\n");
		close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

int main(int argc, char *argv[]){
	int pressed = 0;
	int state = 0;
	if(-1==LED_GPIOExport(POUT))
		return(1);
	if(-1==LED_GPIODirection(POUT,OUT))
		return(2);
	if(-1==BUTTON_GPIOExport(POUT2)||-1==BUTTON_GPIOExport(PIN))
		return(1);
	if(-1==BUTTON_GPIODirection(POUT2,OUT)||-1==BUTTON_GPIODirection(PIN,IN))
		return(2);
	while(1){
		if(BUTTON_GPIORead(PIN) == 1 && state == 1 && pressed == 1){
			pressed = 0;
			state = 0;
			LED_GPIOWrite(POUT,0);
			printf("POUT : %d	|	LED OFF\n",POUT);
		}
		if(BUTTON_GPIORead(PIN) == 1 && state == 0 && pressed == 1){
			pressed = 0;
			state = 1;
			LED_GPIOWrite(POUT,1);
			printf("POUT : %d	|	LED ON\n",POUT);
		}
		if(BUTTON_GPIORead(PIN) == 0){
			pressed = 1;
			printf("POUT : %d	|	Button Clicked\n",POUT2);
		}
		usleep(100000);
	}

	if(-1==LED_GPIOUnexport(POUT))
		return(4);
	if(-1==BUTTON_GPIOUnexport(POUT2)||-1==BUTTON_GPIOUnexport(PIN))
		return(4);
	return(0);
}
