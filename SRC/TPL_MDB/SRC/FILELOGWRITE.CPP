#include "FileLogWrite.hpp"

#define	NO_LOG	0

//-----------------------------------------------------------------------------------
FileLogWrite::FileLogWrite(const char *path, const char *hname)	// changed by ky. 2001.02. 07
{
	int i=strlen(hname);
	
	memset(Hname, 0x00, sizeof(Hname));
	if(i<20) strcpy(Hname, hname);
	else memcpy(Hname, hname, 19);
	
	strcpy(Path, path);
}


//-----------------------------------------------------------------------------------
void FileLogWrite::Write(const char *format, ...)
{
	#if NO_LOG
		return;
	#endif
	
	char	fname[150];
	va_list	arg;

	//memset(strLog, 0x00, sizeof(strLog));
	
	time(&Time);
	memcpy(&Tm,localtime(&Time),sizeof(Tm));
	sprintf(strLog,"[%04d%02d%02d %02d:%02d:%02d] ",
		Tm.tm_year+1900,Tm.tm_mon+1,Tm.tm_mday,Tm.tm_hour,Tm.tm_min,Tm.tm_sec);
		
	va_start(arg,format);
    vsprintf(&strLog[20], format, arg);
	va_end(arg);

	sprintf(fname, "%s/%s.%04d%02d%02d", Path, Hname, Tm.tm_year+1900,Tm.tm_mon+1,Tm.tm_mday);
	fp = fopen(fname,"a+t");
	if(fp == NULL) return;
	
	fprintf(fp,"%s \n",strLog);
	fclose(fp);
}



//--------------------------------------------------------------------------------
// 기       능 : HexDump시 offset를 설정한다.
void FileLogWrite::InitBuf(char *Buf,long Cnt)
{
    sprintf((char *)Buf, "%80s", "");
    sprintf((char *)Buf, "%04X:%04X", (int)((Cnt&0xFF00)>>8), (int)(Cnt&0x00FF));
    Buf[9] = ' ';
}



//--------------------------------------------------------------------------------
// 기       능 : 한Byte 한Byte를 Hex값으로 변환하여 보여준다.
// 기       타 : RBuf 크기는 BufLen*5 이상되여야한다.
void FileLogWrite::GetHexDump(char *RBuf, const char *Buf,long BufLen)
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



//--------------------------------------------------------------------------------
// 기       능 : HexDump를 로그 화일에 출력한다.
int FileLogWrite::WriteHexLog(char *Buf,	long BufLen)
{
	#if NO_LOG
		return 1;
	#endif
	
	char		strFileName[150];
	char		strDump[8192];

	time(&Time);
	memcpy(&Tm,localtime(&Time),sizeof(Tm));
	
	sprintf(strFileName, "%s/%s.%04d%02d%02d", Path, Hname, Tm.tm_year+1900,Tm.tm_mon+1,Tm.tm_mday);

	fp = fopen(strFileName,"a+t");
	if(fp == NULL) return 0;
	
	if(BufLen > 1600) BufLen = 1600;
	GetHexDump(strDump, Buf, BufLen);
	
	fprintf(fp,"%s", strDump);
	fclose(fp);
	
	return 1;
}

	
