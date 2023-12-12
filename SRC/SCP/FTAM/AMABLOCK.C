#include "amastruct.h"

struct Type {
	char TrfPath[255];
        char AmaPath[255];
        int  StartAma;
        int  StartBlock;
        int  SWType;
        int  SWID;
        char StartTrf[255];
};

struct CAL_Type {
        int Trunk;
        int HID[2];
        int LDC[4];
        int CCE[5];
        int TM[9];
        };
struct trance_Type {
                int HID;
                int LDC;
                int CCE;
                int TM;
                int OTG;
                int ITG;
                };
struct CAL_Type InData[200];
struct CAL_Type OutData[200];
int InCount=0;
int OutCount=0;

TranceAma(char fname[],char path[],struct Type Head,int *No,int tt)
{
	char buff[100];
	int Start_No=0,End_No=0;

	Calculate(fname,path);
	make_block(fname,path,No,&Start_No,&End_No);  /*   Make temp       */
	make_trance_file(fname,path,Start_No,End_No,Head,tt); /*   Make 8882       */
	sprintf(buff,"%s/%s",path,"temp");            /*  delete Temp file */
	unlink(buff);	
}

make_trance_file(char fname[],char path[],int Start_No,int End_No,struct Type Head,int tt)
{
	char frn[100],fwn[100];
	int  fr,fw;
	int  i=0;
	char buff[2050];
	struct Billing_Head BiH;

	BiH.Service_Code= Head.SWType;
	BiH.Serial_No  = Head.SWID;
	BiH.Data_Type  = tt; 
	BiH.CDR_Size   = 57;
	BiH.Block_Size = 2048;
	BiH.File_No    = atoi(fname);
	BiH.Start_No   = Start_No;
	BiH.End_No     = End_No;
	sprintf(fwn,"%s/%s",path,fname);
	sprintf(frn,"%s/temp",path);

	fr = open(frn,0);
	fw = open(fwn,O_RDWR | O_CREAT | O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

	write(fw,&BiH,sizeof(BiH));
	for(i=Start_No;i<=End_No;i++) {
		read(fr,buff,2048);
		write(fw,buff,2048);
	}
	close(fr);
	close(fw);
}

make_block(char fname[],char path[],int *No,int *S,int *E)
{
	int fr,fw;
	char frn[100],fwn[100];
	char buff[1999];
	unsigned char buff1[1999];
	int len,i;
	int fill_len=0;
	int Start,End;
	struct Block_Head BH;
	unsigned char fill = 0xff;
	
	sprintf(BH.ID,"BD");
	for(i=0;i<199;i++) buff1[i]=fill;

	Start = End = *No;

	sprintf(frn,"%s/%s",path,fname);
	sprintf(fwn,"%s/temp",path);

	fr = open(frn,0);
	fw = open(fwn,O_RDWR | O_CREAT | O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

	while((len = read(fr,buff,1995)) != NULL) {
		BH.CDR = len/57;
		BH.NO = End++;
		fill_len = 2048-12-len;
		/*   Block Header write        */
		write(fw,&BH,sizeof(BH));
		/*   Data Block                */
		write(fw,buff,len);
		/*   Fill Data                 */
		write(fw,buff1,fill_len);
	}
	close(fr);
	close(fw);
	*No = End;
	End--;
	*S = Start;
	*E = End;

}

Calculate(char fname[],char path[])
{
	int fr;
	struct KRMT_Type d1;
	char buff[255],name[255];
	int old=0,start = 0;
	int i=0;

	printf(" File Name : %s \n",fname);
	fr = open(fname,0);
	while(read(fr,&d1,sizeof(d1)) > 0) {
		sprintf(buff,"%x%02x%02x%02x%02x%02x",d1.CI[0],d1.CI[1],d1.CI[2],d1.CI[3],d1.CI[4],d1.CI[5]);
		buff[6] = NULL;
		if(old != atoi(buff)) {
			if(start == 1) {
				write_CALData(name);
				start=0;
			}
			strcpy(name,buff);
			read_CALData(name);
		}
		write_cal(d1);
		old = atoi(buff);
	}
	write_CALData(name);
	Close(fr);
}

read_CALData(char name[])
{
	int fr;
	char fname[255];

	InCount=0;
	OutCount=0;
	sprintf(fname,"%s.IN",name);
	fr = open(fname,0);
	if(fr != NULL) {
		while(read(fr,&InData[InCount++],sizeof(struct CAL_Type)) > 0);
		close(fr);
	}
        sprintf(fname,"%s.OUT",name);
        fr = open(fname,0);
	if(fr != NULL) {
	        while(read(fr,&OutData[OutCount++],sizeof(struct CAL_Type)) > 0);
       		close(fr);
	}

}

write_CALData(char name[])
{
	int fw,i;
	char fname[255];

	sprintf(fname,"%s.IN",name);
        fw = open(fname,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	for(i=0;i<InCount;i++)
		write(fw,&InData[i],sizeof(struct CAL_Type));
	close(fw);
        sprintf(fname,"%s.OUT",name);
        fw = open(fname,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
        for(i=0;i<OutCount;i++)
                write(fw,&OutData[i],sizeof(struct CAL_Type));
        close(fw);
}

write_cal(struct KRMT_Type dd)
{
	struct trance_Type tt;
	int flag = -1,i=0;

	trance_data(dd,&tt);
	for(i=0;i<InCount;i++) { 
		if(InData[i].Trunk == tt.ITG) {
			flag = i;
			i = InCount+100;
		} 
	}
	if(flag != -1){
		InData[flag].HID[tt.HID]++;
		InData[flag].LDC[tt.LDC]++;
		InData[flag].CCE[tt.CCE]++;
		InData[flag].TM[tt.TM]++;
	}
	else {
		InData[InCount].Trunk = tt.ITG;
		for(i=0;i<2;i++) InData[InCount].HID[i]=0;
		for(i=0;i<4;i++) InData[InCount].LDC[i]=0;
		for(i=0;i<5;i++) InData[InCount].CCE[i]=0;
		for(i=0;i<9;i++) InData[InCount].TM[i]=0;
                InData[InCount].HID[tt.HID]==1;     
                InData[InCount].LDC[tt.LDC]==1;    
                InData[InCount].CCE[tt.CCE]==1;    
                InData[InCount].TM[tt.TM]==1;      
		InCount++;
	}
	flag = -1;
        for(i=0;i<OutCount;i++) {
                if(OutData[i].Trunk == tt.OTG) {
                        flag = i;
                        i = OutCount+100;
                }
        }
        if(flag != -1){
                OutData[flag].HID[tt.HID]++;
                OutData[flag].LDC[tt.LDC]++;
                OutData[flag].CCE[tt.CCE]++;
                OutData[flag].TM[tt.TM]++;
        }
        else {
		OutData[OutCount].Trunk = tt.OTG;
                for(i=0;i<2;i++) OutData[OutCount].HID[i]=0;
                for(i=0;i<4;i++) OutData[OutCount].LDC[i]=0;
                for(i=0;i<5;i++) OutData[OutCount].CCE[i]=0;
                for(i=0;i<9;i++) OutData[OutCount].TM[i]=0;
                OutData[OutCount].HID[tt.HID]==1;
                OutData[OutCount].LDC[tt.LDC]==1;
                OutData[OutCount].CCE[tt.CCE]==1;
                OutData[OutCount].TM[tt.TM]==1;
                OutCount++;
        }
}

trance_data(struct KRMT_Type ss,struct trance_Type *dd)
{
	char buff[255];
	
	if(ss.HID == 0xaa) dd->HID=0;
	else if(ss.HID == 0xab) dd->HID=1;

	sprintf(buff,"%02x",ss.LCD);
	dd->LDC = atoi(buff);
	
	sprintf(buff,"%02x",ss.CCE);
	dd->CCE = atoi(buff);

	sprintf(buff,"%02x",ss.TM);
	dd->TM = atoi(buff);

	sprintf(buff,"%02x%02x",ss.ITG[0],ss.ITG[1]);
	dd->ITG = atoi(buff);

	sprintf(buff,"%02x%02x",ss.OTG[0],ss.OTG[1]);
	dd->OTG = atoi(buff);
}
