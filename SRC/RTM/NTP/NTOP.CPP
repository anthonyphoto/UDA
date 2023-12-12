#include "nTop.hpp"

extern	MessageQueue<SCPFormatAgg>	*mqSCP; 
extern	SendLogManage 				*clsLogM; 
extern	FILE  *fd;
extern	SCPFormatAgg 	SCPFormatNTOP;	

extern  char	    PATH_NAME_NTOP_CFG[80];

//==============================================================================
void nTop_FTP::SendNTopFile(int nRecCount)
{		
	STRINGBUF		stStringBuf;

	while(fread((char *)&stStringBuf, sizeof(stStringBuf), 1, fd) > 0)
	{
		put_nUdrCount(++nRecCount);
		
   	 	put_strBill_Phone_Num(&stStringBuf.strXString[POS_BILL_PHON_NUM] );
		put_strAns_Dt		 (&stStringBuf.strXString[POS_ANS_DT]		 );			
		put_strAns_Tm		 (&stStringBuf.strXString[POS_ANS_TM]		 );			
		put_strRelease_Dt	 (&stStringBuf.strXString[POS_RELEASE_DT]	 );		
		put_strRelease_tm	 (&stStringBuf.strXString[POS_RELEASE_TM]	 );		
		put_strCdr_Type		 (&stStringBuf.strXString[POS_CDR_TYPE]		 );			
		put_strPacket_Normal (&stStringBuf.strXString[POS_PACKET_NOMAL]	 );	
		put_strPacket_Discnt (&stStringBuf.strXString[POS_PACKET_DISCNT] );	
		put_strPacket_Night	 (&stStringBuf.strXString[POS_PACKET_NIGHT]	 );		
		put_strTot_Charge_Amt(&stStringBuf.strXString[POS_TOT_CHARGE_AMT]);
/*
		cout << "stNTOPFTP.strBill_Phone_Num : " << strBill_Phone_Num 	<< endl;
		cout << "stNTOPFTP.strAns_Dt 		 : " << strAns_Dt			<< endl;			
		cout << "stNTOPFTP.strAns_Tm 		 : " << strAns_Tm 			<< endl;			
		cout << "stNTOPFTP.strRelease_Dt 	 : " << strRelease_Dt 		<< endl;
		cout << "stNTOPFTP.strRelease_tm 	 : " << strRelease_tm		<< endl;			
		cout << "stNTOPFTP.strCdr_Tpye	     : " << strCdr_Type			<< endl;
		cout << "stNTOPFTP.strTot_Charge_Amt : " << strTot_Charge_Amt	<< endl;			
*/	
		UseTime();
		
		SCPFormatNTOP.get_ItemA2()->set_Key(0x05);								// nTop컨텐츠 사용결과(구분코드)         
		SCPFormatNTOP.get_ItemA2()->set_Min(atol(strBill_Phone_Num));  			// MSID
		SCPFormatNTOP.get_ItemE()->set_ServiceId(1);							// nTop Service = 1
		SCPFormatNTOP.get_ItemE()->set_CgClass(atol(strCdr_Type));				// 1 : 저속, 2 : 고속     	
		SCPFormatNTOP.get_ItemE()->set_PacketN1(atol(strPacket_Normal));		// 비할인 시간에 사용한 패킷 개수    
		SCPFormatNTOP.get_ItemE()->set_PacketN2(atol(strPacket_Discnt));		// 할인 시간에 사용한 패킷 개수      
		SCPFormatNTOP.get_ItemE()->set_PacketN3(atol(strPacket_Night));			// 심야 시간에 사용한 패킷 개수     
		SCPFormatNTOP.get_ItemE()->set_UseMoney(atol(strTot_Charge_Amt));		// 서비스 사용 요금(단위 : 십전)      
                                   

		// SCP Format으로 변환된 Cdr을 SCP MQ로 send    	
	    if (mqSCP->sendMsg(&SCPFormatNTOP) == FLAG_MQ_IS_FULL)
	    {
			////////////////////////////////////////////////////////////////
			*clsLogM 	<< clsLogM->ErrorCode(0) << "=====Send to SCP=====Waiting===== " 	<< endl;
			////////////////////////////////////////////////////////////////
	        sleep(1);                
	        continue;
	    } 

	    write_nTopConfigFile();   
	    ////////////////////////////////////////////////////////////////
		*clsLogM << clsLogM->ErrorCode(0) 	<< "----------------------------" 					<< endl 
				 << "[SpecificKey     ] " 		<< (int)SCPFormatNTOP.get_ItemA2()->get_Key()  	<< endl
				 << "[Min             ] " 		<< SCPFormatNTOP.get_ItemA2()->get_Min() 	 	<< endl
				 << "[Start Time      ] " 		<< SCPFormatNTOP.get_ItemC()->get_StartT() 		<< endl
				 << "[End   Time      ] " 		<< SCPFormatNTOP.get_ItemC()->get_EndT()   		<< endl
				 << "[ServiceIndicator] "		<< SCPFormatNTOP.get_ItemE()->get_ServiceId()	<< endl
				 << "[ChargintClass   ] " 		<< SCPFormatNTOP.get_ItemE()->get_CgClass() 	<< endl
				 << "[비할인Packet수  ] " 		<< SCPFormatNTOP.get_ItemE()->get_PacketN1()	<< endl
				 << "[할인  Packet수  ] " 		<< SCPFormatNTOP.get_ItemE()->get_PacketN2()	<< endl				 
				 << "[심야  Packet수  ] " 		<< SCPFormatNTOP.get_ItemE()->get_PacketN3()	<< endl				 
				 << "[Use Money       ] " 		<< SCPFormatNTOP.get_ItemE()->get_UseMoney()	<< endl				 
				 << "----------------------------" 												<< endl; 
		////////////////////////////////////////////////////////////////
                  
	    memset((char *)&stStringBuf, 0x00, sizeof(stStringBuf));
	}  // end innor while  
	fclose(fd);
}


