/*
 * $Log: UdrReader.cpp $
 * Revision 0.9.0  2001/05/24 17:17:55  dongcheol
 * ...
 * Revision 0.9.1  ...
 * ...
 */


#include "UdrReader.hpp"
#include <fstream.h>

namespace acc {

UdrReader::UdrReader() : ConfigMGR(1,1) {
;
}

UdrReader::~UdrReader() {
;
}

int UdrReader::read(UdrFormat& u) {

		unsigned int	iSeq;		// UDR 화일번호 (1 - 999,999,999)
		unsigned int	iNum;		// 화일내 UDR갯수 (1-1000)
		int 			Filter;
		
		FILE			*fp;



	int		rlen;
	long	fpPosi;
	char 	fname[150];
	
	sprintf(fname, "%s/pdsn_udr.%d",  PATH_NAME_PDSNIF_UDR, iSeq);
	
	if(!(fp = fopen(fname, "r+b")))
	{
	//	cout << "[CONMGR] File Not Found : " << fname << endl;
		return 0;
	}
	
	fpPosi = UDR_SIZE*iNum;
	//cout << "[CONMGR] fpPosi : " << fpPosi << endl;
	
	if(fseek(fp, fpPosi, SEEK_SET)!=0) { 
//		cout << "[CONMGR] fseek ERROR" << endl;
		fclose(fp); 
		return 0; 
	}
	
	rlen = fread((void*)fmUDR, 1, UDR_SIZE, fp);
	if(rlen < UDR_SIZE) 
	{ 
//		cout << "[CONMGR] fread ERROR : " << rlen << endl;
		fclose(fp); 
		return 0; 
	}
	fclose(fp);
	
	//cout << "[CONMGR] Read Position : " << iNum << endl;
	
	iNum++;
	if(iNum>UDR_MAX_COUNT-1) 
	{
		iSeq++;
		if(iSeq > UDR_MAX_SEQ) iSeq=1;
		iNum=0;
	}
	
	return 1;
}



}; // end of namespace acc

using namespace acc;

int main() {
    UdrReader ur;
}
