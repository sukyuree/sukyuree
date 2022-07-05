#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTEX 30

typedef struct graphNode{
    int vertex;
    struct graphNode* link;
}graphNode;

typedef struct graphType{
    int n;
    graphNode* adjList_H[MAX_VERTEX];
}graphType;

void createGraph(graphType* g){
    int v;
    g->n=0;
    for(v=0; v<MAX_VERTEX; v++)
        g->adjList_H[v]=NULL;
}

void insertVertex(graphType* g, int v){
    if(((g->n)+1)>MAX_VERTEX){
        printf("\n Graph Full!");
        return;
    }
    g->n++;
}
void insertEdge(graphType* g, int u, int v){
    graphNode* node;
    if(u>=g->n|v>=g->n){
        printf("no vertex existed");
        return;
    }
    node = (graphNode*)malloc(sizeof(graphNode));
    node->vertex = v;
    node->link = g->adjList_H[u];
    g->adjList_H[u] =node;
}

void print_adjList(graphType* g){
    int i;
    graphNode* p;
    for(i=0;i<g->n;i++){
        printf("\n\t\t %c", i+65);
        p=g->adjList_H[i];
        while(p){
            printf("->%c", p->vertex+65);
            p=p->link;
        }
    }
}

int main(){
    int i;
    graphType *G1, *G2;
    G1=(graphType*)malloc(sizeof(graphType));
    G2=(graphType*)malloc(sizeof(graphType));
    createGraph(G1);
    createGraph(G2);
    
    for(i=0;i<4;i++)
        insertVertex(G1,i);
    insertEdge(G1,0,3);
    insertEdge(G1,0,1);
    insertEdge(G1,1,3);
    insertEdge(G1,1,2);
    insertEdge(G1,1,0);
    insertEdge(G1,2,3);
    insertEdge(G1,2,1);
    insertEdge(G1,3,2);
    insertEdge(G1,3,1);
    insertEdge(G1,3,0);
    printf("\n G1 : ");
    print_adjList(G1);
    
    for(i=0;i<3;i++)
        insertVertex(G2,i);
    
    insertEdge(G2,0,2);
    insertEdge(G2,0,1);
    insertEdge(G2,1,2);
    insertEdge(G2,1,0);
    insertEdge(G2,2,1);
    insertEdge(G2,2,0);
    printf("\n G2 : ");
    print_adjList(G2);
    getchar();
    return 0;
    
}
