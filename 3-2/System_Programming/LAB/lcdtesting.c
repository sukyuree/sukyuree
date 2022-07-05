
/*
*
* by Lewis Loflin www.bristolwatch.com lewis@bvu.net
* http://www.bristolwatch.com/rpi/i2clcd.htm
* Using wiringPi by Gordon Henderson
*
*
* Port over lcd_i2c.py to C and added improvements.
* Supports 16x2 and 20x4 screens.
* This was to learn now the I2C lcd displays operate.
* There is no warrenty of any kind use at your own risk.
*
*/

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <softTone.h>
#include <errno.h>

// Define some device parameters
#define I2C_ADDR   0x27 // I2C device address

// Define some device constants

#define SPKR 6
#define TOTAL 8

#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line

#define LCD_BACKLIGHT   0x08  // On
// LCD_BACKLIGHT = 0x00  # Off

#define ENABLE  0b00000100 // Enable bit

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define IN 0
#define OUT 1

#define LOW 0
#define HIGH 1

#define OPEN 1
#define CLOSE 0

#define PIN 28
#define POUT 17

#define PIN_BUTTON 26
#define POUT_BUTTON 21

#define PIN_BUTTON_SUPER 5
#define POUT_BUTTON_SUPER 6
#define SERVO_1 1
#define SERVO_2 26

#define VALUE_MAX 40
#define DIRECTION_MAX 45
#define BUFFER_MAX 3

int F = 0;
int STATE = 0;
int IS_OPEN = 0;
int PRESSED = 0;
int SUPER = 0;
int ONLY_ONCE = 0;
int scale [49] = {1047,1047,1568,1568,1760,1760,1568,0,1397,1397,1319,1319,1175,1175,1047,0,0,1568,1568,1397,1397,1319,1319,1175,0,1568,1568,1397,1397,1319,1319,1175,0,0,1047,1047,1568,1568,1760,1760,1568,0,1397,1397,1319,1319,1175,1175,1047};


void *play()
{
	while(1){
		if(STATE == 1){
			softToneCreate(PIN);
			int i;
			for (i = 0 ; i < 49 ; i++){
				//printf ("%3d\n", i);
				softToneWrite(PIN, scale [i]);
				usleep(500*1000);
				//ONLY_ONCE = 1;
			}
			softToneStop(PIN);
		}
	}
}

void lcd_init(void);
void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);
// added by Lewis
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); //move cursor
void ClrLcd(void); // clr LCD return home
void typeln(const char *s);
void typeChar(char val);
int fd;  // seen by all subroutines




void error_handling(char * message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
static int GPIOExport(int pin) {
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export",O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX,"%d",pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int GPIOUnexport(int pin){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport",O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"Failed to open unexport for writing!\n");
		return(-1);
	}
	bytes_written = snprintf(buffer, BUFFER_MAX,"%d",pin);
	write(fd,buffer,bytes_written);
	close(fd);
	return(0);
}

