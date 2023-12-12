#include <stdio.h>

class Header
{
	private:
		int		nMsgType;		//  Message Type
		int		nSystemId;		//	SYSTEM ID
		int		nSvcId;			//	SERVICE ID
		int		nGrpType;		//	GROUP ID
		int		nProcType;		//	PROCESS TYPE
		int		nProcSerial;	//	PROCESS SERIAL NUMBER
		int		nProcRemark;	//	PROCESS REMARK
		
	public:
		Header(){;}
		Header	(	int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
						)
		{
			MsgType=MsgType;
			SystemId=SystemId;
			SvcId=SvcId;
			GrpType=GrpType;
			ProcType=ProcType;
			ProcSerial=ProcSerial;
			nProcRemark=ProcRemark;
			printf("[Header] %d\n", nProcRemark);
			
		}
		int 	get_nProcRemark()	{	
			printf("[get_nProcRemark] %d\n", nProcRemark);
			return   nProcRemark;	}
		
		void	set_Header(int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
							)
		{
			printf("[set_Header] %d\n", ProcRemark);
			Header(	MsgType,		
							SystemId,		
							SvcId,			
							GrpType,		
							ProcType,		
							ProcSerial,		
							ProcRemark);	
			nProcRemark=ProcRemark	;
		}
							
	
	
};


class	THeader : public Header
{
	private:
		
		//char	strDate[LEN_DATE+1];
		//char	strTime[LEN_TIME+1];

	public:
		THeader ( ){;}
		~THeader( ){;}
		THeader	(	int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
				): Header ( MsgType,		
							SystemId,		
							SvcId,			
							GrpType,		
							ProcType,		
							ProcSerial,		
							ProcRemark		
						
						)
		{
			;			
		}
						
		//int 	get_nMsgType()		{	return   nMsgType	;	}
		//int 	get_nSystemId()		{	return   nSystemId	;	}
		//int 	get_nSvcId()		{	return   nSvcId		;	}
		//int 	get_nGrpType()		{	return   nGrpType	;	}
		//int 	get_nProcType()		{	return   nProcType	;	}
		//int 	get_nProcSerial()	{	return   nProcSerial;	}
		
		void	set_THeader(int		MsgType,		//  Message Type
							int		SystemId,		//	SYSTEM ID
							int		SvcId,			//	SERVICE ID
							int		GrpType,		//	GROUP ID
							int		ProcType,		//	PROCESS TYPE
							int		ProcSerial,		//	PROCESS SERIAL NUMBER
							int		ProcRemark		//	PROCESS REMARK
							) 
		{
			set_Header(	MsgType,		
							SystemId,		
							SvcId,			
							GrpType,		
							ProcType,		
							ProcSerial,		
							ProcRemark);		
		}
		//int 	get_nProcRemark()	{	return   nProcRemark;	}
};


int main(void)

{
	/*
	THeader *head[100];
	
	
	head[0] = new THeader(1,1,1,1,1,1,5);
	
	printf("\n %d \n", head[0]->get_nProcRemark());
	
	delete head[0];
	*/
	
	
	THeader *head;
	
	
	head = new THeader[100];
	head[0].set_THeader(1,1,1,1,1,1,5);
	head[10].set_THeader(1,1,1,1,1,1,5);
	
	
	printf("\n %d \n", head[0].get_nProcRemark());
	printf("\n %d \n", head[10].get_nProcRemark());
	
	delete[] head;
	
}
