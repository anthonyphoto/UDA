/****************************************************************************/
/* �� �� �� : Hexdemp.c														*/
/* �� �� �� : 																*/
/* ��    �� :																*/
/* �ۼ����� : 																*/
/* �������� : 																*/
/* ���泻�� :																*/
/* �������� : 																*/
/* ��    Ÿ :																*/
/****************************************************************************/
#ifndef _HEXDUMP_H
#define	_HEXDUMP_H

#include <stdio.h>

/****************************************************************************/
/* Function��  : void InitBuf(char *Buf,long Cnt)							*/
/* Input Value : char *Buf													*/
/*				 long Cnt													*/
/* Output Valeu:  															*/
/* ��       �� : HexDump�� offset�� �����Ѵ�.								*/
/* ��       Ÿ :															*/
/****************************************************************************/
void InitBuf(char *Buf,long Cnt)
{
    sprintf((char *)Buf, "%80s", "");
    sprintf((char *)Buf, "%04X:%04X", (int)((Cnt&0xFF00)>>8), (int)(Cnt&0x00FF));
    Buf[9] = ' ';
}

/****************************************************************************/
/* Function��  : void HexDump(char *Buf,long BufLen)						*/
/* Input Value : char *Buf		Dump�� ����Ÿ����							*/
/*				 long BufLen	Dump�� ����Ÿũ��							*/
/* Output Valeu:															*/
/* ��       �� : ��Byte ��Byte�� Hex������ ��ȯ�Ͽ� �����ش�.				*/
/* ��       Ÿ :															*/
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
