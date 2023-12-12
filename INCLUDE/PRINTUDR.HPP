/*****************************************
* UDR의 내용을 출력하기 위한 Header      *
* writer : Han Guen Hee                  *
* file name : PrintUdr.hpp               *
* program history                        *
*                                		 *
* bug fix                        	     *
*                                  	     *
* last updata : 2000. 9. 29        		 *
*****************************************/
#ifndef PRINTUDR_HPP
#define PRINTUDR_HPP

#include<iostream.h>

class CPrintUdr 
{
	public:
		CPrintUdr();
		~CPrintUdr();
		void 					Print( UdrFormat* pUdrFormat );
	
	private:
		// print method
		void outputLineHEAD( ostream& output, UDRHEAD_TYPE* stHeadType );
		void outputLineA( ostream& output, A_INT_TYPE* stAtype );
		void outputLineB( ostream& output, B_TYPE* stBtype );
		void outputLineC( ostream& output, C_TYPE* stCtype );
		void outputLineD( ostream& output, D_TYPE* stDtype );
		void outputLineE( ostream& output, E_TYPE* stEtype );
		void outputLineF( ostream& output, F_TYPE* stFtype );
		void outputLineG( ostream& output, G_TYPE* stGtype );
		void outputLineH( ostream& output, H_TYPE* stHtype );
		void outputLineI( ostream& output, I_TYPE* stItype );
		void outputLineY( ostream& output, Y_TYPE* stYtype );
		void outputLineZ( ostream& output, Z_TYPE* stZtype );
};
// Constructor
CPrintUdr::CPrintUdr() 
{
}
// destructor
CPrintUdr::~CPrintUdr()
{
}
// Print Method.
// 외부에서 호출하여 UDR의 내용을 출력하는 메소드.
void CPrintUdr::Print( UdrFormat *stDataNode )
{
	// 각각의 구조체의 값을 얻어오기 위한 포인터 변수.
	UDRHEAD_TYPE*		stHeadType;
	A_INT_TYPE*			stAtype;
	B_TYPE*				stBtype;
	C_TYPE*				stCtype;
	D_TYPE*				stDtype;
	E_TYPE*				stEtype;
	F_TYPE*				stFtype;
	G_TYPE*				stGtype;
	H_TYPE*				stHtype;
	I_TYPE*				stItype;
	Y_TYPE*				stYtype;
	Z_TYPE*				stZtype;

		// 각 구조체를 가져온다.
		stHeadType=stDataNode->get_ItemHead();
		stAtype=stDataNode->get_ItemA();
		stBtype=stDataNode->get_ItemB();
		stCtype=stDataNode->get_ItemC();
		stDtype=stDataNode->get_ItemD();
		stEtype=stDataNode->get_ItemE();
		stFtype=stDataNode->get_ItemF();
		stGtype=stDataNode->get_ItemG();
		stHtype=stDataNode->get_ItemH();
		stItype=stDataNode->get_ItemI();
		stYtype=stDataNode->get_ItemY();
		stZtype=stDataNode->get_ItemZ();

		// 각각의 구조체를 출력하는 Method를 호출한다.
		/*
		cout << "start print-----------------------------------------------------------------------------------" << endl;
		cout << "******* HEAD_TYPE *******" << endl;
		outputLineHEAD( cout, stHeadType);
		cout << "******* A_INT_TYPE *******" << endl;
		outputLineA( cout, stAtype);
		cout << "******* B_TYPE *******" << endl;
		outputLineB( cout, stBtype);
		cout << "******* C_TYPE *******" << endl;
		outputLineC( cout, stCtype);
		cout << "******* D_TYPE *******" << endl;
		outputLineD( cout, stDtype);
		cout << "******* E_TYPE *******" << endl;
		outputLineE( cout, stEtype);
		cout << "******* F_TYPE *******" << endl;
		outputLineF( cout, stFtype);
		cout << "******* G_TYPE *******" << endl;
		outputLineG( cout, stGtype);
		cout << "******* H_TYPE *******" << endl;
		outputLineH( cout, stHtype);
		cout << "******* I_TYPE *******" << endl;
		outputLineI( cout, stItype);
		cout << "******* Y_TYPE *******" << endl;
		outputLineY( cout, stYtype);
		cout << "******* Z_TYPE *******" << endl;
		outputLineZ( cout, stZtype);
		cout << "end print**************************************************************************************" << endl;
*/

		 //cout << "<GET UDR>----------------------------------------------------------------------------" << endl;
		 cout << "[UDR TYPE]:" << stHeadType->get_nUdrType() << " " << "[File Number]:" << stHeadType->get_nUdrFileSeq() << " " << "[UDR SEQ NUM]:" << stHeadType->get_nUdrSeqNo() << endl;
		 cout << "[MIN]:"<<stAtype->get_unMsId()<<endl;
		 cout << "[ACCOUNT SESSION ID]:"<<stCtype->get_strAccountSessionID() <<endl;
		 cout << "[CORRELATION ID]:"<<stCtype->get_strCorrelationID()<<endl;
		 /*
		 cout << "[START EVENT TIME]:"<<stGtype->get_nStartEventTime()<<endl;
		 cout << "[END EVENT TIME]:"<<stGtype->get_nEndEventTime()<<endl;
		 cout << "[ACTIVE TIME]:"<<stGtype->get_nActiveTime()<<endl;
		 */
}
void CPrintUdr::outputLineHEAD( ostream& output, UDRHEAD_TYPE* stHeadType)
{
	output << setiosflags( ios::left) << setw( 8 ) << "nUdrType:" << stHeadType->get_nUdrType() << " " 
		<< setw( 8 ) << "nServiceType:" << stHeadType->get_nServiceType() << " " << setw( 8 )  
		<< "nSubsType:" << stHeadType->get_nSubsType() << " " << setw( 8 ) << "nUdrSeqNo:" << stHeadType->get_nUdrSeqNo() << " " 
		<< setw( 8 ) << "nUdrStatus:" << stHeadType->get_nUdrStatus() << " " << setw( 8 ) << "nServiceProvider:" << stHeadType->get_nServiceProvider() << " " 
		<< setw( 8 ) << "nMin1:" << stHeadType->get_nMin1() << " " << setw( 8 ) << "nMin2:" << stHeadType->get_nMin2() << '\n';
}
void CPrintUdr::outputLineA( ostream& output, A_INT_TYPE* stAtype)
{
	output << setiosflags( ios::left) << setw( 12 ) << "unMsId:" << stAtype->get_unMsId() << '\n';
}
void CPrintUdr::outputLineB( ostream& output, B_TYPE* stBtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stBtype->get_nIpAddr() 
		<< setw( 12 )  << stBtype->get_strNAI() << '\n';
}
void CPrintUdr::outputLineC( ostream& output, C_TYPE* stCtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << "strAccountSessionID:" << stCtype->get_strAccountSessionID() << " " 
		<< setw( 12 ) << "strCorrelationID:" << stCtype->get_strCorrelationID() << '\n';
}
void CPrintUdr::outputLineD( ostream& output, D_TYPE* stDtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stDtype->get_nMIP_HomeAgent()
		<< setw( 12 ) << stDtype->get_nPDSN_FA_Addr() 	<< setw( 12 ) << stDtype->get_nServing_PCF() 
		<< setw( 12 ) << stDtype->get_strBS_MSC_ID() 	<< '\n';
}
void CPrintUdr::outputLineE( ostream& output, E_TYPE* stEtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stEtype->get_nUserZone() << '\n';
}
void CPrintUdr::outputLineF( ostream& output, F_TYPE* stFtype )
{
	output << setiosflags( ios::left) << setw( 8 ) << stFtype->get_nForwardMUXOpt() 
		<< setw( 8 ) << stFtype->get_nReverseMUXOpt() 		<< setw( 8 ) << stFtype->get_nForward_Fund_Rate()
		<< setw( 8 ) << stFtype->get_nReverse_Fund_Rate()		<< setw( 8 ) << stFtype->get_nServiceOpt()
		<< setw( 8 ) << stFtype->get_nServiceCorrelation()
		<< setw( 8 ) << stFtype->get_nForwardTrafficType() 	<< setw( 8 ) << stFtype->get_nReverseTrafficType()
		<< setw( 8 ) << stFtype->get_nFundFrameSize()	    << setw( 8 ) << stFtype->get_nForwardFundamentalRC()
		<< setw( 8 ) << stFtype->get_nReverseFundamentalRC() 	<< setw( 8 ) << stFtype->get_nIP_Technology()
		<< setw( 8 ) << stFtype->get_nCompulsoryTunnelInd()	<< setw( 8 ) << stFtype->get_nReleaseIndicator() << '\n';
}
void CPrintUdr::outputLineG( ostream& output, G_TYPE* stGtype )
{
	output << setiosflags( ios::left) << setw( 8 ) << "nDataOctetCntTerimnat:" 
	<< stGtype->get_nDataOctetCntTerminat() << " " << setw( 8 ) << "nDataOctetCntOrigin:" 
	<< stGtype->get_nDataOctetCntOrigin() 	<< " " << setw( 8 ) << "nBadPPPframeCnt:" 
	<< stGtype->get_nBadPPPframeCnt() << " " << '\n';
    output << setiosflags(ios::left ) << setw( 8 ) << "nStartEventTime:" << stGtype->get_nStartEventTime() 
	<< " " << setw( 8 ) << "nEndEventTime:" << stGtype->get_nEndEventTime()	
	<< " " << setw( 8 ) << "nActiveTime:" << stGtype->get_nActiveTime() 			
	<< " " << setw( 8 ) << "nNumOfActiveTransitions:" << stGtype->get_nNumOfActiveTransitions() 
	<< " " << setw( 8 ) << "nSDB_OctetCountTerminat:" << stGtype->get_nSDB_OctetCountTerminat() << '\n'; 
	output << setiosflags(ios::left) << setw( 8 ) << "nSDB_OctetCountOrigin:" 
	<< stGtype->get_nSDB_OctetCountOrigin() << " " << setw( 8 ) << "nNumOfSDBsTerminat:" 
	<< stGtype->get_nNumOfSDBsTerminat() << " " << setw( 8 ) << "nNumofSDBsOrigin:" 
	<< stGtype->get_nNumOfSDBsOrigin() << " " << setw( 8 ) << "nNumOfPPPbytesReceived:" 
	<< stGtype->get_nNumOfPPPbytesReceived() << '\n';
}
void CPrintUdr::outputLineH( ostream& output, H_TYPE* stHtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stHtype->get_nAlternateBillingId() << '\n';
}
void CPrintUdr::outputLineI( ostream& output, I_TYPE* stItype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stItype->get_nIP_QoS()
		<< setw( 8 ) << stItype->get_nInterconnectIP_NetworkProviderID() 
		<< setw( 8 ) << stItype->get_nInterconnectIP_NetworkServiceQoS() << setw( 8 ) << stItype->get_nAirlinkQoS() << '\n';
}
void CPrintUdr::outputLineY( ostream& output, Y_TYPE* stYtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stYtype->get_nAirlinkRecType()
		<< setw( 8 ) << stYtype->get_nRP_SessionID() << setw( 8 ) << stYtype->get_nAirlinkSeqNum()
		<< setw( 8 ) << stYtype->get_nMobileOrigin_TerminatID() << '\n';
}
void CPrintUdr::outputLineZ( ostream& output, Z_TYPE* stZtype )
{
	output << setiosflags( ios::left) << setw( 12 ) << stZtype->get_strContainer() << '\n';
}
#endif



