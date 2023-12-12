/****************************************************************************/
/* 파 일 명 : Hexdemp.c														*/
/* 작 성 자 : 																*/
/* 기    능 :																*/
/* 작성일자 : 																*/
/* 변경일자 : 																*/
/* 변경내용 :																*/
/* 관련파일 : 																*/
/* 기    타 :																*/
/****************************************************************************/
#include <stdio.h>
#include <iostream.h>

/****************************************************************************/
/* Function명  : void InitBuf(char *Buf,long Cnt)							*/
/* Input Value : char *Buf													*/
/*				 long Cnt													*/
/* Output Valeu:  															*/
/* 기       능 : HexDump시 offset를 설정한다.								*/
/* 기       타 :															*/
/****************************************************************************/
void InitBuf(char *Buf,long Cnt)
{
    sprintf((char *)Buf, "%80s", "");
    sprintf((char *)Buf, "%04X:%04X", (int)((Cnt&0xFF00)>>8), (int)(Cnt&0x00FF));
    Buf[9] = ' ';
}

/****************************************************************************/
/* Function명  : void HexDump(char *Buf,long BufLen)						*/
/* Input Value : char *Buf		Dump할 데이타버퍼							*/
/*				 long BufLen	Dump할 데이타크기							*/
/* Output Valeu:															*/
/* 기       능 : 한Byte 한Byte를 Hex값으로 변환하여 보여준다.				*/
/* 기       타 :															*/
/****************************************************************************/
void HexDump(char *Buf,long BufLen)
{
    long BufCnt = 0;
    char DispBuf[100];
    int  Cnt = 1;
    
    InitBuf(DispBuf, BufCnt);
    for(BufCnt = 0L; BufCnt < BufLen; BufCnt++){
        sprintf((char *)&DispBuf[Cnt*3+9], "%02X", (unsigned char)Buf[BufCnt]);
        DispBuf[Cnt*3+9+2] = ' ';
        if(Buf[BufCnt]<0x20)sprintf(&DispBuf[Cnt+61], "%c", '.');
        else sprintf(&DispBuf[Cnt+61], "%c", Buf[BufCnt]);
        if(Cnt%16 == 0){
            Cnt = 0;
            printf("%-79.79s\r\n", DispBuf);
            InitBuf(DispBuf, BufCnt+1);
        }
        Cnt++;
    }
    printf("%-79.79s\r\n", DispBuf);
}




/****************************************************************************/
/* Function명  : void HexDump(char *RBuf, char *Buf,long BufLen)			*/
/* Input Value : char *RBuf     Dump내용을 얻어올 버퍼						*/
/*				 char *Buf		Dump할 데이타버퍼							*/
/*				 long BufLen	Dump할 데이타크기							*/
/* Output Valeu:															*/
/* 기       능 : 한Byte 한Byte를 Hex값으로 변환하여 보여준다.				*/
/* 기       타 : RBuf 크기는 BufLen*5 이상되여야한다.						*/
/****************************************************************************/
void GetHexDump(char *RBuf, const char *Buf,long BufLen)
{
    long 	BufCnt = 0;
    char 	DispBuf[100];
    int  	Cnt = 1;
    int		Rn=0;
    
    InitBuf(DispBuf, BufCnt);
    for(BufCnt = 0L; BufCnt < BufLen; BufCnt++){
        sprintf((char *)&DispBuf[Cnt*3+9], "%02X", (unsigned char)Buf[BufCnt]);
        DispBuf[Cnt*3+9+2] = ' ';
        if(Buf[BufCnt]<0x20)sprintf(&DispBuf[Cnt+61], "%c", '.');
        else sprintf(&DispBuf[Cnt+61], "%c", Buf[BufCnt]);
        if(Cnt%16 == 0){
            Cnt = 0;
            //printf("%-79.79s\r\n", DispBuf);
            sprintf((char *)&RBuf[Rn*81], "%-79.79s\r\n",  DispBuf);
            InitBuf(DispBuf, BufCnt+1);
            Rn++;
        }
        Cnt++;
    }
    //printf("%-79.79s\r\n", DispBuf);
    sprintf((char *)&RBuf[Rn*81], "%-79.79s\r\n",  DispBuf);
    Rn++;
    RBuf[Rn*81]=0;
}



int main(int argc, char *argv[])
{
	
	if(argc<2)
	{
		printf ("\n[USE] : hexdump <filename> \n");
		return 0;
	}
	
	int		len, tlen=0;
	int		select;
	char	fBuf[1024];
	FILE	*fp;
	
		
	fp = fopen(argv[1], "r+b");
	if(!fp)
	{
		printf("%s Open Fail !! \n", argv[1]);
		return 0;
	}
	
	
	while((len=fread(fBuf, 1, 512, fp)))
	{
		tlen+=len;
		HexDump(fBuf, len);
		printf("\n[%d]---------------------(plase <Enter> key)--------(stop : Ctrl+C)------ \n", tlen);
		getchar();
		//cin >> select;
	}

	fclose(fp);
	return 1;
}

