#include <stdio.h>

void main() {
	char* ptrArray[2];
	char** ptrptr;
	int i;

	ptrArray[0] = "Korea";
	ptrArray[1] = "Suwon";

	ptrptr = ptrArray;

	printf("\n %u", &ptrArray[0]);
	printf("\n %c", ptrArray[0]);
	printf("\n %c", ptrArray[0][0]);
	printf("\n %c", ptrArray[0][1]);
	printf("\n %s \n\n", ptrArray[0]);

	printf("\n %u", &ptrArray[1]);
	printf("\n %c", ptrArray[1]);
	printf("\n %c", ptrArray[1][0]);
	printf("\n %c", ptrArray[1][1]);
	printf("\n %s \n\n", ptrArray[1]);

	printf("\n %u", &ptrptr);
	printf("\n %u", ptrptr);
	printf("\n %c", *ptrptr);
	printf("\n %c", *(ptrptr[0] + 0));
	printf("\n %c", *(ptrptr[0] + 1));
	printf("\n %s", *ptrptr);



}