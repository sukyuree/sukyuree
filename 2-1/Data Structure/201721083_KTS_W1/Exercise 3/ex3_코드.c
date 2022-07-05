#pragma warning(disable : 4996)
#include <stdio.h>

void hanoi(int n, char start, char middle, char last)
{
	if (n == 1)
	{
		printf("move object %d from %c to %c\n", n, start, last);
	}
	else {
		hanoi(n-1, start, last, middle);
		printf("move object %d from %c to %c\n", n, start, last);
		hanoi(n - 1, middle, start, last);
	}
}

int main()
{
	int n;
	printf("Number of disk : ");
	scanf_s("%d", &n);

	hanoi(n, 'A', 'B', 'C');

	getchar();
	return 0;
}