#include <stdio.h>
#include <limits.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 5
#define INF 10000



int weight[MAX_VERTICES][MAX_VERTICES]={
    {0,10,5,INF,INF},
    {INF,0,2,1,INF},
    {INF,3,0,9,2},
    {INF,INF,INF,0,4},
    {7,INF,INF,6,0}
};

int distance[MAX_VERTICES];
int S[MAX_VERTICES];

int printStep(int step){
    int i;
    printf("\n %3d step : S = {",step);
    for(i=0; i<MAX_VERTICES;i++){
        if(S[i]==TRUE)
            printf("%3c",i+65);
    }
    if(step<1)
        printf("}\t\t\t\t");
    else if(step<3)
        printf("}\t\t\t");
    else if(step<4)
        printf("}\t\t");
    else
        printf("}\t");
    printf("distance : [");
    for(i=0; i<MAX_VERTICES; i++){
        if(distance[i]==10000)
            printf("%4c",'*');
        else
            printf("%4d",distance[i]);
        
    }
    printf("%4c",']');
    return ++step;
}

int choose(int n){
    int i,min,minPos;
    min = INT_MAX;
    minPos = -1;
    for(i=0;i<n;i++){
        if((distance[i])<min&&!S[i]){
            min = distance[i];
            minPos = i;
        }
    }
    return minPos;
}

void Dijkstra(int start, int n){
    int i,u,w,step = 0;
    for(i=1;i<n;i++){
        distance[i]=weight[start][i];
        S[i]=FALSE;
    }
    S[start] = TRUE;
    distance[start]=0;
    step = printStep(0);
    for(i=0;i<n-1;i++){
        u=choose(n);
        S[u]=TRUE;
        for(w=0;w<n;w++){
            if(!S[w]){
                if(distance[u]+weight[u][w]<distance[w]){
                    distance[w]=distance[u]+weight[u][w];
                }
            }
        }
        step = printStep(step);
    }
}


int main(void){
    int i,j;
    printf("\n**********adj matrix**********\n\n");
    for(i=0;i<MAX_VERTICES;i++){
        for(j=0;j<MAX_VERTICES;j++){
            if(weight[i][j]==10000)
                printf("%4c",'*');
            else
                printf("%4d",weight[i][j]);
        }
        printf("\n\n");
    }
    printf("\n**********Dijkstra shortest path**********\n\n");
    Dijkstra(0,MAX_VERTICES);
}

