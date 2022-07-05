//
//  main.m
//  exercise3
//
//  Created by 김태석 on 2020/04/27.
//  Copyright © 2020 김태석. All rights reserved.
//

#include<stdbool.h>
#include<stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct treeNode{
    char data;
    struct treeNode *left;
    struct treeNode *right;
} treeNode;
treeNode* makeNode(char data, treeNode* leftNode, treeNode* rightNode){
    treeNode* root =(treeNode*)malloc(sizeof(treeNode));
    root->data = data;
    root->left = leftNode;
    root ->right = rightNode;
    return root;
}
void Preorder(treeNode* root){
    if(root != NULL){
        printf("[%c]", root->data);
        Preorder(root->left);
        Preorder(root->right);
    }
}
void inorder(treeNode * root){
    if(root!=NULL){
        inorder(root->left);
        printf("[%c]", root->data);
        inorder(root->right);
    }
}

void Postorder(treeNode* root){
    if(root!= NULL){
        Postorder(root->left);
        Postorder(root->right);
        printf("[%c]", root->data);
    }
}

int main(){
    treeNode* n7 = makeNode('D',NULL,NULL);
    treeNode* n6 = makeNode('C',NULL,NULL);
    treeNode* n5 = makeNode('B',NULL,NULL);
    treeNode* n4 = makeNode('A',NULL,NULL);
    treeNode* n3 = makeNode('/',n6,n7);
    treeNode* n2 = makeNode('*',n4,n5);
    treeNode* n1 = makeNode('-',n2,n3);
    
    printf("Preorder : ");
    Preorder(n1);
    printf("\n");
    
    printf("inorder : ");
    inorder(n1);
    printf("\n");
    
    printf("postorder : ");
    Postorder(n1);
    printf("\n");
    
    getchar();
    return 0;
}
