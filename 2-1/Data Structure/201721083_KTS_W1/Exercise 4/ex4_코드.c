#pragma warning(disable: 4996)
#include<stdio.h>
#include<time.h>
#define MAX_LENGTH 51

unsigned long long recursive_fibo(unsigned int n)
{
	if (n == 0) return 0;
	if (n == 1) return 1;
	return recursive_fibo(n - 1) + recursive_fibo(n - 2);
}
unsigned long long iterative_fibo(unsigned int n)
{
	unsigned long long previous = 0;
	unsigned long long current = 1;
	unsigned long long next = 1;
	int i;
	if (n == 0) return 0;
	for (int i = 1; i < n; ++i)
	{
		next = previous + current;
		previous = current;
		current = next;
	}
	return current;
}

int main() {
	unsigned int length;
	double begin, end;
	double t1, t2;
	unsigned long long val;

	for (length = 1; length <= MAX_LENGTH; length += 5)
	{
		begin = clock();
		val = recursive_fibo(length);
		end = clock();
		t1 = (end - begin) / CLOCKS_PER_SEC;

		begin = clock();
		val = iterative_fibo(length);
		end = clock();
		t2 = (end - begin) / CLOCKS_PER_SEC;

		printf("[Length : %3d] Recursive : %.3lf sec, Iterative : %.3lf sec\n", length, t1, t2);
	}
	getchar();
	return 0;
}