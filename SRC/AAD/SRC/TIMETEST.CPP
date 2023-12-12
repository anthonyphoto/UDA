 /* ---------------------------------------------------------
 *  Name     : Duration
 *  Argument :  dest     -> 시간을 저장할 string
 *                      fromtime   -> 시작시간("200010251232201"(년:4,월:2,일:2,시:2,분:2,초:1,1/10초:1)의 형태로 입력하여야 한다)
 *                      totime -> 끝시간("200010251232202"(년:4,월:2,일:2,시:2,분:2,초:1,1/10초:1)의 형태로 입력하여야 한다)
 *  Return   : void
 *  Purpose  : 입력 받은fromtime과  totime의 시간차를 1/10초까지 계산후 dest에 넘겨 준다.
 * --------------------------------------------------------- */
void CTime::Duration ( char *dest, char *fromtime, char *totime )
{
        char    strFrom[15];
        char    strTo[15];
        int     nFrom = 0;
        int     nTo = 0;
        int     nDur = 0;
        time_t  fromT, toT, duration;

        memset (strFrom, 0x00, 15);
        memset (strTo, 0x00, 15);

        strncpy(strFrom,fromtime,14);
        strncpy(strTo,totime,14);

        nFrom = atoi(&fromtime[14]);
        nTo = atoi(&totime[14]);

        fromT   = MakeTm(fromtp,strFrom);
        toT     = MakeTm(totp,strTo);

        duration = toT - fromT;
        nDur     = nTo - nFrom;

        if(nDur<0)
        {
                duration = duration -1;
                nDur = 10 + nDur;
        }
        sprintf (dest, "%d%d", duration, nDur);
}
