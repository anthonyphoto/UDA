#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Tpl_Btree.hpp>
#include <SendLogManage.hpp>
//#include <FileLogWrite.hpp>
#include <UDR.hpp>
//#include <KeyDef.hpp>


int DisplayMenu(void);
void makeUdr( UdrFormat* pUdrFormat )
{
	pUdrFormat->get_ItemA()->put_unMsId(1198001111);
	pUdrFormat->get_ItemC()->put_strAccountSessionID("1000");
	pUdrFormat->get_ItemC()->put_strCorrelationID("2333");
}

SendLogManage*    LogM;   // 로그매니저 (Main Class에서 정의)

int main( int argc, char *argv[] )
{  
	char 						buffer[50];
    KeyDef						testKey;
    _DATATYPE<KeyDef,UdrFormat> testDataType;
    int							temp;
 //   FileLogWrite    			cFileLogWrite("./", "test");
    char   						cId='A';
    char 						strDataFile[100];
    CBtree<KeyDef,UdrFormat>*	pBtree;
    int							flag = 1;
    UdrFormat 					testUdr;
    int							nSystemId=0;
	int							nSvcId=0;
	int							nProcNum=0;
	int							nProcNum1=0;
	char    					COMMAND_TOUCH[100];  
	
	
	nSystemId =  1;
    nSvcId = 1;
    nProcNum = 0;
    nProcNum1 = 1;
	
	LogM	= new SendLogManage( 1,1,2,1,0,0 );
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	
	 memset( &strDataFile, NULL, sizeof(strDataFile) );
    
    // FLT -> AGR
    sprintf(strDataFile, "/SVC%d/DATA/btreeshm%d%d.cfg",nSvcId,nProcNum,nProcNum1);
    sprintf( COMMAND_TOUCH, "touch %s",strDataFile );
	system( COMMAND_TOUCH );
	
  	pBtree	= new CBtree<KeyDef, UdrFormat>( strDataFile, cId, 1,1,2,1,0,0 );
  	for(int i=1; i<1000; i++ )
  	{
  		testKey.init();
  		testKey.set_unMsID(i);
    	testKey.set_strAccSessionID("233");
  		testKey.set_strCorrID("233");
  		testDataType.init();
  		testDataType.set_keyType( &testKey );
  		makeUdr(&testUdr);
		testDataType.set_dataType(&testUdr);
		pBtree->InsertData( testDataType );
		cout << "number i=" << i << endl;
//		sleep(1);
  	}
    while(flag)
	{
		switch( DisplayMenu() )
		{
			case 1:
				testKey.init();
				cout << "Insert MsId: ";
				cin >> temp;
				testKey.set_unMsID(temp);
    			testKey.set_strAccSessionID("233");
  				testKey.set_strCorrID("233");
  				
  				testDataType.init();
  				testDataType.set_keyType( &testKey );
  				makeUdr(&testUdr);
				testDataType.set_dataType(&testUdr);
				
				pBtree->InsertData( testDataType );
				break;
			case 2:
				testKey.init();
				cout << "Insert MsId: ";
				cin >> temp;
				testKey.set_unMsID(temp);
    			testKey.set_strAccSessionID("100");
  				testKey.set_strCorrID("100");
  				
  				testDataType.init();
  				testDataType.set_keyType( &testKey );
  				makeUdr(&testUdr);
				testDataType.set_dataType(&testUdr);
				
  				pBtree->DelNode( testDataType );
				break;
			case 3:
				pBtree->Print();
				break;
			default:
				flag=0;
				break;
		}
	}
	
    //cFileLogWrite.WriteHexLog((char*)&b,sizeof(b));
    delete LogM;
    delete pBtree;
    return 0;
}
int DisplayMenu(void)
{
	int select;

	cout << "\n\n\n\n" << endl;
	cout << "\t\t*********** TEST BTREE ***********\n" << endl;
	cout << "\t\t*                                *\n" << endl;
	cout << "\t\t* 1. INSERT Btree				  *\n" << endl;
	cout << "\t\t* 2. Delete Btree				  *\n" << endl;
	cout << "\t\t* 3. Display Btree		          *\n" << endl;
	cout << "\t\t* 4. exit                        *\n" << endl;
	cout << "\t\t*                                *\n" << endl;
	cout << "\t\t**********************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}

