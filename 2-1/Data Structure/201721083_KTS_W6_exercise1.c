#include <stdio.h>
#include <stdlib.h>
#include<string.h>

typedef int element;

typedef struct stackNode {
	element data;
	struct stackNode* link;
}stackNode;

stackNode* top;

int isEmpty() {
	if (top == NULL) return 1;
	else return 0;
}

void push(element item) {
	stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
	temp->data = item;
	temp->link = top;
	top = temp;
}

element pop() {
	element item;
	stackNode* temp = top;
	if (top == NULL) {
		printf("\n\n Stack is empty !\n");
		return 0;
	}
	else {
		item = temp->data;
		top = temp->link;
		free(temp);
		return item;
	}
}

void printStack() {
	stackNode* p = top;
	printf("\n STACK [");
	while (p) {
		printf("%d", p->data);
		p = p->link;
	}
	printf("]");
}

void main(void) {
	element item;
	top = NULL;
	printf("\n** Linked Stack **\n");
	printStack();
	push(4); printStack();
	push(3); printStack();
	push(1); printStack();
	push(7); printStack();
	push(5); printStack();

	item = pop();
	printStack();
	printf("\t pop => %d", item);
	item = pop();
	printStack();
	printf("\t pop => %d", item);
	item = pop();
	printStack();
	printf("\t pop => %d", item);
	item = pop();
	printStack();
	printf("\t pop => %d", item);
	item = pop();
	printStack();
	printf("\t pop => %d", item);
	getchar();
}