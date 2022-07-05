#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdint.h>

#define IN 0
#define OUT 1
#define PWM 2
#define LOW 0
#define HIGH 1
#define VALUE_MAX 256
#define PIN 20
#define POUT 21

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

static const char *DEVICE = "/dev/spidev0.0";
static uint8_t MODE = SPI_MODE_0;
static uint8_t BITS = 8;
static uint32_t CLOCK = 1000000;
static uint16_t DELAY = 5;


static int PWMExport(int pwmnum){
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	int bytes_written;
	int fd;

	fd = open("/sys/class/pwm/pwmchip0/unexport",O_WRONLY);
	if (fd == -1){
		fprintf(stderr,"Failed to open in unexport!\n");
		return(-1);
	}

	bytes_written - snprintf(buffer, BUFFER_MAX,"%d", pwmnum);
	write(fd, buffer, bytes_written);
	close(fd);

	sleep(1);
	fd = open("/sys/class/pwm/pwmchip0/export",O_WRONLY);
	if(fd==-1){
		fprintf(stderr,"Failed to open in export!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer,BUFFER_MAX,"%d",pwmnum);
	write(fd, buffer, bytes_written);
	close(fd);
	sleep(1);
	return(0);
}

static int PWMEnable(int pwmnum){
	static const char s_unenable_str[]="0";
	static const char s_enable_str[]="1";

#define DIRECTION_MAX 45
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX,"/sys/class/pwm/pwmchip0/pwm%d/enable",pwmnum);
	fd = open(path,O_WRONLY);
	if(fd==-1){
		fprintf(stderr,"Failed to open in enbale!\n");
		return (-1);
	}
	
	write(fd,s_unenable_str,strlen(s_unenable_str));
	close(fd);

	fd = open(path,O_WRONLY);
	if(fd==-1){
		fprintf(stderr,"Failed to open in enable!\n");
		return -1;
	}

	write(fd,s_enable_str,strlen(s_enable_str));
	return(0);
}

static int PWMWritePeriod(int pwmnum, int value){
	char s_values_str[VALUE_MAX];
	char path[VALUE_MAX];
	int fd,byte;

	snprintf(path, VALUE_MAX,"/sys/class/pwm/pwmchip0/pwm%d/period",pwmnum);
	fd = open(path, O_WRONLY);
	if(fd==-1){
		fprintf(stderr,"Failed to open in period!\n");
		return(-1);
	}

	byte = snprintf(s_values_str,10,"%d",value);
	
	if(write(fd,s_values_str,byte)==-1){
		fprintf(stderr,"Failed to write value in period!\n");
		close(fd);
		return(-1);
	}

	close(fd);
	return(0);
}

static int PWMWriteDutyCycle(int pwmnum, int value){
	char path[VALUE_MAX];
	char s_values_str[VALUE_MAX];
	int fd,byte;

	snprintf(path,VALUE_MAX,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwmnum);
	fd = open(path,O_WRONLY);
	if(fd ==-1){
		fprintf(stderr,"Failed to open in duty_cycle!\n");
		return(-1);
	}

	byte = snprintf(s_values_str,10,"%d",value);
	if(write(fd,s_values_str,byte)==-1){
		fprintf(stderr,"Failed to write value! in duty_cycle\n");
		close(fd);
		return(-1);
	}
	close(fd);
	return(0);
}

static int prepare(int fd) {

	if(ioctl(fd,SPI_IOC_WR_MODE,&MODE)==-1){
		perror("Cant set MODE");
		return -1;
	}

	if(ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&BITS)==-1){
		perror("Cant set number of BITS");
		return -1;
	}

	if(ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ,&CLOCK)==-1){
		perror("Can't set write CLOCK");
		return -1;
	}

	if (ioctl(fd,SPI_IOC_RD_MAX_SPEED_HZ,&CLOCK)==-1){
		perror("Can't set read CLOCK");
		return -1;
	}

	return 0;
}

uint8_t control_bits_differential(uint8_t channel){
	return (channel & 7) << 4;
}

uint8_t control_bits(uint8_t channel){
	return 0x8 | control_bits_differential(channel);
}

int readadc(int fd, uint8_t channel){
	uint8_t tx[] = {1,control_bits(channel),0};
	uint8_t rx[3];

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = DELAY,
		.speed_hz = CLOCK,
		.bits_per_word = BITS,
	};

	if(ioctl(fd,SPI_IOC_MESSAGE(1),&tr)==1){
		perror("IO ERROR");
		abort();
	}

	return ((rx[1] << 8)&0x300) | (rx[2] & 0xFF);
}
int main(void){
	int light = 0;

	PWMExport(0);
	PWMWritePeriod(0,20000000);
	PWMWriteDutyCycle(0,0);
	PWMEnable(0);

	int fd = open(DEVICE,O_RDWR);
	if(fd<=0){
		printf("Device %s not found\n",DEVICE);
		return -1;
	}

	if(prepare(fd)==-1){
		return -1;
	}

	while(1){
		light = readadc(fd,0);
		printf("%d\n",light);
		PWMWriteDutyCycle(0,light*7500);
		usleep(1000);
	}
	close(fd);

	return 0;
}



