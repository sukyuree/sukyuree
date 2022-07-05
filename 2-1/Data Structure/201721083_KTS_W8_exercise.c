#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct treeNode{
    char data;
    struct treeNode *left;
    struct treeNode *right;
    int isThreadRight;
}treeNode;

treeNode* makeNode(char data, treeNode* leftNode,treeNode* rightNode,int isThreadRight){
    treeNode*node=(treeNode*)malloc(sizeof(treeNode));
    node->data = data;
    node->left=leftNode;
    node->right = rightNode;
    node->isThreadRight = isThreadRight;
    return node;
}

treeNode* findThreadPredecessor(treeNode*p){
    treeNode *q = p->left;
    if(q->isThreadRight ==1) return q;
    if(q==NULL) return q;
    while(q->right!=NULL) q= q->right;
    return q;
}

void*insertLeft(treeNode*p,treeNode *r){
    treeNode* temp;
    r->left=p->left;
    r->right=p;
    r->isThreadRight = 1;
    if(r->left !=NULL){
        temp = findThreadPredecessor(r);
        temp->right = r;
        temp->isThreadRight = 1;
    }
    return NULL;
}

void*insertRight(treeNode*p,treeNode*r){
    treeNode* temp;
    r->right = p->right;
    r->isThreadRight=0;
    p->right = r;
    return NULL;
}
treeNode *findThreadSuccessor(treeNode*p){
    treeNode *q = p->right;
    if(p->isThreadRight==1) return q;
    if(q==NULL) return q;
    while(q->left != NULL)q=q->left;
    return q;
}

void threadInorder(treeNode*root){
    treeNode *q;
    q=root;
    while(q->left) q=q->left;
    do{
        printf("%3c",q->data);
        q=findThreadSuccessor(q);
    }while(q);
}


int main(){
    treeNode*n7=makeNode('G',NULL,NULL,0);
    treeNode*n6=makeNode('f',NULL,NULL,1);
    treeNode*n5=makeNode('E',NULL,NULL,1);
    treeNode*n4=makeNode('D',NULL,NULL,1);
    treeNode*n3=makeNode('C',n6,n7,0);
    treeNode*n2=makeNode('B',n4,n5,0);
    treeNode*n1=makeNode('A',n2,n3,0);
    
    n4->right=n2;
    n5->right=n1;
    n6->right=n3;
    printf("\n Inorder of a threaded binary tree : \n");
    threadInorder(n1);
    treeNode* r1 = makeNode('H',NULL,NULL,1);
    insertLeft(n2,r1);
    treeNode* r2=makeNode('I',NULL,NULL,1);
    insertRight(n3,r2);
    printf("\n Inorder of a threaded binary tree : \n");
    threadInorder(n1);
    getchar();
}
