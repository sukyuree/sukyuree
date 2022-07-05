//
//  main.c
//  exercise2
//
//  Created by 김태석 on 2020/04/27.
//  Copyright © 2020 김태석. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define MALLOC(p,s) (p) = (nodePointer) malloc((s));

typedef struct node *nodePointer;
typedef struct node{
    int data;
    nodePointer link;
};

int main(){
    bool out[MAX_SIZE];
    nodePointer seq[MAX_SIZE];
    nodePointer x,y,top;
    int i,j,n;
    
    printf("Enter the number of polygons(<=%d)",MAX_SIZE);
    scanf("%d", &n);
    for(i=0; i<n;i++){
        out[i] = true;
        seq[i] = NULL;
    }
    printf("Enter a pair of numbers(-1 -1 to quit): ");
    while(1){
        scanf("%d %d", &i, &j);
        if(i<0||j<0){
            break;
        }
        MALLOC(x,sizeof(x));
        x->data = j; x->link = seq[i]; seq[i]=x;
        MALLOC(x,sizeof(x));
        x->data = i; x->link = seq[j]; seq[j]=x;
        printf("Enter a pair of numbers(-1 -1 to quit): ");
    };
    for(i=0;i<n;i++){
        if(out[i]==true){
            printf("\n New class : %5d", i);
            out[i]=false;
            x=seq[i];
            top = NULL;
            
            for(;;){
                while(x){
                    j=x->data;
                    if(out[j]==true){
                        printf("%5d",j);
                        out[j]=false;
                        y=x->link;
                        x->link=top;
                        top=x;
                        x=y;
                    }
                    else{
                        x=x->link;
                    }
                }
                if(!top) break;
                x=seq[top->data];
                top=top->link;
            }
        }
    }
    getchar();
    printf("\n\nEnter 0 to end");
}
