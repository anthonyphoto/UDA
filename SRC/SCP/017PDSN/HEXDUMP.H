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
#ifndef _HEXDUMP_H
#define	_HEXDUMP_H

#include <stdio.h>

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

#endif

