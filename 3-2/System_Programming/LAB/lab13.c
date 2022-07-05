#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IN 0
#define OUT 1
#define PWM 0

#define LOW 0
#define HIGH 1
#define VALUE_MAX 256

#define DIRECTION_MAX 45
#define BUFFER_MAX 3


#define PIN_1 20
#define POUT_1 21
#define POUT 23
#define PIN 24

double distance = 0;
int state = 1;
int prev_state = 1;
int serv_sock,clnt_sock = -1;
struct sockaddr_in serv_addr,clnt_addr;
socklen_t clnt_addr_size;
char msg[4];

void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

static int PWMExport(int pwmnum){
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

static int PWMUnexport(int pwmnum){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/pwm/pwmchip0/unexport",O_WRONLY);
	if(-1==fd){
		fprintf(stderr,"Failed to open in unexport!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d",pwmnum);
	write(fd, buffer, bytes_written);
	close(fd);
	sleep(1);
	return(0);
}

static int PWMEnable(int pwmnum){
	static const char s_unenable_str[]="0";
	static const char s_enable_str[]="1";
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

static int PWMUnable(int pwmnum){
	static const char s_unable_str[]="0";
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/pwm/pwmchip0/pwm%d/enable",pwmnum);
	fd = open(path,O_WRONLY);
	if(-1==fd){
		fprintf(stderr, "Failed to open in enable!\n");
		return -1;
	}

	write(fd,s_unable_str,strlen(s_unable_str));
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

void *ultrawave_thd(){
	clock_t start_t, end_t;
	double time;

	if(-1==GPIOExport(POUT) || -1 == GPIOExport(PIN)){
		printf("gpio export err\n");
		exit(0);
	}

	usleep(100000);

	if(-1==GPIODirection(POUT,OUT)|| -1 == GPIODirection(PIN,IN)){
		printf("gpio direction err\n");
		exit(0);
	}

	GPIOWrite(POUT,0);
	usleep(10000);

	while(1){
		if(-1==GPIOWrite(POUT,1)){
			printf("gpio wirte/trigger err\n");
			exit(0);
		}

		usleep(10);
		GPIOWrite(POUT,0);

		while(GPIORead(PIN)==0){
			start_t = clock();
		}

		while(GPIORead(PIN)==1){
			end_t = clock();
		}

		time = (double)(end_t-start_t)/CLOCKS_PER_SEC;
		distance = time/2*34000;

		if(distance > 900)
			distance = 900;

		//printf("time : %.4lf\n",time);
		//printf("distance : %.2lfcm\n",distance);

		state = GPIORead(PIN_1);

		if(prev_state == 0 && state ==1){
			//light = (light+1)%2;
			//snprintf(msg,2,"%d",light);
			sprintf(msg,"%d",(int)distance);
			write(clnt_sock,msg,sizeof(msg));
			printf("distance = %dcm\n",(int)distance);
		}

		prev_state = state;

		usleep(500000);
	}
}

void *led_thd(){
	int target_bright = 0;
	int prev_bright = 0;

	PWMExport(PWM);
	PWMWritePeriod(PWM,20000000);
	PWMWriteDutyCycle(PWM,0);
	PWMEnable(PWM);


	while (1){
		target_bright = (int)1/distance * 22000 * 900;

		if(target_bright > prev_bright){
			for(int i = prev_bright; i <target_bright; i+=4000){
				PWMWriteDutyCycle(PWM,i);
				usleep(1000);
			}
		}

		else if(target_bright <= prev_bright){
			for(int i = prev_bright; i > target_bright; i-=4000){
				PWMWriteDutyCycle(PWM,i);
				usleep(1000);
			}
		}

		prev_bright = target_bright;
		usleep(100000);
	}
	exit(0);
}


int main(int argc, char *argv[]){
	//ultrawave_thd();
	//PWMExport(PWM);
	//PWMWritePeriod(PWM,20000000);
	//PWMWriteDutyCycle(PWM,20000000);
	//PWMEnable(PWM);
//	led_thd();

	if(-1 == GPIOExport(PIN_1) || -1 == GPIOExport(POUT_1))
		return(1);

	if(-1 == GPIODirection(PIN_1,IN) || -1 == GPIODirection(POUT_1,OUT))
		return(2);

	if(-1==GPIOWrite(POUT_1,1))
		return(3);

	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0 );
	if(serv_sock == -1)
		error_handling("scoket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock,5) ==-1)
		error_handling("listen() error");

	if(clnt_sock<0){
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
		if(clnt_sock == -1)
			error_handling("accept() error");
	}

	pthread_t p_thread[2];
	int thr_id;
	int status;
	char p1[] = "thread_1";
	char p2[] = "thread_2";

	thr_id = pthread_create(&p_thread[0],NULL, ultrawave_thd, NULL);
	if(thr_id < 0){
		perror("thread create error : ");
		exit(0);
	}
	printf("Hi\n");
	thr_id = pthread_create(&p_thread[1],NULL, led_thd, NULL);
	if(thr_id<0){		
		perror("thread create error : ");
		exit(0);
	}

	pthread_join(p_thread[0],NULL);
	pthread_join(p_thread[1],NULL);

	if(-1==GPIOUnexport(POUT) || -1 == GPIOUnexport(PIN))
		return(-1);

	PWMUnexport(PWM);

	close(clnt_sock);
	close(serv_sock);
	return 0;
}
