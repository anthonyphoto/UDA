#include <stdio.h>
#include <string.h>

typedef unsigned short 	sint;
typedef unsigned int 	uint;
typedef unsigned char	uchar;

//IF �޽���HEADER -------------------------------------------------------
struct IF_MSGHEADER {
	uint	MessageId;				// �ʼ�
	char	SytemId[6];
	char	ApplicationId[6];
	char	Date[8];
	uint	Tid;
	uint	DataSize;				// �ʼ�
};	
typedef IF_MSGHEADER	msgHeader;
//IF �޽���HEADER -------------------------------------------------------


int main(void)
{
	char 		 buf[1024];
	msgHeader	 mheader;
	
	mheader.DataSize=4;
	memcpy(buf, (char *)&mheader, sizeof(msgHeader));
	
	
	printf("\n [%d] \n", ((msgHeader *)(buf))->DataSize);
	
}