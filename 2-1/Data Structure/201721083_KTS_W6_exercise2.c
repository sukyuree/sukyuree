#include <stdio.h>
#include<malloc.h>

typedef char element;
typedef struct QNode {
	element data;
	struct QNode* link;
}QNode;

typedef struct {
	QNode* front, * rear;
}LQueueType;

LQueueType *createLinkedQueue(){
	LQueueType* LQ;
	LQ = (LQueueType*)malloc(sizeof(LQueueType));
	LQ->front = NULL;
	LQ->rear = NULL;
	return LQ;
}

int isEmpty(LQueueType* LQ) {
	if (LQ->front == NULL) {
		printf("Linked Queue is empty!");
		return 1;
	}
	else return 0;
}

void enQueue(LQueueType* LQ, element item) {
	QNode* newNode = (QNode*)malloc(sizeof(QNode));
	newNode->data = item;
	newNode->link = NULL;

	if (LQ->front == NULL) {
		LQ->front = newNode;
		LQ->rear = newNode;
	}
	else {
		LQ->rear ->link = newNode;
		LQ->rear = newNode;
	}
}

element deQueue(LQueueType* LQ) {
	QNode* old = LQ->front;
	element item;
	if (isEmpty(LQ)) return 0;
	else {
		item = old->data;
		LQ->front = old->link;
		free(old);
		return item;
	}
}

void printLQ(LQueueType* LQ) {
	QNode* temp = LQ->front;
	printf("Linked Queue: [");
	while (temp) {
		printf("%3c", temp->data);
		temp = temp->link;
	}
	printf("]");
}

void main(void) {
	LQueueType* LQ = createLinkedQueue();
	element data;

	printf("\n***** Queue *****\n");
	printf("\n enqueue C >>>"); enQueue(LQ, 'C'); printLQ(LQ);
	printf("\n enqueue A >>>"); enQueue(LQ, 'A'); printLQ(LQ);
	printf("\n enqueue B >>>"); enQueue(LQ, 'B'); printLQ(LQ);
	printf("\n enqueue D >>>"); enQueue(LQ, 'D'); printLQ(LQ);
	printf("\n enqueue E >>>"); enQueue(LQ, 'E'); printLQ(LQ);

	printf("\n dequeue   >>>"); data = deQueue(LQ); printLQ(LQ);
	printf("\tdequeued data: %c", data);
	printf("\n dequeue   >>>"); data = deQueue(LQ); printLQ(LQ);
	printf("\tdequeued data: %c", data);
	printf("\n dequeue   >>>"); data = deQueue(LQ); printLQ(LQ);
	printf("\tdequeued data: %c", data);
	printf("\n dequeue   >>>"); data = deQueue(LQ); printLQ(LQ);
	printf("\tdequeued data: %c", data);
	printf("\n dequeue   >>>"); data = deQueue(LQ); printLQ(LQ);
	printf("\tdequeued data: %c", data);
}