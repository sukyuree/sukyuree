#include <stdio.h>
#include <errno.h>
#include <string.h>
 
#include <wiringPi.h>
#include <softTone.h>
 
#define PIN 28


//char noteNames[] = {'C','D','E','F','G','a','b','c'};

//int frequency[] = {1047,1175,1319,1397,1568,1760,1976,2093}; 


//byte noteCount = sizeof(noteNames);

int scale [49] = {1047,1047,1568,1568,1760,1760,1568,0,1397,1397,1319,1319,1175,1175,1047,0,0,1568,1568,1397,1397,1319,1319,1175,0,1568,1568,1397,1397,1319,1319,1175,0,0,1047,1047,1568,1568,1760,1760,1568,0,1397,1397,1319,1319,1175,1175,1047};

//char score[] = "CCGGaaGFFEEDDC  GGFFEEDGGFFEED  CCGGaaGFFEEDDC";

 
int main ()
{
	int i;
	wiringPiSetup ();
	softToneCreate(PIN);
	for (i = 0 ; i < 49 ; ++i){
		printf ("%3d\n", i);
		softToneWrite(PIN, scale [i]);
		delay (500);
	}
}
