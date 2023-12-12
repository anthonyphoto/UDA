/*
 * $Log: BDRManager.cpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * ...
 * Revision 0.9.1  me
 * ...
 */


#include "BDRManager.hpp"

#include "../INCLUDE/UdrBds.hpp"  // included local directory
#include <fstream.h>
#include <unistd.h>
#include "CTime.hpp"
#include <vector>

namespace aad {

BDRManager::BDRManager() { }

BDRManager::~BDRManager() { }

string BDRManager::getDate() const {

    CTime tm;
    char temp[13]; memset(temp, 0x00, sizeof(temp));
    tm.Time_Display(temp, 4);
    string time(temp);
    return time;
}

string BDRManager::getPath() const {
    return _path;
}

void BDRManager::setPath(string& s) {
    _path = s;
}

string BDRManager::makeFileName() const {

    string name(getPath());
    name += "AAD";
    name += ".";
    name += getDate();
    return name;
}

void BDRManager::detect(float text, float origin, float multi, const int sequence) {

    _fout.open(makeFileName().c_str(), ios::app);   // check !! open failure!!


    //
    // fallowing source is duplicated from "/home/uda1/user/smy/bds/bdsudr.cpp" : start
    //

    const int AAD_BDR_SIZE_DATA_BLOCK   = 1012;    // One Block Size - Block Header
    const int AAD_BDR_SIZE_ONE_BLOCK    = 1024;    // One Block Size
    const int AAD_BDR_SIZE_BLOCK_HEADER = 12;      // Block Header
    const int AAD_BDR_SIZE_RECORD       = 336;

    char    strpathNm[200];         // BDS File Path
    char    strFileNm[200];         // TextM-GM-|M-=M-DM-@M-G BDS File Name
    
    CBDS_UdrFormat  clsBDSform; // BDS Udr Form

    FILE*   fd;
    unsigned int    unTotalCnt= 0;  // Total BDS Udr Count in  BDS File
    unsigned int    unUdrCnt  = 0;  // BDS Udr Count in One Block
    unsigned int    unBlckCnt = 0;  // Block Count in  BDS File

    char    strTxtbuf[AAD_BDR_SIZE_RECORD+1];               // Text File buffer
    char    strOneBlck[AAD_BDR_SIZE_ONE_BLOCK+1];   // One Block

    CbdsUdrBlockHeader      clsBlckHead;// Block Header
    CbdsUdrFileHeader       clsFileHead;// File Header
    
    memset(strpathNm, 0x00, sizeof(strpathNm));
    memset(strFileNm, 0x00, sizeof(strFileNm));     

#ifdef _UDA1_
    sprintf(strpathNm,"/SVC1/BDS/BDS_UDR.%d", sequence);  // hard coded !!!
#endif

#ifdef _UDA2_
    sprintf(strpathNm,"/SVC3/BDS/BDS_UDR.%d", sequence);  // hard coded !!!
#endif

    //sprintf(strpathNm,"./BDS_UDR.%d", sequence);  // hard coded !!!
    //sprintf(strFileNm,"./BDS_UDR%d.txt", sequence);

    //unlink(strFileNm);

    // BDS File Open
    if((fd = fopen(strpathNm, "r")) == NULL){
            cout << "!!!Error : BDS File open fail." << endl;
            return ;
    }       

    // Read File Header in BDS File
    if(fread((void *)&clsFileHead, sizeof(clsFileHead), 1, fd) < 0){
            cout << "!!!Error : Fail Read BDS File Header." << endl;
            return ;
    }       
    unBlckCnt = clsFileHead.get_unBlockCount();

#ifdef _DEBUG_
    cout << '\n' << "-----BDS File Header-----" << endl;
    cout << "-File Seq       : " << clsFileHead.get_unFileSeq()      << endl;
    cout << "-Block Count    : " << unBlckCnt                                                << endl;
    cout << "-Start BlockSeq : " << clsFileHead.get_unStartBlockSeq()<< endl;
    cout << "-End BlockSeq   : " << clsFileHead.get_unEndBlockSeq()  << '\n' << endl;
#endif
    
    while(unBlckCnt--)
    {        
            // Block Header
            if(fread(strOneBlck, AAD_BDR_SIZE_ONE_BLOCK, 1, fd) < 0){
                    cout << "!!!Error : Fail Read BDS Block Header." << endl;
                    return ;
            }       
            unUdrCnt = ((CbdsUdrBlockHeader *)(strOneBlck))->get_nRecCount();
            unTotalCnt += unUdrCnt;

#ifdef _DEBUG_
            cout << "Block Seq:" << ((CbdsUdrBlockHeader *)(strOneBlck))->get_unBlockSeq()
                     <<     "  ==  BDS Udr Count:" << unUdrCnt << endl;
#endif            
            // BDS Udr
            //clsBDSform.display_UdrItem(&strOneBlck[AAD_BDR_SIZE_BLOCK_HEADER], AAD_BDR_SIZE_RECORD, unUdrCnt, strFileNm);

            clsBDSform.collectUDRItem(&strOneBlck[AAD_BDR_SIZE_BLOCK_HEADER], AAD_BDR_SIZE_RECORD, unUdrCnt, _fout, text, origin, multi);
    }

#ifdef _DEBUG_
    cout << '\n' << "-BDS Udr Total Count : " << unTotalCnt << endl;
    cout << "-------------------------" << '\n' << endl;
#endif

    // UDR file close 
    if (fclose(fd)!=0)
    {
    	cout << " BDS_UDR close error [" << strpathNm << "]" << endl;
    }

    //
    // fallowing source is duplicated from "/home/uda1/user/smy/bds/bdsudr.cpp" : end
    //
    _fout.close();    // check !! close failure!!
}


} // end of namesapce add




