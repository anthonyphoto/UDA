#include <stdio.h>

int chkfile(char path[],char sub[],char fname[])
{
	FILE *ptr;
	char buff[255];
	int flag=0;

	sprintf(buff,"ls -t /home2/pentacom/5ESS/%s/%s >data",path,sub);
	system(buff);
	ptr = fopen("data","r");
	if(ptr == NULL) {
		fname[0]=NULL;
		return(-1);
	}
	flag=1;
	while(fscanf(ptr,"%s",buff) != EOF) {
		strcpy(fname,buff);	
		buff[0] = NULL;
		flag=0;
	}
	fclose(ptr);
	return(flag);
}
	
