#pragma warning(disable : 4996)
#include<stdio.h>

long long fact(int);

int main()
{
	int n;
	long long result;
	
	printf("Input 'N'\n");
	scanf_s("%d", &n);

	result = fact(n);

	printf("\n\n factorial(%d) = %ld.\n",n, result);
	 
	getchar(); getchar();
}

long long fact(int n)
{
	long long value;

	if(n <= 1)
	{
		printf("\n fact(1) called!");
		printf("\n fact(1) returned!!");

		return 1;
	}
	else {
		printf("\n fact(%d) called!", n);

		value = (n * fact(n - 1));

		printf("\n fact(%d) returned value : %d!!", n, value);
		return value;
	}
}