#include <stdio.h>
#include <string.h>

struct employee {
	char name[10];
	int year;
	int pay;
};

void main() {
	int i;
	struct employee em[2] = {
		{"Jack", 2014, 4200},
	{"Jone", 2015, 3300}
	};
	for (i = 0; i < 2; i++) {
		printf("\n Name : %s", em[i].name);
		printf("\n Year : %d", em[i].year);
		printf("\n Salary : %d \n", em[i].pay);
	}
	getchar();
}