/****************************************************************************/
/* �� �� �� : CustMdbMan.hpp		    									*/
/* �� �� �� : KSI / �ѱ���								                    */
/* ��    �� : Customer Memory DB Operation                            		*/
/* �ۼ����� : 2000�� 11�� 7��                                               */
/* �������� :     ��   ��   ��                                              */
/* ����ȭ�� : 																*/
/* ��    Ÿ :                                                               */
/****************************************************************************/
#ifndef CUSTMDBMAN_HPP
#define CUSTMDBMAN_HPP

#include <fstream.h>
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include <sys/unistd.h>

#include <MDB/CustomerBtree.hpp>

//#include <scpformat.hpp>

const   int					MIN_LEN = 10;
const 	int					JOBCODE_LEN = 2;
const   int					ACCINFO_LEN = 2;
const   int					CUSTMDB_DEBUG=1;
const   int                 SIZE_CUSTFORMATMDB  = sizeof( CustFormatMdb );
		char 				PATH_NAME_CUSTOMER_MDB[80] 	= "/SVC#/MB/SUBS.mb";

class CCustMdbMan
{
	public:
								CCustMdbMan();
								CCustMdbMan(char *Data_Name_Shm, char Data_Id_Shm,int nSystemId, int nServiceId, int nGroupId, int nProcessType, int nPserialNo, int nPreMark );
								~CCustMdbMan();
		int           			InsertCustMdb( CustFormat stCustFormat );
		int						UploadDbToMdb( CustFormatMdb stCustFormatMdb );
		int						SearchMdb( int nSearchMin );
		int           			DeleteCustMdb( CustFormat stCustFormat );
		int						UpdateCustMdb( CustFormat stCustFormat );
		CCustomerBtree*			get_CustBtreePointer() { return pCustomerBtree; }
	private:
		CustFormatMdb 				cCustFormatMdb;
		CCustomerBtree*         	pCustomerBtree;
		SharedMemory*         		shmData;
		CustFormatMdb* 				stPtrDataIndexInfo;
		HandleOfAllocIndex_New*     pAllocDataIndexC;
		fstream         			fInOut;
		void 						PrintCustFormatMdb( ostream& output, CustFormatMdb* pCustFormatMdb );
		int 						ConversionData( CustFormatMdb* pCustFormatMdb, CustFormat stCustFormat );
};

#endif




