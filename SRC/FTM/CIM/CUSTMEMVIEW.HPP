#ifndef CUSTMEMVIEW_HPP
#define CUSTMEMVIEW_HPP

#include <CustMdbMan.hpp>
#include <iostream.h>
#include <iomanip.h>

char 	PATH_NAME_CUSTOMER[80] 	= "/SVC#/DATA/SubsShm.cfg";
class CCustMemView
{
	public:
		CCustMemView();
		CCustMemView(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
		~CCustMemView();
		void					 CustMemView( void );
	private:
	
		CCustMdbMan*			 cCustMdbMan;
		CCustomerBtree*        	 pCustomerBtree;
		_BTREENODE*				 pBtreeNode;
		int						 _CustMemView( _BTREENODE* pBtreeNode );
		void					 PrintCustFormatMdb( ostream& output, CustFormatMdb* pCustFormatMdb );
		int 					 ConversionData( CustFormatMdb* pCustFormatMdb, CustFormat stCustFormat );	
		CustFormatMdb* 			 PtrCustMdbData;

};
CCustMemView::CCustMemView()
{
}
CCustMemView::CCustMemView(int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark )
{
//	char   cPath[30];
	char   cId='A';
	
	sprintf( PATH_NAME_CUSTOMER, "/SVC%d/DATA/SubsShm.cfg",nServiceId );
//	memcpy(cPath, "/SVC1/DATA/SubsShm.cfg", 30);
	cCustMdbMan = new CCustMdbMan( PATH_NAME_CUSTOMER, cId, nSystemId,nServiceId,nGroupId,nProcessType,nPserialNo,nPreMark );
	
	pCustomerBtree = cCustMdbMan->get_CustBtreePointer();
}
CCustMemView::~CCustMemView()
{
	delete cCustMdbMan;
}
// 외부에서 Observer를 호출하는 Method.
// 
void CCustMemView::CustMemView( void )
{
	// Btree의 Root Pointer를 얻는다.
	pBtreeNode=pCustomerBtree->getBtreeRoot();
	// Observer를 실행하는 재귀함수.
	if( _CustMemView( pBtreeNode ) )
	{
		cout << "MemView : _MemView Error!!! " << endl;
		exit( 1 );
	}
	
}
// 실제 MemView를 실행하는 재귀함수.
// return value : 0 -> 성공적인 종료
//                -1 -> 실패
int CCustMemView::_CustMemView( _BTREENODE* pBtreeNode )
{
	int nTotalCount=0;
	
	// Btree Node가 Null이 아니면.
	if( pBtreeNode )
	{
		// Btree Node의 key를 차례로 검색한다.
		for( int i=0; i<pBtreeNode->iNumberKey; i++ )
		{
			
			PtrCustMdbData = pBtreeNode->kIndexKey[i].pCustFormatMdb;
			PrintCustFormatMdb( cout, PtrCustMdbData );
			nTotalCount++;
		}
		// 다음 자식 Node를 호출하여
		for( i=0; i<=pBtreeNode->iNumberKey; i++ )
		{
			// 다시 Observer를 재귀호출한다.
			_CustMemView( pBtreeNode->pBTREENODE[i] );
		}
	}
	cout.flush();
	cout << "Total Data Count ==>" << nTotalCount << endl;
	return 0;
}
void CCustMemView::PrintCustFormatMdb( ostream& output, CustFormatMdb* pCustFormatMdb )
{
	output << "==========================================" << endl;
	output << setiosflags( ios::left) 
		   << setw( 11) << "MsID"  
		   << setw( 11) << "EventDate"
		   << setw( 11) << "EventTime"
		   << setw( 11) << "ServiceType"
		   << setw( 11) << "PricePlan"
		   << setw( 11) << "CustStatus" << '\n';
	output << setiosflags( ios::left) 
		   << setw( 11) << pCustFormatMdb->get_MsID()
		   << setw( 11) << pCustFormatMdb->get_EventDate()
		   << setw( 11) << pCustFormatMdb->get_EventTime()
		   << setw( 11) << pCustFormatMdb->get_ServiceType()
		   << setw( 11) << pCustFormatMdb->get_PricePlan()
		   << setw( 11) << pCustFormatMdb->get_CustStatus() << '\n';
   	output << "==========================================" << endl;
}
#endif

