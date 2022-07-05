#include<stdio.h>
#include<string.h>

int failure[20] = { 0, };

void fail(char* pat) {
	int i, j;
	int n = strlen(pat);
	failure[0] = -1;
	for (j = 1; j < n; j++) {
		i = failure[j - 1];
		while (pat[j] != pat[i + 1] && i >= 0)
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else
		{
			failure[j] = -1;
		}
	}
}

int pmatch(char* string, char* pat) {
	int i = 0;
	int j = 0;
	int lens = strlen(string);
	int lenp = strlen(pat);

	while (i < lens && j < lenp) {
		if (string[i] == pat[j]) {
			i++;
			j++;
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}

	return j == lenp ? i - lenp : -1;
}

int main() {
	char string[] = "Ajou university is the best in the world.";
	char* pat = "best";

	fail(pat);
	int matched_count = pmatch(string, pat);

	if (matched_count == -1) {
		printf("No pattern found");
	}
	else {
		printf("Index of the first pattern found: %d", matched_count);
	}

	getchar();
}