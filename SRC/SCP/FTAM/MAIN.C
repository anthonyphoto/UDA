#include <stdio.h>
#include <time.h>
#define	MaxErr		10 

struct Type {
	char TrfPath[255];
	char AmaPath[255];
	int  StartAma;
	int  StartBlock;
	int  SWType;
	int  SWID;
	char StartTrf[255];
}Init = {
		"/home2/pentacom/NCR/ms",
		"/home2/pentacom/NCR/subrev",
		8882,1,1,0,"00001B.001" };

int timeSleep = 10;
int PrintFlag =0;

main(int argc, char *argv[])
{
	char name[255],buff[255];
	int flag =3;
	int no=0;
	int TrfErr=0,AmaErr=0;

	if(argc == 1) PrintFlag=0;
	else PrintFlag = atoi(argv[1]);
	readData();
	while(1) {
		if(flag == 3) {
			no = chkfile("ms","ready",name);
			if(no < 0) { 
				if(getTrf(Init.TrfPath,Init.StartTrf) < 0) TrfErr++;
				else TrfErr=0;
			}
			else if(no == 0) {
				strcpy(Init.StartTrf,name);
				getTrf(Init.TrfPath,Init.StartTrf);
				TrfErr=0;
				continue;	
			} else {
				TrfErr++;
			}
			flag = 0;
		}
		flag++;
         	no = chkfile("subrev","ready",name);
                if(no < 0) { 
	                if(getAma(Init.AmaPath,&Init.StartAma) < 0) AmaErr++;
			else AmaErr=0;
		}
                else if(no == 0) {
                        Init.StartAma = atoi(name);
                        getAma(Init.AmaPath,&Init.StartAma);
			AmaErr=0;
                        continue;
                }
		else {
			sprintf(buff," Sleep  %d %d ",TrfErr,AmaErr);
			outputMsg(buff);
			AmaErr++;
			sleep(timeSleep);
		}
		if(AmaErr > MaxErr) break;
		if(TrfErr > MaxErr) break;
	}
}

outputMsg(char buff[])
{
  	long          clock;
  	struct tm     *tmm;
  	char          lcl_str[256];

  	clock = time( NULL );
  	tmm = localtime( &clock );
  	sprintf( lcl_str, "%d-%02d-%02d %02d:%02d:%02d ",
        tmm->tm_year, tmm->tm_mon+1, tmm->tm_mday,
        tmm->tm_hour, tmm->tm_min, tmm->tm_sec );

	if(PrintFlag == 1) 
		printf("%s %s \n",lcl_str,buff);
}
readData()
{
        FILE *ptr;

        ptr = fopen("ftam.dat","r");
	if(ptr == NULL) return;

        if(fscanf(ptr,"%s",Init.TrfPath) == EOF) return;
        if(fscanf(ptr,"%s",Init.AmaPath) == EOF) return;
        if(fscanf(ptr,"%d",&Init.StartAma) == EOF) return;
        if(fscanf(ptr,"%d",&Init.StartBlock) == EOF) return;
        if(fscanf(ptr,"%d %d",&Init.SWType,&Init.SWID) == EOF) return;
        if(fscanf(ptr,"%s",Init.StartTrf) == EOF) return;
        fclose(ptr);
}

writeData()
{
	FILE *ptr;

	ptr = fopen("ftam.dat","w");
	fprintf(ptr,"%s\n",Init.TrfPath);
	fprintf(ptr,"%s\n",Init.AmaPath);
	fprintf(ptr,"%d\n",Init.StartAma);
	fprintf(ptr,"%d\n",Init.StartBlock);
	fprintf(ptr,"%d %d\n",Init.SWType,Init.SWID);
	fprintf(ptr,"%s\n",Init.StartTrf);
	fclose(ptr);
}

int getTrf(char path[],char fname[])
{
	char buff[255];

	if(process_fcp("ftam2",fname) != 0) return(-1);
/*      trance Traffic Data */
	sprintf(buff,"mv %s %s/ready/%s",fname,path,fname);
	system(buff);
	return(0);
}

int getAma(char path[],int *no)
{
	char buff[255];
	char fname[255];
	int  No=Init.StartBlock;

	sprintf(fname,"%04d",*no);
	outputMsg("Ama    start");

        if(process_fcp("ftam1",fname) != 0) return(-1);
/*   trance Ama File */
	outputMsg("Ama     end trance start");
	TranceAma(fname,".",Init,&No,0);
        sprintf(buff,"mv %s %s/ready/%s",fname,path,fname);
        system(buff);
	Init.StartBlock = No;
	NextAma(no);
	outputMsg("Ama     trance End");
	writeData();
        return(0);
}

NextAma(int *no)
{
	*no++;
	if(*no > 9999) *no=1;
}
	
int process_fcp(char path[],char fname[])
{
        char buff[255];
        int err_no=0;

        sprintf(buff,"fcp_ -T3 -v %s! \"%s;state=ready\" > err 2>&1 ",path,fname);
        err_no = system(buff);
        return(err_no);
}

