#include <stdio.h>

void main() {
	int i;
	char* ptrArray[4] = { {"Korea"},{"Suwon"},{"AjouSoftware"},{"DataStructures"} };
	for (i = 0; i < 4; i++) {
		printf("\n %u", &ptrArray[i]);
	}

	ptrArray[3] = "Human-Computer Interaction";
	printf("\n\n");
	for (i = 0; i < 4; i++) {
		printf("\n %s", ptrArray[i]);
	}
	getchar();
}