#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"

int is_large(int num1, int num2)
{
	return num1 > num2;
}

int sum_x(int x1, int x2)
{
	int sum = 0;
	/* Fill this function */
	sum=x1+x2;
	return sum;
}

void sub_y(int y1, int y2, int *sub)
{
	/* Fill this function */
	*sub=y1-y2;
}

// You have to allocate memory for pointer members of "struct Point_ref"
// Hint: Use malloc()
void Point_val_to_Point_ref(struct Point_val *P1, struct Point_ref *P2)
{
	/* Fill this function */
	P2->x=malloc(sizeof(int));
	P2->y=malloc(sizeof(int));
	*P2->x=(P1->x);
	*P2->y=(P1->y);
}

void Point_ref_to_Point_val(struct Point_ref *P1, struct Point_val *P2)
{
	/* Fill this function */
	P2->x=*(P1->x);
	P2->y=*(P1->y);
}

int calc_area1(struct Point_val *P1, struct Point_val *P2)
{
	/* Fill this function */
	int area = 0;
	int width=0;
    	int height=0;
    	if(is_large(P1->x,P2->x)==1){
        	if(is_large(P1->y,P2->y)==1){
           	 width=(P1->x)-(P2->x);
          	  height=(P1->y)-(P2->y);
        	}
        	else{
            	width=(P1->x)-(P2->x);
            	height=(P2->y)-(P1->y);
        	}
    	}
    	else{
        	if(is_large(P1->y,P2->y)==1){
            	width=(P2->x)-(P1->x);
            	height=(P1->y)-(P2->y);
        	}
        	else{
            	width=(P2->x)-(P1->x);
            	height=(P2->y)-(P1->y);
       	 	}
    	}
    	area=width*height;
	return area;
}

void calc_area2(struct Point_ref *P1, struct Point_ref *P2, int *area)
{
	/* Fill this function */
	int width=0;
     	int height=0;
	int p1_x=*(P1->x);
	int p1_y=*(P1->y);
	int p2_x=*(P2->x);
	int p2_y=*(P2->y);
   	if(is_large(p1_x,p2_x)==1){
   	    if(is_large(p1_y,p2_y)==1){
   	         width=(p1_x)-(p2_x);
   	         height=(p1_y)-(p2_y);
   	    }
   	    else{
  	         width=(p1_x)-(p2_x);
  	         height=(p2_y)-(p1_y);
  	    }
  	}
  	else{
  	    if(is_large(p1_y,p2_y)==1){
  	         width=(p2_x)-(p1_x);
  	         height=(p1_y)-(p2_y);
  	    }
  	    else{
  	         width=(p2_x)-(p1_x);
  	         height=(p2_y)-(p1_y);
  	    }
  	}
  	*area=width*height;
}

char* reverse(char *word)
{
	/* Fill this function */
	int i=0;
	int count=0;
	while(word[i++]>0){
		count++;
	}
	char temp;
	for(i=0;i<count/2;i++){
		temp=word[i];
		word[i]=word[count-i-1];
		word[count-i-1]=temp;
	}
	return word;
}