//==============================================================================
void nTop_FTP::UseTime()
{
	struct  tm 	*Tm;
	time_t		today;
	time_t		onet, twot;
	
	char stryear[4];
	char strmon[3];
	char strday[3];
	char strhour[3];
	char strmin[3];
	char strsec[3];

	memset(stryear, 0x00, sizeof(stryear));
	memset(strmon , 0x00, sizeof(strmon));
	memset(strday , 0x00, sizeof(strday));
	memset(strhour, 0x00, sizeof(strhour));
	memset(strmin , 0x00, sizeof(strmin));
	memset(strsec , 0x00, sizeof(strsec));

	time(&today);	// 오늘날자 얻기  
	Tm = localtime(&today);

	memcpy(stryear, strAns_Dt   , 3);
	memcpy(strmon , strAns_Dt+3 , 2);
	memcpy(strday , strAns_Dt+5 , 2);
	memcpy(strhour, strAns_Tm   , 2);
	memcpy(strmin , strAns_Tm+2 , 2);
	memcpy(strsec , strAns_Tm+4 , 2);

	Tm->tm_year		=	atoi(stryear) + 1800 - 1900;
	Tm->tm_mon		=	atoi(strmon) - 1;
	Tm->tm_mday		=	atoi(strday);
	Tm->tm_hour		=	atoi(strhour);
	Tm->tm_min		=	atoi(strmin);
	Tm->tm_sec		=	atoi(strsec);

	onet = mktime(Tm);

	memcpy(stryear, strRelease_Dt  , 3);
	memcpy(strmon , strRelease_Dt+3, 2);
	memcpy(strday , strRelease_Dt+5, 2);
	memcpy(strhour, strRelease_tm  , 2);
	memcpy(strmin , strRelease_tm+2, 2);
	memcpy(strsec , strRelease_tm+4, 2);

	Tm->tm_year		=	atoi(stryear) + 1800 - 1900;
	Tm->tm_mon		=	atoi(strmon) - 1;
	Tm->tm_mday		=	atoi(strday);
	Tm->tm_hour		=	atoi(strhour);
	Tm->tm_min		=	atoi(strmin);
	Tm->tm_sec		=	atoi(strsec);

	twot = mktime(Tm);	
	
	SCPFormatNTOP.get_ItemC()->set_StartT(onet);	// 서비스 사용 시작 시각         
	SCPFormatNTOP.get_ItemC()->set_EndT(twot);		// 서비스 사용 종료 시각	    							   		
}


//==============================================================================
// /SVC%d/BDS/NB/에 저장되어있는 file을 /BDS/WORK/TEMP/로 Move    
int nTop_FTP::MoveFile(char* strMovePath)
{
	char strMove[MAXLEN];

	memset(strMove, 0x00, MAXLEN);
	sprintf(strMove, "mv %s %s", get_strNTopFileName1(), strMovePath);

    if(system(strMove) == -1) 		return -1;

    memset(strMove, 0x00, MAXLEN);
	sprintf(strMove, "mv %s %s", get_strNTopFileName2(), strMovePath);

    if(system(strMove) == -1)		return -1;
    
    return 0;
}    


//==============================================================================
void nTopFileConfig::read_nTopConfigFile()
{
	ifstream	ConfigFile(PATH_NAME_NTOP_CFG, ios::binary);

	ConfigFile.read((char*)this, sizeof(nTopFileConfig));
	ConfigFile.close();
}


//==============================================================================
void nTopFileConfig::write_nTopConfigFile()
{
	ofstream	ConfigFile(PATH_NAME_NTOP_CFG, ios::binary);

	ConfigFile.write((char*)this, sizeof(nTopFileConfig));
	ConfigFile.close();
}
