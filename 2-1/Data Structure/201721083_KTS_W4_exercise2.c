#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 100
#define MAZE_SIZE 6

typedef struct StackObjectRec {
	short r;
	short c;
} StackObject;

StackObject stack[MAX_STACK_SIZE];
int top = -1;

void initialize() {
	top = -1;
}

bool isEmpty() {
	return(top == -1);
}
bool isFull() {
	return (top == (MAX_STACK_SIZE - 1));
}
void push(StackObject item) {
	if (isFull()) {
		printf("stack is full\n");
		exit(-1);
	}
	else
		stack[++top] = item;
}

StackObject pop() {
	if (isEmpty()) {
		printf("stack is empty\n");
		exit(-1);
	}
	else
		return stack[top--];
}

void printStack() {
	int i;
	for (i = 5; i > top; i--)
		printf("l    l\n");
	for (i = top; i >= 0; i--)
		printf("l(%01d,%01d)l\n", stack[i].r, stack[i].c);
	printf("-------\n\n");
	printf("====================\n");
}

StackObject here = { 0,0 };
StackObject entry = { 0,0 };

char maze[MAZE_SIZE][MAZE_SIZE] = {
	{'e','0','1','1','1','1'},
	{'1','0','1','0','0','0'},
	{'1','0','0','0','1','0'},
	{'1','0','1','1','0','0'},
	{'1','0','1','0','0','1'},
	{'1','1','1','0','0','x'},
};

void printMaze(char m[MAZE_SIZE][MAZE_SIZE]) {
	int r, c;
	printf("\n");
	for (r = 0; r < MAZE_SIZE; r++) {
		for (c = 0; c < MAZE_SIZE; c++) {
			if (c == here.c && r == here.r)
				printf("m");
			else {
				if (m[r][c] == 0) printf("0");
				else printf("%c", m[r][c]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void pushLoc(int r, int c) {
	if (r < 0 || c < 0)
		return;
	if (maze[r][c] != '1' && maze[r][c] != '.') {
		StackObject tmp;
		tmp.r = r;
		tmp.c = c;
		push(tmp);
	}
}
bool search_maze() {
	int r, c;
	while (maze[here.r][here.c] != 'x') {
		printMaze(maze);
		r = here.r;
		c = here.c;
		maze[r][c] = '.';
		pushLoc(r - 1, c);
		pushLoc(r + 1, c);
		pushLoc(r, c - 1);
		pushLoc(r, c + 1);
		if (isEmpty()) {
			return false;
		}
		else
			here = pop();
	}
	return true;
}

int main() {
	bool result;
	printf("학번 201721083 이름 김태석\n\n");

	here = entry;
	printf("====================\n");
	printf("Start\n");
	printf("====================\n");
	printMaze(maze);
	printStack();

	result = search_maze();

	printf("====================\n");
	if (result == true) {
		printf("Success!!\n");
	}
	else {
		printf("Fail!!\n");
	}
	printf("====================\n");
	printMaze(maze);
	printStack();

	getchar();
	return 0;
}