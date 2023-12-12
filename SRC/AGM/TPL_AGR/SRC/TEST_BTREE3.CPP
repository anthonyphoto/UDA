#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Tpl_Btree.hpp>
#include <SendLogManage.hpp>
//#include <FileLogWrite.hpp>
#include <UDR.hpp>


int DisplayMenu(void);
void makeUdr( UdrFormat* pUdrFormat, int nMsId )
{
	pUdrFormat->get_ItemA()->put_unMsId(nMsId);
}

SendLogManage*    LogM;   // 로그매니저 (Main Class에서 정의)

int main( int argc, char *argv[] )
{  
	char 						buffer[50];
	KeyDef						tempKeyType;
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
	int							i;
	
	
	nSystemId =  1;
    nSvcId = 1;
    nProcNum = 0;
    nProcNum1 = 0;
	
	LogM	= new SendLogManage( 1, 1, 2, 1, 0, 0 );
	LogM->put_EventHistData(FLAG_START, FLAG_PROC_START);
	
	 memset( &strDataFile, NULL, sizeof(strDataFile) );
    
    // FLT -> AGR
    sprintf(strDataFile, "/SVC%d/DATA/btreeshm%d%d.cfg", nSvcId,nProcNum,nProcNum1);
    sprintf( COMMAND_TOUCH, "touch %s",strDataFile );
	system( COMMAND_TOUCH );
	
  	pBtree	= new CBtree<KeyDef,UdrFormat>( strDataFile, cId );
  	/*
  	for(int i=0; i<100; i++ )
  	{
  		tempKeyType.set_unMsID(i);
  		tempKeyType.set_strAccSessionID("1111");
  		tempKeyType.set_strCorrID("2222");
  		testDataType.set_keyType(&tempKeyType);
  		makeUdr(&testUdr, i);
		testDataType.set_dataType(&testUdr);
		pBtree->InsertData( testDataType );
		cout << "number i=" << i << endl;
  	}
  	*/
    while(flag)
	{
		switch( DisplayMenu() )
		{
			case 1:
				cout << "Insert MsId: ";
				cin >> temp;
				
				tempKeyType.set_unMsID(temp);
		  		tempKeyType.set_strAccSessionID("1111");
		  		tempKeyType.set_strCorrID("2222");
		  		testDataType.set_keyType(&tempKeyType);
		  		makeUdr(&testUdr, temp);
				testDataType.set_dataType(&testUdr);
				pBtree->InsertData( testDataType );
				break;
			case 2:
				cout << "Insert MsId: ";
				cin >> temp;
				
  					tempKeyType.set_unMsID(temp);
		  		tempKeyType.set_strAccSessionID("1111");
		  		tempKeyType.set_strCorrID("2222");
		  		testDataType.set_keyType(&tempKeyType);
		  		makeUdr(&testUdr, temp);
				testDataType.set_dataType(&testUdr);
  				pBtree->DelNode( testDataType );
				break;
			case 3:
				pBtree->Print();
				break;
			case 4:
				cout << "Insert MsId: ";
				cin >> temp;
				
				for( i=0; i<10; i++ )
				{
  					tempKeyType.set_unMsID(temp+i);
			  		tempKeyType.set_strAccSessionID("1111");
			  		tempKeyType.set_strCorrID("2222");
			  		testDataType.set_keyType(&tempKeyType);
			  		makeUdr(&testUdr, temp+i);
					testDataType.set_dataType(&testUdr);
	  				pBtree->InsertData( testDataType );
	  			}
  				break;
  			case 5:
				cout << "Insert MsId: ";
				cin >> temp;
				
  				for(i=0; i<10; i++ )
  				{
  					tempKeyType.set_unMsID(temp+i);
			  		tempKeyType.set_strAccSessionID("1111");
			  		tempKeyType.set_strCorrID("2222");
			  		testDataType.set_keyType(&tempKeyType);
			  		makeUdr(&testUdr, temp+i);
					testDataType.set_dataType(&testUdr);
	  				pBtree->DelNode( testDataType );
	  			}
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
	cout << "\t\t* 4. Insert Many		          *\n" << endl;
	cout << "\t\t* 5. Delete Many   	          *\n" << endl;
	cout << "\t\t* 6. exit                        *\n" << endl;
	cout << "\t\t*                                *\n" << endl;
	cout << "\t\t**********************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}