static int GPIODirection(int pin, int dir) {
	static const char s_directions_str[] = "in\0out";
	char path[DIRECTION_MAX]="/sys/class/gpio/gpio%d/direction";
	int fd;

	snprintf(path,DIRECTION_MAX,"/sys/class/gpio/gpio%d/direction",pin);

	fd = open(path,O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"failed to open gpio direction for writing!\n");
		return(-1);
	}

	if(-1==write(fd,&s_directions_str[IN==dir ? 0 : 3], IN== dir ? 2 : 3)){
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int GPIORead(int pin){
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path,VALUE_MAX,"/sys/class/gpio/gpio%d/value",pin);
	fd = open(path,O_RDONLY);
	if(fd == -1){
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if(read(fd,value_str,3)==-1){
		fprintf(stderr,"Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int GPIOWrite(int pin, int value){
	static const char s_values_str[] = "01";
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value",pin);
	fd = open(path,O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)){
		fprintf(stderr,"failed to write value!\n");
		return(-1);
	}
	close(fd);
	return(0);
}
void *servo_button(int sock){
	while(1){
		if(SUPER == 0){
			if(GPIORead(PIN_BUTTON) == 1 && IS_OPEN == OPEN && PRESSED == 1){
				PRESSED = 0;
				IS_OPEN = CLOSE;
				softPwmWrite(SERVO_1,5);
				//softPwmWrite(SERVO_2,24);
				//printf("closed");
				//printf("POUT : %d	|	LOOP CLOSE\n",POUT);
			}
			if(GPIORead(PIN_BUTTON) == 1 && IS_OPEN == CLOSE && PRESSED == 1){
				PRESSED = 0;
				IS_OPEN = OPEN;
				softPwmWrite(SERVO_1,15);
				//softPwmWrite(SERVO_2,15);
				//printf("open");
				//printf("POUT : %d	|	LOOP OPEN\n",POUT);
			}
			if(GPIORead(PIN_BUTTON) == 0){
				PRESSED = 1;
				sleep(1);
				//printf("POUT : %d	|	Button Clicked\n",POUT_BUTTON);
			}
		}
	}
}

void *servo_button_super(int sock){
	while(1){
		if(GPIORead(PIN_BUTTON_SUPER) == 1 && IS_OPEN == OPEN && PRESSED == 1){
			PRESSED = 0;
			IS_OPEN = CLOSE;
			softPwmWrite(SERVO_1,5);
			//softPwmWrite(SERVO_2,24);
			SUPER = 0;
			ONLY_ONCE = 0;
			//printf("closed");
			//printf("POUT : %d	|	LOOP CLOSE\n",POUT);
		}
		if(GPIORead(PIN_BUTTON_SUPER) == 1 && IS_OPEN == CLOSE && PRESSED == 1){
			PRESSED = 0;
			IS_OPEN = OPEN;
			softPwmWrite(SERVO_1,15);
			//softPwmWrite(SERVO_2,15);
			//printf("open");
			//printf("POUT : %d	|	LOOP OPEN\n",POUT);
		}
		if(GPIORead(PIN_BUTTON_SUPER) == 0){
			PRESSED = 1;
			sleep(1);
			//printf("POUT : %d	|	Button Clicked\n",POUT_BUTTON);
		}

	}
}
void *lcd_function_read(int sock){
	char msg_read[4];
	int str_len;
	while(1){
		if(IS_OPEN == CLOSE){
			str_len = read(sock,msg_read,sizeof(msg_read));
			F = atoi(msg_read);
			if(F>16) F=16;
			float percent = (((16.0f)-(float)F)/16.0f)*100;
			if(str_len==-1)
				error_handling("read() error");
			if(F>4&&F<=20){
				STATE = 0;
				lcdLoc(LINE1);
    				typeFloat(percent);
				lcdLoc(LINE2);
				typeln("HTTC://");
				printf("Receive message from Server : %s\n",msg_read);
				sleep(1);
			}
    			else if(F<=4){
				lcdLoc(LINE1);
				typeln("FULL");
				lcdLoc(LINE2);
				typeln("HTTC://");
				STATE = 1;
				SUPER = 1;
				//printf("%d\n",SUPER);
				sleep(1);
				//play();
				//softToneStop(PIN);
			}
    			ClrLcd();
			//printf("Receive message from Server : %s\n",msg_read);
		}
	}

}

void *lcd_function_write(int sock){
	char msg_write[2];
	while(1){
		snprintf(msg_write,2,"%d",STATE);
		write(sock,msg_write,sizeof(msg_write));
		printf("Send message To Server : %s\n",msg_write);
		
		sleep(1);

	}


}
int main(int argc, char *argv[]){

	if (wiringPiSetup () == -1) exit (1);

  	fd = wiringPiI2CSetup(I2C_ADDR);

  	//printf("fd = %d ", fd);

  	lcd_init(); // setup LCD
  	int sock;
	struct sockaddr_in serv_addr;
	int full = 300;
	char msg_test[4];
	char msg[4];
	char on[2] ="1";
	softPwmCreate(SERVO_1,0,200);
	//softPwmCreate(SERVO_2,0,200);

	//int light = 0;

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}

	if(GPIOExport(POUT)==-1)
		return -1;

	if(GPIODirection(POUT,OUT))
		return (2);

	if(-1==GPIOExport(POUT_BUTTON)||-1==GPIOExport(PIN_BUTTON))
		return(1);
	if(-1==GPIODirection(POUT_BUTTON,OUT)||-1==GPIODirection(PIN_BUTTON,IN))
		return(2);
	if(-1==GPIOWrite(POUT_BUTTON,1))
		return(2);

	if(-1==GPIOExport(POUT_BUTTON_SUPER)||-1==GPIOExport(PIN_BUTTON_SUPER))
		return(1);
	if(-1==GPIODirection(POUT_BUTTON_SUPER,OUT)||-1==GPIODirection(PIN_BUTTON_SUPER,IN))
		return(2);
	if(-1==GPIOWrite(POUT_BUTTON_SUPER,1))
		return(2);

	sock = socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error");

	pthread_t p_thread[5];
	int thr_id;
	int status;
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char p3[] = "thread_3";

	thr_id = pthread_create(&p_thread[0],NULL,lcd_function_read,sock);
	if(thr_id < 0){
		perror("thread create error : ");
		exit(0);
	}

	thr_id = pthread_create(&p_thread[1],NULL,lcd_function_write,sock);
	if(thr_id<0){
		perror("thread create error : ");
		exit(0);
	}
	
	thr_id = pthread_create(&p_thread[2],NULL,servo_button,sock);
	if(thr_id<0){
		perror("thread create error : ");
		exit(0);
	}

	thr_id = pthread_create(&p_thread[3],NULL,servo_button_super,sock);
	if(thr_id<0){
		perror("thread create error : ");
		exit(0);
	}

	thr_id = pthread_create(&p_thread[4],NULL,play,NULL);
	if(thr_id<0){
		perror("thread create error : ");
		exit(0);
	}

	pthread_join(p_thread[0],(void**)&status);
	pthread_join(p_thread[1],(void**)&status);
	pthread_join(p_thread[2],(void**)&status);
	pthread_join(p_thread[3],(void**)&status);
	pthread_join(p_thread[4],(void**)&status);
	close(sock);

	if(GPIOUnexport(POUT))
		return(4);

	if(-1==GPIOUnexport(POUT_BUTTON)||-1==GPIOUnexport(PIN_BUTTON))
		return(4);
	return(0);

}


// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

