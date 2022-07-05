#pragma warning(disable: 4996)
#include<stdio.h>

struct st {
	int a;
	char ch;
};

int main()
{
	struct st obj;
	struct st* stobj = &obj;

	stobj->a = 5;
	stobj->ch = 'a';

	printf("\n [%d] [%c]\n", stobj->a, stobj->ch);

	getchar();

	return 0;
}