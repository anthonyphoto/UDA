#include <stdio.h>

struct _DATANODE
{
	int iIndex;        // find disk_index node with return address.
    time_t tTime;
	int serialnumber;
	int weight;
	int price;
	char name[10];
	char address[50];
	char phone[13];
};

int main()
{
	
	struct _DATANODE  tmp;
	
	printf("struct size:%d\n", sizeof(tmp));
}

