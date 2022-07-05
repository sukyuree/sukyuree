#include <stdio.h>
#include <string.h>

struct employee {
	char name[10];
	int year;
	int pay;
};

void main() {
	struct employee em1;
	struct employee* em2 = &em1;
	strcpy(em2->name, "You");
	em2->year = 2015;
	em2->pay = 5900;
	printf("\n Name : %s", em2->name);
	printf("\n Year : %d", em2->year);
	printf("\n Salary : %d", em2->pay);
	printf("\n Name : %s", em1.name);
	printf("\n Year : %d", em1.year);
	printf("\n Salary : %d", em1.pay);
}