#include <stdio.h>

void main() {
	int i;
	char string1[20] = "Dreams come true!", string2[20], * ptr1;
	ptr1 = string1;
	printf("\n %u", string1);
	printf("\n %u", ptr1);
	printf("\n %s", string1);
	printf("\n %s", ptr1);
	printf("\n\n %s\n", (ptr1 + 7));

	for (i = 16; i >= 0; i--) {
		putchar(*(ptr1 + i));
	}
	for (i = 0; i < 20; i++) {
		string2[i] = *(ptr1 + i);
	}
	printf("\n %s", string2);
	*(ptr1 + 0) = 'P';
	printf("\n ptr1 = %s", ptr1);
	*(ptr1 + 1) = 'e';
	printf("\n ptr1 = %s", ptr1);
	getchar();
}