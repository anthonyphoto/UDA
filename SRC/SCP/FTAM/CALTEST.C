#include <stdio.h>

struct trance_Type {
                int HID;
                int LDC;
                int CCE;
                int TM;
                int OTG;
                int ITG;
                };
struct CAL_Type {
        int Trunk;
        int HID[2];
        int LDC[4];
        int CCE[5];
        int TM[9];
        };
struct CAL_Type InData[200];
struct CAL_Type OutData[200];
int InCount=0;
int OutCount=0;

main(int argc, char *argv[])
{
	struct CAL_Type In,Out ;
	int i,j,total;

        for(j=0;j<2;j++) In.HID[j] = 0; 
        for(j=0;j<4;j++) In.LDC[j] = 0; 
        for(j=0;j<5;j++) In.CCE[j] = 0; 
        for(j=0;j<9;j++) In.TM[j] = 0; 
        for(j=0;j<2;j++) Out.HID[j] = 0; 
        for(j=0;j<4;j++) Out.LDC[j] = 0; 
        for(j=0;j<5;j++) Out.CCE[j] = 0; 
        for(j=0;j<9;j++) Out.TM[j] = 0; 
	read_CALData(argv[1]);
	for(i=0;i<InCount;i++) {
		for(j=0;j<2;j++) In.HID[j] += InData[i].HID[j];
		for(j=0;j<4;j++) In.LDC[j] += InData[i].LDC[j];
		for(j=0;j<5;j++) In.CCE[j] += InData[i].CCE[j];
		for(j=0;j<9;j++) In.TM[j]  += InData[i].TM[j];
	}
        for(i=0;i<OutCount;i++) {
                for(j=0;j<2;j++) Out.HID[j] += OutData[i].HID[j];
                for(j=0;j<4;j++) Out.LDC[j] += OutData[i].LDC[j];
                for(j=0;j<5;j++) Out.CCE[j] += OutData[i].CCE[j];
                for(j=0;j<9;j++) Out.TM[j]  += OutData[i].TM[j];
        }
	printf(" In Trunk Count %d \n", InCount);
	total=0;
	for(i=0;i<2;i++) total += In.HID[i];
	printf("%10d %10d %10d \n",total,In.HID[0],In.HID[1]);

        total=0;
        for(i=0;i<4;i++) total += In.LDC[i];
        printf("%10d ",total);
	for(i=0;i<4;i++) printf("%10d ",In.LDC[i]);printf("\n");

        total=0;
        for(i=0;i<5;i++) total += In.CCE[i];
        printf("%10d ",total);
        for(i=0;i<5;i++) printf("%10d ",In.CCE[i]);printf("\n");

        total=0;
        for(i=0;i<9;i++) total += In.TM[i];
        printf("%10d ",total);
        for(i=0;i<9;i++) printf("%10d ",In.TM[i]);printf("\n");

        printf(" Out Trunk Count %d \n",OutCount);
        total=0;
        for(i=0;i<2;i++) total += Out.HID[i];
        printf("%10d %10d %10d \n",total,Out.HID[0],Out.HID[1]);

        total=0;
        for(i=0;i<4;i++) total += Out.LDC[i];
        printf("%10d ",total);
        for(i=0;i<4;i++) printf("%10d ",Out.LDC[i]);printf("\n");

        total=0;
        for(i=0;i<5;i++) total += Out.CCE[i];
        printf("%10d ",total);
        for(i=0;i<5;i++) printf("%10d ",Out.CCE[i]);printf("\n");

        total=0;
        for(i=0;i<9;i++) total += Out.TM[i];
        printf("%10d ",total);
        for(i=0;i<9;i++) printf("%10d ",Out.TM[i]);printf("\n");

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

