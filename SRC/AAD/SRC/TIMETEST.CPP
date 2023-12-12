 /* ---------------------------------------------------------
 *  Name     : Duration
 *  Argument :  dest     -> �ð��� ������ string
 *                      fromtime   -> ���۽ð�("200010251232201"(��:4,��:2,��:2,��:2,��:2,��:1,1/10��:1)�� ���·� �Է��Ͽ��� �Ѵ�)
 *                      totime -> ���ð�("200010251232202"(��:4,��:2,��:2,��:2,��:2,��:1,1/10��:1)�� ���·� �Է��Ͽ��� �Ѵ�)
 *  Return   : void
 *  Purpose  : �Է� ����fromtime��  totime�� �ð����� 1/10�ʱ��� ����� dest�� �Ѱ� �ش�.
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