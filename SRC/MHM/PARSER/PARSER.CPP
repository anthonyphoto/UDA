/******************************************************************
* 파 일 명     
      : parser.cpp                
* 기   능                                                          
     : Radius로 부터 들어온 xml형태의 UDR을 parsing한 후, 
       UDR format에 맞게 변형한다. 
* 작 성 자     
     : 신미연                         
* first data  
     : 2000. 10.15                      
* last updata 
     : 2001. 04. 30                  
* 관 련 화 일  
     : parser.h                     
* program history                                
                           
* Version
  : 1.1 ( 2001. 05. 21 )                                           
******************************************************************/


#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream.h>
#include <ctype.h>
#include <string>
#include "parser.h"
#include <SendLogManage.hpp>
extern SendLogManage *clsLogM; 


#define SERVICE_CORRELATION 15
#define END_EVENT_TIME      28
#define AIRLINK_QOS         40
#define UDRTYPE_POSITION    25
extern int		serviceId;

UdrItem ui;

XString::XString() {

    mLength   = 0;
    mCapacity = 1;  
    mpString[0] = '\0';           
}

XString::~XString() {

}

XString::XString(const char* str) {

    assert ( str != 0 ); // C string not NULL?
    mLength   = strlen(str);
    mCapacity = mLength + 1;      // make room for '\0'
    strcpy(mpString, str);
    
}

XString::XString(const XString& str) {

    mLength   = str.length();
    mCapacity = mLength + 1;
    strcpy(mpString, str.mpString);
}

int XString::length() const {

    return mLength;
}

const char* XString::c_str() const {

    return mpString;
}

char XString::operator[](int idx) const {

    if ( idx < 0 || mLength <= idx ) {
/** 
        if (DEBUG) {
            cout << "index out of range : " << idx << " string : " << mpString << endl;
        }
**/        
        exit(1);
    }
    return mpString[idx];
}


// pos의 위치에서부터 검사하여 char와 같다면 그 위치를 return     
int XString::find(char ch, int pos) const {

    int index;
    
    for ( index = pos; index < mLength; index++ ) {
        if ( mpString[index] == ch ) {
            return index;
        }
    }
    return -1;  // not found, default position
}

int XString::find(char ch) const {

    int index;
    
    for ( index = 0; index < mLength; index++ ) {
        if ( mpString[index] == ch ) {
            return index;
        }
    }
    return -1;  // not found, default position
}

void XString::replace(char findch, char repch, int pos) {

            mpString[pos] = repch;
}

int XString::find(const XString& str) const {
    
    int len = str.length();
    int lastIndex = length() - len;   // ?? this.length??
    int strIdx;
    
    for ( strIdx = 0; strIdx <= lastIndex; strIdx++ ) {
        if ( strncmp(mpString+strIdx, str.c_str(), len) == 0 ) {
            return strIdx;
        }
    }
    return -1;  // not found, default position
}

int XString::find(const XString& str, int pos) const {

    int len = str.length();
    int lastIndex = length() - len;   
    int strIdx;
	const char* strSt = str.c_str();
	
    for ( strIdx = pos; strIdx <= lastIndex; strIdx++ ) {
        if ( strncmp(mpString+strIdx, strSt, len) == 0 ) {
            return strIdx;
        }
    }
    return -1;  // not found, default position
}


// UDRTYPE의 4번째 string을 검사
const char* XString::udrType(int pos) const{
	
	switch(toupper(mpString[pos])){
		case 69 : return UDR_INTERIM;	// 'E' --> ASCII '69'
		case 80 : return UDR_STOP;		// 'P' --> ASCII '80'
		case 82 : return UDR_START;		// 'R' --> ASCII '82'
		default : break;
	}	
}


// pos(start position)에서 length만큼 cutting
XString XString::substr(int pos, int len) const {

    if (pos < 0) {
        pos = 0;   // start at front when pos < 0
    }
    
    if (pos >= mLength) {
        return "";  // empty string
    }

    int lastIndex = pos + len - 1;  // last char's index to copy 
    
    if (lastIndex >= mLength) {   // end of string?
        lastIndex = mLength - 1;
    }

    XString result(*this);  // make sure enough space allocated
  
    int orgIdx;
    int subIdx;
    
    for (orgIdx = 0, subIdx = pos; subIdx <= lastIndex; orgIdx++, subIdx++) {
    	result.mpString[orgIdx] = mpString[subIdx];
    }
    result.mpString[orgIdx] = '\0';
    result.mLength = orgIdx;
    return result;
}


const XString& XString::operator=(const XString& rhs) {

    if (this != &rhs) {

        if (mCapacity < rhs.length() + 1) {     

            mCapacity = rhs.length() + 1;        // add 1 for '\0'
        }
        memset(mpString, 0x00, mLength);
        mLength = rhs.length();
        strcpy(mpString,rhs.mpString);
    }
    return *this;
}

const XString& XString::operator=(const char* s) {

    int len = 0;                  // length of newly constructed string
    assert(s != 0);               // make sure s non-NULL
    len = strlen(s);              // # of characters in string

    // free old string if necessary
    if (mCapacity < len + 1) {

        mCapacity = len + 1;      // add 1 for '\0'
    }
    memset(mpString, 0x00, mLength);
    mLength = len;
    strcpy(mpString, s);
    return *this;
}


const XString& XString::operator+=(const XString & str) {

    XString copystring(str);                 // copy to avoid aliasing problems

    int newLength = length() + str.length(); // self + added string
    int lastLocation = length();             // index of '\0'

/**
    // check to see if local buffer not big enough
    if (newLength >= mCapacity) {

        mCapacity = newLength + 1;
        if (str.length() == 1) {             // special case for catenating one char
                                             // make room for future catenations
            mCapacity *= 2;
        }
        char* newBuffer = new char[mCapacity];
        strcpy(newBuffer,mpString);          // copy into new buffer
        delete [] mpString;                  // delete old string
        mpString = newBuffer;
    }
**/
    ///// now catenate str (copystring) to end of myCstring
    strcpy(mpString+lastLocation,copystring.c_str() );
    mLength = newLength;                     // update information

    return *this;
}


XString operator+(const XString& lhs, const XString& rhs) {

    XString result(lhs);  // copies lhs to result
    result += rhs;        // catenate rhs
    return result;        // returns a copy of result
}

XString XString::extractItem(const XString& str) const {
    
    //////////// precondition "<HA>150</HA>"    
    //////////// postcondition "150"

    if( find(str) == -1 ) { // not found 
        if (DEBUG) {
            cout << "extractItem(): not found " << str << " tag" << endl;        
        }
        return "";
    }
    
    int startPos = find(str) + str.length();

    if ( startPos >= mLength ) {
        if (DEBUG) {
            cout << "extractItem(): invalid position" << endl;
        }
        return "";
    }

    int len = ( find(str, startPos) - startPos ) - 2;

    XString result(substr(startPos, len));

    return result;
}

XString XString::extractItem(const char* str) const {
        
    if( find(str) == -1 ) { // not found 
        if (DEBUG) {
            cout << "extractItem(): not found " << str << " tag" << endl;
        }
        return "";
    }

    int startPos = find(str) + strlen(str);

    if ( startPos >= mLength ) {
        if (DEBUG) {
            cout << "extractItem(): invalid position" << endl;
        }
        return "";
    }

    int len = ( find(str, startPos) - startPos ) - 2;

    XString result(substr(startPos, len));
    
    return result;
}
 
   
ostream& operator<<(ostream& os, const XString& str) {

    return os << str.c_str();
}

bool XString::parse(ConvUdr& cudr) const {
    XString rawudr;
    XString starttag(START_TAG);
    XString endtag(END_TAG);

    int rstart = 0;
    int rlen   = 0;
    int rpos   = 0;
    int udrreccnt = 0;
    int udritemcnt = ui.maxCapacity();
   
    int udrtypelen=0;

    XString udrtypestr;
    XString udrtype(UDRTYPE);
    XString result;

    char logbuf[30];
    int i        = 0;
	int startpos = 0;
	int itemlen  = 0;
    //////////////////////////////////////
    // IN HERE, DO MAKE GRAMMER CHECKER //
    //////////////////////////////////////

    int udrsize = length();
   
    if (udrsize <= 0) {
        if (DEBUG) {
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1000) << "]" << " [ length is very short ] " << endl;
        }
        return FALSE;
    }
    
   
   while ( rpos >= 0 && rpos <= udrsize ) {
		
        rstart = find(starttag, rpos);
        
        if ( rstart == -1 ) {            // end of string
/**
            if (DEBUG) {
                *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1001) << "]"
                		 << " [ Not found start tag ] " 					 << endl;
            }
**/
            break;
        }

        rpos   = rstart + starttag.length();
        rpos   = find(endtag, rpos+1200) + endtag.length();   // <UDRDOC>..about 124x Byte..</UDR>
        rlen   = rpos - rstart;
        rpos   = rpos - endtag.length();
               
        if ( rpos < 0 ) { 
            if (DEBUG) { 
                *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1002) << "]" << " [ Not found end tag ] " << endl;
            }
            return FALSE;
        }
    	
        rawudr = substr(rstart, rlen);  // cutting <UDRDOC><UDR>...</UDR>
/**
        if (DEBUG) { 
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1003) << "]"
            		 << "Can't cutting one Udr in the XML data" 		 << endl;
        }
**/
        ///// chopping udr type  ex. <UDR UDRTYPE="START"><M_ID>
        
        const char* udrstr = rawudr.udrType(UDRTYPE_POSITION);	// get UDRTYPE
		udrtypelen = strlen(udrstr);
		
        udrtypelen -= 4;		// if   (UDRTYPE==START)   : standard position + 1    
        						// else (UDRTYPE==INTERIM) : standard position + 3
        
        ///// chopping item
        for (int j = 0; j < udritemcnt; j++ ) {

			if((j > AIRLINK_QOS) && (j < udritemcnt))						ui.putSkipTag(j);
		    else if((j == SERVICE_CORRELATION) || (j == END_EVENT_TIME))	ui.putSkipTag(j);
    	    else{	
			   startpos = ui.mStartPotion[j] + udrtypelen;
    		   itemlen = ui.mEndPosition[j] - startpos + udrtypelen + 1;
			   result = rawudr.substr(startpos, itemlen);	// <..> tag사이의 값을 가져옴 </..> 
			   ui.put(j, result.c_str());
    		} 	    
        } 

//        if (DEBUG) { ui.print(); }
        //////////// A Udr에 대해서 Conversion
        if ( cudr.resolve(ui, udrreccnt, udrstr) == TRUE ) {
            cudr.setCount("P");
            udrreccnt++;
          
        } else {
            memset(logbuf, 0x00, sizeof(logbuf));            
            sprintf(logbuf, "[ FSEQ ] [ %d ]\n", cudr.getFileSeq());
            cudr.writeDropLog(logbuf);
            cudr.writeDropLog(rawudr.c_str());
            cudr.writeDropLog("\n\n");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1046)    << "]" 
            		 << " [ UDR DROP ] [ FSEQ " << cudr.getFileSeq() << "]" << " [ Write to /SVC"
            		 << serviceId
            		 << "/RADIUS/parser_drop.log ] " << endl;            
            cudr.setCount("D");
        }
        udrtypestr = "";
        rawudr = "";
        result = "";
        cudr.setCount("T");
    } // end of while
 
    return TRUE;
}

unsigned int XString::IPstrtoi(void) {

    XString str(*this);
    
    int len  = 0;
    int cpos = 0;
    int spos = 0;
    
    char aclass[4];
    char bclass[4];
    char cclass[4];
    char dclass[4];
    
    memset(aclass, 0x00, sizeof(aclass));
    memset(bclass, 0x00, sizeof(bclass));
    memset(cclass, 0x00, sizeof(cclass));
    memset(dclass, 0x00, sizeof(dclass));
    
    ///// check ip adddress
    ///// 255.255.255.255 type이 아니면 UDR DROP    
    XString imdot(".");

    int dot = 0;
    while (1) {
        cpos = str.find(imdot, cpos+1);
        if (cpos == -1) {
            break;
        }
        ++dot;
    }

    if (dot != 3) {
        return -2;   // invalid address, udr drop
    }
    
    ///////// convert ip to unsigned integer;    

    cpos = 0;
    cpos = str.find('.', spos);
    len = cpos - spos;
    memcpy(aclass, str.substr(spos, len).c_str(), len);
    spos = cpos + 1;

    cpos = str.find('.', spos);
    len = cpos - spos;
    memcpy(bclass, str.substr(spos, len).c_str(), len);
    spos = cpos + 1;

    cpos = str.find('.', spos);
    len = cpos - spos;
    memcpy(cclass, str.substr(spos, len).c_str(), len);
    spos = cpos + 1;

    cpos = str.find(' ', spos);
    len = cpos - spos;
    memcpy(dclass, str.substr(spos, len).c_str(), len);

    unsigned int ipaddr = 0;
    unsigned int temp = 0;
    
    temp = (unsigned int)atoi(aclass);
    ipaddr |= temp << 24;    

    temp = (unsigned int)atoi(bclass);
    ipaddr |= temp << 16;    
    
    temp = (unsigned int)atoi(cclass);
    ipaddr |= temp << 8;    
    
    temp = (unsigned int)atoi(dclass);
    ipaddr |= temp;

    return ipaddr;
}

UdrItem::UdrItem() {

    int itemcount = 0;
    string temp[CNT_UDR_ITEM];
    
    ///// getting tag list and item attribute
    for (int k = 0; k < CNT_UDR_ITEM; k++) {
        if ( stConfigUdr[k].nType != TAG_TYPE ) {
            temp[itemcount] = stConfigUdr[k].strPattern;
            mConfigItemType[itemcount]   = stConfigUdr[k].nType;
            mConfigItemLength[itemcount] = stConfigUdr[k].nStringLen;
            mStartPotion[itemcount] = stConfigUdr[k].nPosition;
            mEndPosition[itemcount] = stConfigUdr[k].nEndPosition;
            itemcount++;
        }
    }

    mCapacity = itemcount;   
    mTagSlot  = new string[mCapacity];
    mItemSlot = new string[mCapacity];
   
    ///// copy tag list
    for (k = 0; k < mCapacity; k++) {
        mTagSlot[k] = temp[k];
    }
    
    ///// clear skip tag list
    for (k = 0; k < mCapacity; k++) {
        mTagSkip[k] = 0;
    }
}

UdrItem::~UdrItem() { 
    
    delete[] mTagSlot;
    delete[] mItemSlot;
}

void UdrItem::put(int i, const string& item) {

    mItemSlot[i] = item;
}

void UdrItem::putSkipTag(int item) {

    mTagSkip[item] = 1;
}

int  UdrItem::getSkipTag(int item) const {

    return mTagSkip[item];
}

bool UdrItem::get(const string& tag, string& item) const {
    
    for (int i = 0; i < mCapacity; i++) {
        if (mTagSlot[i] == tag) {
            item = mItemSlot[i];
            return TRUE;
        }
    }
    return FALSE;    
}

const char* UdrItem::getTag(int itemindex) const {

    if (itemindex >= mCapacity) {
        return "";
    }
    
    // string --> c string type
    string str(mTagSlot[itemindex]);
    char *tmp;

    int len=str.length();
    tmp = new char[len+1];
    for(int j = 0; j < len; j++) {
        tmp[j] = str[j];
    }
    tmp[j]='\0';

    char* ret = tmp;
    delete[] tmp;

    return ret;
    
}

const char* UdrItem::getItem(int itemindex) const {

    if (itemindex >= mCapacity) {
        return "";
    }
    
    // string --> c string type
    string str(mItemSlot[itemindex]);
	static char tmp[128];
    int len=str.length();

    for(int j = 0; j < len; j++) {
        tmp[j] = str[j];
    }
    tmp[len] = NULL;

	return tmp;
}

int UdrItem::maxCapacity(void) const {

    return mCapacity;
}

int UdrItem::getItemType(int index) const {

    return mConfigItemType[index];
}

int UdrItem::getItemLength(int index) const {

    return mConfigItemLength[index];
}

void UdrItem::print(void) {
    
    cout << ">>>>>>>>>>>>>>>>>>> ITEM LIST >>>>>>>>>>>>>>>>>>>" << endl;
    for (int i = 0; i < mCapacity; i++) {
        cout << i; 
        cout << "   TAG : ";
        cout.width(12);
        cout << mTagSlot[i];
        cout << "  ITEM : ";
        cout.setf(ios::left);
        cout.width(65);
        cout << mItemSlot[i];
//        cout << "ITEM LEN : ";
//        cout << mItemSlot[i].length() << endl;

//        cout << " TP : ";
//        cout << mConfigItemType[i];
//        cout << " LN : ";
//        cout << mConfigItemLength[i];
        cout << endl;
    }    	
}

ConvUdr::ConvUdr() {

    memset((char*)&stConvUdr, 0x00, sizeof(stConvUdr));
    memset((char*)&m_rdbuf,   0x00, sizeof(m_rdbuf));
    memset((char*)&m_wtbuf,   0x00, sizeof(m_wtbuf));
    m_udrtotalcnt   = 0;
    m_udrprocesscnt = 0;
    m_udrdropcnt    = 0;    
    m_fileseq       = 0;
    m_udrseq        = 0;

    ///// improvised --> later read configuration
    mReadLog = new Log("/SVC1/LOG/parser_read.log");

	char mDropLogName[100];
	sprintf(mDropLogName, "/SVC%d/RADIUS/parser_drop.log", serviceId); 

    mDropLog = new Log(mDropLogName);
    mSendLog = new Log("/SVC1/LOG/parser_send.log");
}

ConvUdr::~ConvUdr() {
    
    delete mReadLog;
    delete mDropLog;
    delete mSendLog;    
}

void ConvUdr::clear(void) {

    memset((char*)&stConvUdr, 0x00, sizeof(stConvUdr));
    memset((char*)&m_rdbuf,   0x00, sizeof(m_rdbuf));
    memset((char*)&m_wtbuf,   0x00, sizeof(m_wtbuf));
    m_udrtotalcnt   = 0;
    m_udrprocesscnt = 0;
    m_udrdropcnt    = 0;    
    m_fileseq       = 0;
    m_udrseq        = 0;
}

const char* ConvUdr::setDefaultStr(const UdrItem& ui, int index) {

	static char temp[33];  
    int len = ui.getItemLength(index);
	temp[len] = NULL;
    memset(temp, '*', len);
    return temp;
}

const char* ConvUdr::validateStr(const UdrItem& ui, int index) {

    char tempstr[64];
    const char* uitem = ui.getItem(index);
    int lenstr = strlen(uitem);

    memset(tempstr, 0x00, 64);    
    memcpy(tempstr, uitem, lenstr);

    if ( tempstr[0] == ' ' || lenstr == 0 ) {

		static char temp[33];  
    	int len = ui.getItemLength(index);
		temp[len] = NULL;
   		memset(temp, '*', len);
    	return temp;  
    }

    return uitem;
}

int ConvUdr::setDefaultInt(void) {

    struct result_type {
        int hbit1:8;
        int hbit2:8;
        int hbit3:8;
        int hbit4:8;
    } ret;
    ret.hbit1 = 0x00;
    ret.hbit2 = 0x00;
    ret.hbit3 = 0x00;
    ret.hbit4 = 0x00;
        
    int result;
    memcpy((char*)&result, (char*)&ret, sizeof(result));

    return result;    
}

unsigned int ConvUdr::setDefaultUInt(void) {

    struct result_type {
        int hbit1:8;
        int hbit2:8;
        int hbit3:8;
        int hbit4:8;
    } ret;
    ret.hbit1 = 0x00;
    ret.hbit2 = 0x00;
    ret.hbit3 = 0x00;
    ret.hbit4 = 0x00;
        
    unsigned int result;
    memcpy((char*)&result, (char*)&ret, sizeof(result));

    return result;    
}

int ConvUdr::validateInt(const UdrItem& ui, int index) {

    char tempstr[64];
    const char* uitem = ui.getItem(index);    
    int lenstr = strlen(uitem);

    memset(tempstr, 0x00, 64);    
    memcpy(tempstr, uitem, lenstr);

    int rindex = lenstr - 1;
    
    //////////// Integer 이외의 값은 모두 비정상 UDR로 간주 Drop 

    for (int i = rindex; i >= 0; i--) {
        
        if (isspace(tempstr[i]) == 0) {
            int lastofint = i;
            for (int j = 0; j <= lastofint; j++) {
                if (isdigit(tempstr[j]) == 0) {
/**
                    if (DEBUG) {
                        cout << "validateInt(): unexpected data detected : " << tempstr[j] << " " << j << endl;
                    }
**/                    
                    return -2;  // unexpected data 
                }
            }
            break;
        }
    }

    int result = 0;

    if ( tempstr[0] == ' ' || lenstr == 0 ) {
        struct result_type {
            int hbit1:8;
            int hbit2:8;
            int hbit3:8;
            int hbit4:8;
        } ret;
        ret.hbit1 = 0x00;
        ret.hbit2 = 0x00;
        ret.hbit3 = 0x00;
        ret.hbit4 = 0x00;
            
        memcpy((char*)&result, (char*)&ret, 4);
        return result;    
    }
        
    result = atoi(uitem);        
    return result;
}

unsigned int ConvUdr::validateIp(const UdrItem& ui, int index) {

    char temp[64];
    const char* uitem = ui.getItem(index);    
    int len = strlen(uitem);

    memset(temp, 0x00, 64);    
    memcpy(temp, uitem, len);

    unsigned int result;
    
    if ( temp[0] == ' ' || len == 0 ) {
     
        struct result_type {
            int hbit1:8;
            int hbit2:8;
            int hbit3:8;
            int hbit4:8;
        } ret;
        ret.hbit1 = 0x00;
        ret.hbit2 = 0x00;
        ret.hbit3 = 0x00;
        ret.hbit4 = 0x00;
          
        memcpy((char*)&result, (char*)&ret, 4);
        
        return result;
    }

    XString ip(uitem);
    result = ip.IPstrtoi();

    return result;
}

bool ConvUdr::resolve(UdrItem& ui, int index, const char* udrtype) {
    
    //////////// if ui.maxCapacity > 44 then can not convert
    //////////// conversion and validation 처리 

    char tempstr[64];
    char substr[5];    
    int  lenstr;
    int  intconversed = 0;
    unsigned int uintconversed = 0;
    
//        stConvUdr[index].stUdrHeader.nUdrSeqNo
    
    //////////// UDR TYPE INFO
    //////////// to upper case only   

    char upperudrtype[64];
    char inputudrtype[64];
    
    memset(upperudrtype, 0x00, sizeof(upperudrtype));
    memset(inputudrtype, 0x00, sizeof(inputudrtype));
    memcpy(inputudrtype, udrtype, strlen(udrtype));
    
    for (int n = 0; n < strlen(udrtype); n++) {
        upperudrtype[n] = toupper(inputudrtype[n]);
    }    

    if (memcmp(upperudrtype, UDR_START, sizeof(UDR_START)) == 0) {
        stConvUdr[index].stUdrHeader.put_nUdrType(START);
    } else if (memcmp(upperudrtype, UDR_INTERIM, sizeof(UDR_INTERIM)) == 0) {
        stConvUdr[index].stUdrHeader.put_nUdrType(INTERIM);
    } else if (memcmp(upperudrtype, UDR_STOP, sizeof(UDR_STOP)) == 0) {
        stConvUdr[index].stUdrHeader.put_nUdrType(STOP);
    } else {
        writeDropLog("[ UDR DROP ] [ undefined udr type ] [ ");
        writeDropLog(udrtype);
        writeDropLog(" ] ");

		*clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1004) << "]" 
				 << "[ UDR DROP ] [ Undefined udr type ] [ " 		 << udrtype << " ] " << endl;		
        return FALSE;
    }

    ///// set file sequence
    stConvUdr[index].stUdrHeader.put_processType(getProcessType());
    stConvUdr[index].stUdrHeader.put_processSN(getProcessSN());
    stConvUdr[index].stUdrHeader.put_processRemark(getProcessRemark());
    stConvUdr[index].stUdrHeader.put_nUdrFileSeq(getFileSeq());
   
    //////////// min check

    lenstr = strlen(validateStr(ui, 0));
    memset(tempstr, 0x00, sizeof(tempstr));
    memcpy(tempstr, validateStr(ui, 0), lenstr);

    if ( ui.getSkipTag(0) != 0 && lenstr != LEN_MSID ) {
        writeDropLog("[ UDR DROP ] [ 0_MobileIdenfier ] ");
        *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1005) << "]" 
        		 << "[ UDR DROP ] [ Invalid MSID Length ]" 			 << endl;
        return FALSE;
    }
    
    if (memcmp(tempstr, "119", 3) != 0 &&
        memcmp(tempstr, "011", 3) != 0 &&
        memcmp(tempstr, "017", 3) != 0) {
        writeDropLog("[ UDR DROP ] [ undefined min ] [ ");
        writeDropLog(tempstr);
        writeDropLog(" ] ");
        *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1006) << "]"
        		 << " [ UDR DROP ] [ Undefined min ] [ " << tempstr << " ] "	 << endl;
        return FALSE;
    } 

    //////////// min info 

    if (tempstr[0] == '0') {

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr, 3);
        stConvUdr[index].stUdrHeader.put_nServiceProvider(atoi(substr));

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr+3, 3);
        stConvUdr[index].stUdrHeader.put_nMin1(atoi(substr));

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr+6, 4);
        stConvUdr[index].stUdrHeader.put_nMin2(atoi(substr));

    } else if (tempstr[1] == '1') {

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr, 2);
        stConvUdr[index].stUdrHeader.put_nServiceProvider(atoi(substr));

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr+2, 4);
        stConvUdr[index].stUdrHeader.put_nMin1(atoi(substr));

        memset(substr, 0x00, sizeof(substr));
        memcpy(substr, tempstr+6, 4);
        stConvUdr[index].stUdrHeader.put_nMin2(atoi(substr));
        
    } else {
        writeDropLog("[ UDR DROP ] [ undefined min info ] [ ");
        writeDropLog(substr);
        writeDropLog(" ] ");
        return FALSE;
    }
    
    //////////// __MobileIdenfier

    if ( ui.getSkipTag(0) == 0 ) {
        intconversed = validateInt(ui, 0);
   
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_MobileIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1007) << "]"
     	    		 << "[ UDR DROP ] [ MSID : Not digit ]" 			 << endl;
            return FALSE;
        }
        stConvUdr[index].stItemA.put_unMsId(intconversed); 
    } else {
        writeDropLog("[ UDR DROP ] [ 2_MobileIdenfier ] ");
        return FALSE;
    }
    
    //////////// __UserIdenfier
/*	int  i1 = 0;
    int  i2 = 0;
    int  i3 = 0;
    int  i4 = 0;
*/
	
    if ( ui.getSkipTag(1) == 0 ) {
        uintconversed = validateIp(ui, 1);
/*
		i1=uintconversed/256/65536;
        i2=(uintconversed-i1*256*65536)/65536;
        i3=(uintconversed-i1*256*65536-i2*65536)/256;
        i4=uintconversed-i1*256*65536-i2*65536-i3*256;
        printf("%d.%d.%d.%d\n",i1,i2,i3,i4);
*/
        
        if ( uintconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_UserIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1008) << "]"
     	    		 << " [ UDR DROP ] [ IP_ADDR : Invalid Format ]"		 << endl;
            return FALSE;
        }
        stConvUdr[index].stItemB.put_nIpAddr(uintconversed); 
 		   
    } else {
        stConvUdr[index].stItemB.put_nIpAddr(setDefaultUInt());
    }

    if ( ui.getSkipTag(2) == 0) {
        if ( strlen(ui.getItem(2)) == LEN_NAI ) {
            ///// "unauthenticated      ..." 경우에만 처리 됨 "   unauthenticated       ..."은 미처리 
            if (memcmp(validateStr(ui, 2), UNAUTHENTICATED, strlen(UNAUTHENTICATED)) == 0) {
                writeDropLog("[ UDR DROP ] [ 2_UserIdenfier unauthenticated ] ");
          	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1009)		 << "]"
          	    		 << " [ UDR DROP ] [ NAI : Unauthenticated, Invalid length ]" << endl;        
                return FALSE;
            }
            stConvUdr[index].stItemB.put_strNAI(validateStr(ui, 2));
        } else {
            writeDropLog("[ UDR DROP ] [ 2_UserIdenfier ] ");
            return FALSE;
        }        
    } else {
        stConvUdr[index].stItemB.put_strNAI(setDefaultStr(ui, 2));
    }

    //////////// __SessionIdenfier
    
    if ( ui.getSkipTag(3) == 0 ) {
        if ( strlen(ui.getItem(3)) == LEN_ACCOUNT_SESSION_ID ) {
            stConvUdr[index].stItemC.put_strAccountSessionID(validateStr(ui, 3));
        } else {
	     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1010)	   << "]"
	     	    		 << "[ UDR DROP ] [ ACCOUNT_SESSION_ID : Invalid length ]" << endl;
            writeDropLog(" [ UDR DROP ] [ 1_SessionIdenfier ] ");
            return FALSE;
        }        
    } else {
        stConvUdr[index].stItemC.put_strAccountSessionID(setDefaultStr(ui, 3));
    }

    if ( ui.getSkipTag(4) == 0 ) {
        if ( strlen(ui.getItem(4)) == LEN_CORRELATION_ID ) {
            stConvUdr[index].stItemC.put_strCorrelationID(validateStr(ui, 4));
        } else {
            writeDropLog("[ UDR DROP ] [ 2_SessionIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1011)   << "]"
     	    		 << " [ UDR DROP ] [ CORRELATION_ID : Invalid length ]" << endl;
            return FALSE;
        }        
    } else {
        stConvUdr[index].stItemC.put_strCorrelationID(setDefaultStr(ui, 4));
    }
    
    //////////// __InfraIdenfier

    if ( ui.getSkipTag(5) == 0 ) {
        uintconversed = validateIp(ui, 5);

        if ( uintconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_InfraIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1012) << "]"
     	    		 << " [ UDR DROP ] [ HomeAgent : Invalid Format ]" 	 << endl;
            return FALSE;
        }
        stConvUdr[index].stItemD.put_nMIP_HomeAgent(uintconversed); 
    } else {
        stConvUdr[index].stItemD.put_nMIP_HomeAgent(setDefaultUInt());
    }

    if ( ui.getSkipTag(6) == 0 ) {
        uintconversed = validateIp(ui, 6);

        if ( uintconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 2_InfraIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1013) << "]"
     	    		 << " [ UDR DROP ] [ PDSN_FA_Addr : Invalid Format ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemD.put_nPDSN_FA_Addr(uintconversed); 
    } else {
        stConvUdr[index].stItemD.put_nPDSN_FA_Addr(setDefaultUInt());
    }

    if ( ui.getSkipTag(7) == 0 ) {
        uintconversed = validateIp(ui, 7);
        if ( uintconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 3_InfraIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1014) << "]"
     	    		 << " [ UDR DROP ] [ Serving_PCF : Invalid Format ]"  << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemD.put_nServing_PCF(uintconversed); 
    } else {
        stConvUdr[index].stItemD.put_nServing_PCF(setDefaultUInt());
    }

    if ( ui.getSkipTag(8) == 0 ) {
        if ( strlen(ui.getItem(8)) == LEN_BS_MSC_ID ) {
            char strmscid[LEN_BS_MSC_ID];            
            char strupper[LEN_BS_MSC_ID+1];
            memset(strmscid, 0x00, sizeof(strmscid));
            memset(strupper, 0x00, sizeof(strupper));
            memcpy(strmscid, ui.getItem(8), LEN_BS_MSC_ID);

            for (int m = 0; m < LEN_BS_MSC_ID; m++) {
                strupper[m] = toupper(strmscid[m]);
            }
            
            stConvUdr[index].stItemD.put_strBS_MSC_ID(strupper); 
        } else {
            writeDropLog("[ UDR DROP ] [ 4_InfraIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1015) << "]"
     	    		 << " [ UDR DROP ] [ BS_MSC_ID : Invalid length ]"	 << endl;  
            return FALSE;
        }
    } else {
        char bsmscid[LEN_BS_MSC_ID];
        memset(bsmscid, '*', sizeof(bsmscid));
        stConvUdr[index].stItemD.put_strBS_MSC_ID(bsmscid);
    }

    //////////// __ZoneIdenfier

    if ( ui.getSkipTag(9) == 0 ) {
        intconversed = validateInt(ui, 9);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ __ZoneIdenfier ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1016) << "]"
     	    		 << " [ UDR DROP ] [ UserZone : Not digit ]" 		 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemE.put_nUserZone(intconversed); 
    } else {
        stConvUdr[index].stItemE.put_nUserZone(setDefaultInt());
    }

    //////////// __SessionStatus

    if ( ui.getSkipTag(10) == 0 ) {
        intconversed = validateInt(ui, 10);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1017) << "]"
     	    		 << " [ UDR DROP ] [ ForwardMUXOpt : Not digit ]"	 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nForwardMUXOpt(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nForwardMUXOpt(setDefaultInt());
    }

    if ( ui.getSkipTag(11) == 0 ) {
        intconversed = validateInt(ui, 11);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 2_SessionStatus ] ");
      	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1018) << "]"
      	    		 << " [ UDR DROP ] [ ReverseMUXOpt : Not digit ]"	 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nReverseMUXOpt(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nReverseMUXOpt(setDefaultInt());
    }

    if ( ui.getSkipTag(12) == 0 ) {
        intconversed = validateInt(ui, 12);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 3_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1019) << "]"
     	     		 << " [ UDR DROP ] [ Forward_Fund_Rat: Not digit ]"   << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nForward_Fund_Rate(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nForward_Fund_Rate(setDefaultInt());
    }

    if ( ui.getSkipTag(13) == 0 ) {
        intconversed = validateInt(ui, 13);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 4_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1020) << "]"
     	     		 << " [ UDR DROP ] [ everse_Fund_Rate : Not digit ]"  << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nReverse_Fund_Rate(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nReverse_Fund_Rate(setDefaultInt());
    }

    if ( ui.getSkipTag(14) == 0 ) {
        intconversed = validateInt(ui, 14);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 5_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1021) << "]" 
     	    		 << " [ UDR DROP ] [ ServiceOpt : Not digit ]" 		 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nServiceOpt(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nServiceOpt(setDefaultInt());
    }

    if ( ui.getSkipTag(16) == 0 ) {
        intconversed = validateInt(ui, 16);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 6_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1022)  << "]"
     	 		     << " [ UDR DROP ] [ ForwardTrafficType : Not digit ]" << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nForwardTrafficType(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nForwardTrafficType(setDefaultInt());
    }

    if ( ui.getSkipTag(17) == 0 ) {
        intconversed = validateInt(ui, 17);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 7_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1023)  << "]"
     	    		 << " [ UDR DROP ] [ ReverseTrafficType : Not digit ]" << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nReverseTrafficType(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nReverseTrafficType(setDefaultInt());
    }

    if ( ui.getSkipTag(18) == 0 ) {
        intconversed = validateInt(ui, 18);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 8_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1024) << "]"
     	    		 << " [ UDR DROP ] [ FundFrameSize : Not digit ]"	 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nFundFrameSize(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nFundFrameSize(setDefaultInt());
    }

    if ( ui.getSkipTag(19) == 0 ) {
        intconversed = validateInt(ui, 19);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 9_SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1025)   << "]"
     	 		    << " [ UDR DROP ] [ ForwardFundamentalRC : Not digit ]" << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nForwardFundamentalRC(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nForwardFundamentalRC(setDefaultInt());
    }

    if ( ui.getSkipTag(20) == 0 ) {
        intconversed = validateInt(ui, 20);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 10SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1026) 	<< "]"
     	    		 << " [ UDR DROP ] [ ReverseFundamentalRC : Not digit ]" << endl;              
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nReverseFundamentalRC(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nReverseFundamentalRC(setDefaultInt());
    }

    if ( ui.getSkipTag(21) == 0 ) {
        intconversed = validateInt(ui, 21);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 11SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1027) << "]"
     			     << " [ UDR DROP ] [ IP_Technology : Not digit ]" 	 << endl;  
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nIP_Technology(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nIP_Technology(setDefaultInt());
    }

    if ( ui.getSkipTag(22) == 0 ) {
        intconversed = validateInt(ui, 22);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 12SessionStatus ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1028)   << "]"
     			     << " [ UDR DROP ] [ CompulsoryTunnelInd : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nCompulsoryTunnelInd(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nCompulsoryTunnelInd(setDefaultInt());
    }

    if ( ui.getSkipTag(23) == 0 ) {
        intconversed = validateInt(ui, 23);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 13SessionStatus ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1029) << "]"
         		     << " [ UDR DROP ] [ ReleaseIndicator : Not digit ]"  << endl; 
            return FALSE;
        }
        stConvUdr[index].stItemF.put_nReleaseIndicator(intconversed); 
    } else {
        stConvUdr[index].stItemF.put_nReleaseIndicator(setDefaultInt());
    }

    //////////// __SessionActivity
    
    if ( ui.getSkipTag(24) == 0 ) {
        intconversed = validateInt(ui, 24);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_SessionActivity ] ");
			*clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1030)    << "]"
					 << " [ UDR DROP ] [ DataOctetCntTerminat : Not digit ]" << endl; 
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nDataOctetCntTerminat(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nDataOctetCntTerminat(setDefaultInt());
    }

    if ( ui.getSkipTag(25) == 0 ) {
        intconversed = validateInt(ui, 25);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 2_SessionActivity ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1031)  << "]"
     			     << " [ UDR DROP ] [ DataOctetCntOrigin : Not digit ]" << endl;            
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nDataOctetCntOrigin(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nDataOctetCntOrigin(setDefaultInt());
    }

    if ( ui.getSkipTag(26) == 0 ) {
        intconversed = validateInt(ui, 26);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 3_SessionActivity ] ");
     	    *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1032) << "]"
  		   	     << " [ UDR DROP ] [ BadPPPframeCnt : Not digit ]" 		 << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nBadPPPframeCnt(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nBadPPPframeCnt(setDefaultInt());
    }

    ///// Event Time이 0이면 Filter로 보내지 않음
    ///// START, INTERIM, STOP  --> start event time = 0, set only end event time  
    if ( stConvUdr[index].stUdrHeader.get_nUdrType() == START ) {
        if ( validateInt(ui, 27) == 0 ) {    
            writeDropLog("[ UDR DROP ] [ 4_SessionActivity Event Time 0 ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1033) << "]"
 		            << " [ UDR DROP ] [ StartEventTime : Value is zero ]" << endl;
            return FALSE;
        }
//        stConvUdr[index].stItemG.put_nStartEventTime(setDefaultInt()); 
        stConvUdr[index].stItemG.put_nStartEventTime(0); 
        stConvUdr[index].stItemG.put_nEndEventTime(validateInt(ui, 27)); 
    } else if ( stConvUdr[index].stUdrHeader.get_nUdrType() == INTERIM ||
                stConvUdr[index].stUdrHeader.get_nUdrType() == STOP ) {
        if ( validateInt(ui, 27) == 0 ) {    
            writeDropLog(" [ UDR DROP ] [ 4_SessionActivity Event Time 0 ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1033)  << "]"
            		 << "[ UDR DROP ] [ StartEventTime : Value is zero ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nStartEventTime(0); 
        stConvUdr[index].stItemG.put_nEndEventTime(validateInt(ui, 27)); 
    }

    if ( ui.getSkipTag(29) == 0 ) {
        intconversed = validateInt(ui, 29);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 5_SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1034) << "]"
            		 << "[ UDR DROP ] [ ActiveTime : Not digit ]"		 << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nActiveTime(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nActiveTime(setDefaultInt());
    }

    if ( ui.getSkipTag(30) == 0 ) {
        intconversed = validateInt(ui, 30);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 6_SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1035)	  << "]"
       		         << " [ UDR DROP ] [ NumOfActiveTransitions : Not digit ]" << endl;
            return FALSE;
        }
        
        stConvUdr[index].stItemG.put_nNumOfActiveTransitions(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nNumOfActiveTransitions(setDefaultInt());
    }

    if ( ui.getSkipTag(31) == 0 ) {
        intconversed = validateInt(ui, 31);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 7_SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1036)	  << "]"
  		             << " [ UDR DROP ] [ SDB_OctetCountTerminat : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nSDB_OctetCountTerminat(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nSDB_OctetCountTerminat(setDefaultInt());
    }
 
    if ( ui.getSkipTag(32) == 0 ) {
        intconversed = validateInt(ui, 32);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 8_SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1037)    << "]"
            		 << " [ UDR DROP ] [ SDB_OctetCountOrigin : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nSDB_OctetCountOrigin(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nSDB_OctetCountOrigin(setDefaultInt());
    }

    if ( ui.getSkipTag(33) == 0 ) {
        intconversed = validateInt(ui, 33);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 9_SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1038) << "]"
    		         << " [ UDR DROP ] [ NumOfSDBsTerminat : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nNumOfSDBsTerminat(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nNumOfSDBsTerminat(setDefaultInt());
    }

    if ( ui.getSkipTag(34) == 0 ) {
        intconversed = validateInt(ui, 34);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 10SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1039) << "]"
     		         << " [ UDR DROP ] [ NumOfSDBsOrigin : Not digit ]"   << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nNumOfSDBsOrigin(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nNumOfSDBsOrigin(setDefaultInt());
    }

    if ( ui.getSkipTag(35) == 0 ) {
        intconversed = validateInt(ui, 35);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 11SessionActivity ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1040)     << "]"
      		         << " [ UDR DROP ] [ NumOfPPPbytesReceived : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemG.put_nNumOfPPPbytesReceived(intconversed); 
    } else {
        stConvUdr[index].stItemG.put_nNumOfPPPbytesReceived(setDefaultInt());
    }

    //////////// __SpecialBillingInstruction

    if ( ui.getSkipTag(36) == 0 ) {
        intconversed = validateInt(ui, 36);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ __SpecialBillingInstruction ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1041)  << "]"
   		             << " [ UDR DROP ] [ AlternateBillingId : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemH.put_nAlternateBillingId(intconversed); 
    } else {
        stConvUdr[index].stItemH.put_nAlternateBillingId(setDefaultInt());
    }

    //////////// __QoS

    if ( ui.getSkipTag(37) == 0 ) {
        intconversed = validateInt(ui, 37);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_QoS ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1042) << "]"
    		         << " [ UDR DROP ] [ IP_QoS : Not digit ]"            << endl;
            return FALSE;
        }
        stConvUdr[index].stItemI.put_nIP_QoS(intconversed); 
    } else {
        stConvUdr[index].stItemI.put_nIP_QoS(setDefaultInt());
    }

    if ( ui.getSkipTag(38) == 0 ) {
        uintconversed = validateIp(ui, 38);
        if ( uintconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 2_QoS ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1043) 		 << "]" 
     		         << " [ UDR DROP ] [ IP_NetworkProviderID : Invalid Fotmat ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemI.put_nInterconnectIP_NetworkProviderID(uintconversed); 
    } else {
        stConvUdr[index].stItemI.put_nInterconnectIP_NetworkProviderID(setDefaultUInt());
    }

    if ( ui.getSkipTag(39) == 0 ) {
        intconversed = validateInt(ui, 39);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 3_QoS ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1044)    << "]"
   		             << " [ UDR DROP ] [ IP_NetworkServiceQoS : Not digit ]" << endl;
            return FALSE;
        }
        stConvUdr[index].stItemI.put_nInterconnectIP_NetworkServiceQoS(intconversed); 
    } else {
        stConvUdr[index].stItemI.put_nInterconnectIP_NetworkServiceQoS(setDefaultInt());
    }

    if ( ui.getSkipTag(40) == 0 ) {
        intconversed = validateInt(ui, 40);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 4_QoS ] ");
            *clsLogM << "LOG : Error No = [" << clsLogM->ErrorCode(1045) << "]"
         		     << " [ UDR DROP ] [ AirlinkQoS : Not digit ]"        << endl;
            return FALSE;
        }
        stConvUdr[index].stItemI.put_nAirlinkQoS(intconversed); 
    } else {
        stConvUdr[index].stItemI.put_nAirlinkQoS(setDefaultInt());
    }

    //////////// __AirlinkRecSpecParam
    
    if ( ui.getSkipTag(41) == 0 ) {
        intconversed = validateInt(ui, 41);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 1_AirlinkRecSpecParam ] ");
            return FALSE;
        }
        stConvUdr[index].stItemY.put_nAirlinkRecType(intconversed); 
    } else {
        stConvUdr[index].stItemY.put_nAirlinkRecType(setDefaultInt());
    }

    if ( ui.getSkipTag(42) == 0 ) {
        intconversed = validateInt(ui, 42);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 2_AirlinkRecSpecParam ] ");
            return FALSE;
        }
        stConvUdr[index].stItemY.put_nRP_SessionID(intconversed); 
    } else {
        stConvUdr[index].stItemY.put_nRP_SessionID(setDefaultInt());
    }

    if ( ui.getSkipTag(43) == 0 ) {
        intconversed = validateInt(ui, 43);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 3_AirlinkRecSpecParam ] ");
            return FALSE;
        }
        stConvUdr[index].stItemY.put_nAirlinkSeqNum(intconversed); 
    } else {
        stConvUdr[index].stItemY.put_nAirlinkSeqNum(setDefaultInt());
    }

    if ( ui.getSkipTag(44) == 0 ) {
        intconversed = validateInt(ui, 44);
        if ( intconversed == -2 ) {   
            writeDropLog("[ UDR DROP ] [ 4_AirlinkRecSpecParam ] ");
            return FALSE;
        }
        stConvUdr[index].stItemY.put_nMobileOrigin_TerminatID(intconversed); 
    } else {
        stConvUdr[index].stItemY.put_nMobileOrigin_TerminatID(setDefaultInt());
    }
    
    //////////// __Container
    
    if ( ui.getSkipTag(45) == 0 ) {
        // UDR.hpp에서 stConfigUdr.nStringLen이 0으로 설정되어 string size를 알 수 없음으로
        // LEN_CONTAINER 사용 
        if ( strlen(ui.getItem(45)) == LEN_CONTAINER ) {
            stConvUdr[index].stItemZ.put_strContainer(validateStr(ui, 44));
        } else {
            writeDropLog("[ UDR DROP ] [ __Container ] ");
            return FALSE;
        }
    } else {
        char container[LEN_CONTAINER];
        memset(container, '*', sizeof(container));
        stConvUdr[index].stItemZ.put_strContainer(container);
    }

    ///// set udr sequence
    updateUdrSeq();
    stConvUdr[index].stUdrHeader.put_nUdrSeqNo(getUdrSeq());

    ///// added 00.9.30 test
//    stConvUdr[index].stUdrHeader.put_nUdrSeqNo(getRadiusType());

    return TRUE;
}

/////// deplicated ///////
void ConvUdr::setCount(unsigned int total) {

    m_udrtotalcnt = total;
}

void ConvUdr::setCount(const char* type) {

    if (memcmp(type, "T", 1) == 0) {    
        m_udrtotalcnt++;
    } else if (memcmp(type, "P", 1) == 0) {
        m_udrprocesscnt++;
    } else if (memcmp(type, "D", 1) == 0) {
        m_udrdropcnt++;
    } else {
/**
        if (DEBUG) {
            cout << "unknown count type : " << type << endl;
        }
**/
    }
}

unsigned int ConvUdr::getCount(const char* type) const {

    if (memcmp(type, "T", 1) == 0) {
        return m_udrtotalcnt;
    } else if (memcmp(type, "P", 1) == 0) {
        return m_udrprocesscnt;
    } else if (memcmp(type, "D", 1) == 0) {
        return m_udrdropcnt;
    } else {
/**
        if (DEBUG) {
            cout << "unknown count type : " << type << endl;
        }
**/
        return -2;
    }
}

void ConvUdr::print(void) {
    
    for (int index = 0; index < UDR_REC_CNT; index++) {
        cout << ">>>>>>>>>>>>>>>> Item Convert >>>>>>>>>>>>>>>> " << index + 1 << " <<<<" << endl;
        cout << "==  stConvUdr[" << index << "]" << " size : " << sizeof(stConvUdr[index]) << endl;

        cout << "> UdrHeader" << endl;
        cout << stConvUdr[index].stUdrHeader.get_nUdrType()     << endl; 
        cout << stConvUdr[index].stUdrHeader.get_nUdrFileSeq()  << endl; 
        cout << stConvUdr[index].stUdrHeader.get_nUdrSeqNo()    << endl; 
        cout << stConvUdr[index].stUdrHeader.get_nUdrStatus()   << endl; 
        cout << stConvUdr[index].stUdrHeader.get_nServiceType() << endl; 
        cout << stConvUdr[index].stUdrHeader.get_nSubsType()    << endl; 
         
        cout << stConvUdr[index].stUdrHeader.get_nUdrType() << endl;
        cout << stConvUdr[index].stUdrHeader.get_nUdrType() << endl;
        cout << stConvUdr[index].stUdrHeader.get_nUdrType() << endl;

        cout << stConvUdr[index].stUdrHeader.get_nServiceProvider() << endl;
        cout << stConvUdr[index].stUdrHeader.get_nMin1() << endl;
        cout << stConvUdr[index].stUdrHeader.get_nMin2() << endl;

        cout << "> MobileIdenfier" << endl;
    
        cout << stConvUdr[index].stItemA.get_unMsId() << endl;
        
        cout << "> UserIdenfier" << endl;
    
        cout << stConvUdr[index].stItemB.get_nIpAddr() << endl;
        cout << stConvUdr[index].stItemB.get_strNAI() << endl;
    
        cout << "> SessionIdenfier" << endl;
    
        cout << stConvUdr[index].stItemC.get_strAccountSessionID() << endl;
        cout << stConvUdr[index].stItemC.get_strCorrelationID() << endl;
    
        cout << "> InfraIdenfier" << endl;
    
        cout << stConvUdr[index].stItemD.get_nMIP_HomeAgent() << endl;
        cout << stConvUdr[index].stItemD.get_nPDSN_FA_Addr() << endl;
        cout << stConvUdr[index].stItemD.get_nServing_PCF() << endl;
        cout << stConvUdr[index].stItemD.get_strBS_MSC_ID() << endl;
    
        cout << "> ZoneIdenfier" << endl;
    
        cout << stConvUdr[index].stItemE.get_nUserZone() << endl;
    
        cout << "> SessionStatus" << endl;
    
        cout << stConvUdr[index].stItemF.get_nForwardMUXOpt() << endl;
        cout << stConvUdr[index].stItemF.get_nReverseMUXOpt() << endl;
        cout << stConvUdr[index].stItemF.get_nForward_Fund_Rate() << endl;
        cout << stConvUdr[index].stItemF.get_nReverse_Fund_Rate() << endl;
        cout << stConvUdr[index].stItemF.get_nServiceOpt() << endl;     
        cout << stConvUdr[index].stItemF.get_nForwardTrafficType() << endl;
        cout << stConvUdr[index].stItemF.get_nReverseTrafficType() << endl;
        cout << stConvUdr[index].stItemF.get_nFundFrameSize() << endl;     
        cout << stConvUdr[index].stItemF.get_nForwardFundamentalRC() << endl;
        cout << stConvUdr[index].stItemF.get_nReverseFundamentalRC() << endl;
        cout << stConvUdr[index].stItemF.get_nIP_Technology() << endl;      
        cout << stConvUdr[index].stItemF.get_nCompulsoryTunnelInd() << endl;
        cout << stConvUdr[index].stItemF.get_nReleaseIndicator() << endl;  
    
        cout << "> SessionActivity" << endl;
    
        cout << stConvUdr[index].stItemG.get_nDataOctetCntTerminat() << endl;
        cout << stConvUdr[index].stItemG.get_nDataOctetCntOrigin() << endl;   
        cout << stConvUdr[index].stItemG.get_nBadPPPframeCnt() << endl;       
        cout << stConvUdr[index].stItemG.get_nStartEventTime() << endl;       
        cout << stConvUdr[index].stItemG.get_nEndEventTime() << endl;         
        cout << stConvUdr[index].stItemG.get_nActiveTime() << endl;           
        cout << stConvUdr[index].stItemG.get_nNumOfActiveTransitions() << endl; 
        cout << stConvUdr[index].stItemG.get_nSDB_OctetCountTerminat() << endl;
        cout << stConvUdr[index].stItemG.get_nSDB_OctetCountOrigin() << endl;  
        cout << stConvUdr[index].stItemG.get_nNumOfSDBsTerminat() << endl;     
        cout << stConvUdr[index].stItemG.get_nNumOfSDBsOrigin() << endl;      
        cout << stConvUdr[index].stItemG.get_nNumOfPPPbytesReceived() << endl;
    
        cout << "> SpecialBillingInstruction" << endl;
    
        cout << stConvUdr[index].stItemH.get_nAlternateBillingId() << endl;
    
        cout << "> QoS" << endl;
    
        cout << stConvUdr[index].stItemI.get_nIP_QoS() << endl;
        cout << stConvUdr[index].stItemI.get_nInterconnectIP_NetworkProviderID() << endl;
        cout << stConvUdr[index].stItemI.get_nInterconnectIP_NetworkServiceQoS() << endl;
        cout << stConvUdr[index].stItemI.get_nAirlinkQoS() << endl;                      
    
        cout << "> AirlinkRecSpecParam" << endl;
    
        cout << stConvUdr[index].stItemY.get_nAirlinkRecType() << endl;         
        cout << stConvUdr[index].stItemY.get_nRP_SessionID() << endl;           
        cout << stConvUdr[index].stItemY.get_nAirlinkSeqNum() << endl;          
        cout << stConvUdr[index].stItemY.get_nMobileOrigin_TerminatID() << endl;
    
        cout << "> Container" << endl;
    
        cout << stConvUdr[index].stItemZ.get_strContainer() << endl;
    } // end of for
} 


void ConvUdr::getByIndex(SendBuffer& sbuf, int index) const {

    memcpy((char*)&sbuf, (char*)&stConvUdr[index], sizeof(sbuf));
}


/////// deplicated ///////
void ConvUdr::read(void) {

    fstream fin;

    const char* infile = "UDRDATA.1";

    fin.open(infile, ios::in);

    if (!fin.good()) {
        cout << "Can't open " << infile << " file." << endl;
        exit(0);
    }
    
    fin.seekg(0, ios::beg);
    fin.read(reinterpret_cast<char*>(&m_rdbuf), sizeof(m_rdbuf));
    fin.close();
}


/////// deplicated after 00.9.30 ///////

void ConvUdr::putRadiusType(unsigned int rtype) {
    m_radiustype = rtype;
}

void ConvUdr::putProcessType(unsigned int procType) {
	m_processType = procType;
}
	
void ConvUdr::putProcessSN(unsigned int procSN) {
	m_processSN = procSN;
}

void ConvUdr::putProcessRemark(unsigned int procRemark) {
	m_processRemark = procRemark;
}

unsigned int ConvUdr::getProcessType(void) {
    return m_processType;
}

unsigned int ConvUdr::getProcessSN(void) {
    return m_processSN;
}

unsigned int ConvUdr::getProcessRemark(void) {
    return m_processRemark;
}


/////// deplicated after 00.9.30 ///////
int ConvUdr::getRadiusType(void) {
    return m_radiustype;
}

void ConvUdr::setFileSeq(unsigned int fseq) {
    m_fileseq = fseq;
}

unsigned int ConvUdr::getFileSeq(void) {
    return m_fileseq;
}

void ConvUdr::setUdrSeq(int useq) {
    m_udrseq = useq;
}

void ConvUdr::updateUdrSeq(void) {
    m_udrseq += 1;
}

int ConvUdr::getUdrSeq(void) {
    return m_udrseq;
}

void ConvUdr::putXmlStr(const char* xstr) {
    memcpy((char*)&m_rdbuf, xstr, sizeof(m_rdbuf));
}

/////// deplicated ///////
int ConvUdr::writeSeqStatusLog() {

    fstream fout;
    const char* outfile = "/SVC1/LOG/parser_fseq_useq.log";
    
    fout.open(outfile, ios::in|ios::out|ios::trunc);

    if (!fout.good()) {
        cout << "Can't open " << outfile << " file." << endl;
        return -1;
    }

    fout << getFileSeq() << endl;
    fout << getUdrSeq() << endl;
   
    fout.close();
}

/////// deplicated ///////
int ConvUdr::readSeqStatusLog() {

    fstream fin;

    const char* infile = "/SVC1/LOG/parser_fseq_useq.log";
    unsigned int fseq = 0;    
    int useq = 0;
    
    fin.open(infile, ios::in|ios::out|ios::trunc);

    if (!fin.good()) {
        cout << "Can't open " << infile << " file." << endl;
        return -1;
    }

    while (!fin.eof() && !fin.fail()) {
        fin >> fseq >> useq;
    }
    
    setFileSeq(fseq);
    setUdrSeq(useq);

    fin.close();
}

void ConvUdr::readQueue(ReadBuffer& rb) {

//    putRadiusType(rb.radiustype);     // added 00.9.30 test 
	putProcessType(rb.processType);	
	putProcessSN(rb.processSN);	
	putProcessRemark(rb.processRemark);	
    setFileSeq(rb.fileseqno);
    putXmlStr(rb.xmlstring);

}

void ConvUdr::writeReadLog(const char* str) {
    mReadLog->write(str);
}

void ConvUdr::writeSendLog(const char* str) {
    mSendLog->write(str);
}

void ConvUdr::writeDropLog(const char* str) {
    mDropLog->write(str);
}

void ConvUdr::writeDropLog(const int list) {
    
    char strbuf[20];
    
    memset(strbuf, 0x00, sizeof(strbuf));
    sprintf(strbuf, "%d", list);
    mDropLog->write(strbuf);
}

void ConvUdr::writeSendLog(const int list) {
    
    char strbuf[20];
    
    memset(strbuf, 0x00, sizeof(strbuf));
    sprintf(strbuf, "%d", list);
    mSendLog->write(strbuf);
}

void ConvUdr::writeReadLog(const int list) {
    
    char strbuf[20];
    
    memset(strbuf, 0x00, sizeof(strbuf));
    sprintf(strbuf, "%d", list);
    mReadLog->write(strbuf);
}

/////// deplicated ///////
void ConvUdr::writeLog(const char* function, const char* str) {

    fstream fout;
    
    const char* outfile0  = "../log/readudr.log";
    const char* outfile1  = "../log/dropudr.log";
    const char* outfile2  = "../log/sendudr.log";
    
    if ( memcmp(function, "READ", 4) == 0 ) {
  	fout.open(outfile0,  ios::out | ios::app );
        if (!fout.good()) {
            cout << "Can't open " << outfile0 << " file." << endl;
        }
    } else if ( memcmp(function, "DROP", 4) == 0 ) {
        fout.open(outfile1,  ios::out | ios::app );
        if (!fout.good()) {
            cout << "Can't open " << outfile1 << " file." << endl;
        }
    } else if ( memcmp(function, "SEND", 4) == 0 ) {
        fout.open(outfile2,  ios::out | ios::app );
        if (!fout.good()) {
            cout << "Can't open " << outfile2<< " file." << endl;
        }
    } else {
        cout << "undefined function" << function << endl;
    }

    memcpy(reinterpret_cast<char*>(&m_wtbuf), str, strlen(str));

    fout.seekp(ios::cur);
    fout.write(reinterpret_cast<char*>(&m_wtbuf), strlen(str));        
    memset(reinterpret_cast<char*>(&m_wtbuf), 0x00, sizeof(m_wtbuf));

    fout.close();

/*****************                
    int onerecsize  = sizeof(stConvUdr[0]);
    int recsperfile = OUT_FILE_SIZE / onerecsize;
    int reminder = UDR_REC_CNT;
    int k = 0;

    for (int index = 0; index < UDR_REC_CNT; index+=recsperfile) {
        
        if (reminder < recsperfile) {
            memset((char*)&m_wtbuf, ' ', sizeof(m_wtbuf));
            memcpy((char*)&m_wtbuf, (char*)&stConvUdr[index], onerecsize*reminder);
            fout[k].seekp(ios::cur);
            fout[k].write(reinterpret_cast<char*>(&m_wtbuf), sizeof(m_wtbuf));        
            cout << "one mineral is extracted..." << endl;
            break;
        }
        memcpy((char*)&m_wtbuf, (char*)&stConvUdr[index], onerecsize*recsperfile);
        fout[k].seekp(ios::cur);
        fout[k].write(reinterpret_cast<char*>(&m_wtbuf), sizeof(m_wtbuf));        
        k++;
        reminder = reminder - recsperfile;
    }
*****************/    
}    

bool ConvUdr::convert(void) {

    XString raw(m_rdbuf);
    char logbuf[IN_FILE_SIZE+80];

    if ( raw.parse(*this) == FALSE ) {

        memset(logbuf,0x00,sizeof(logbuf));
        sprintf(logbuf, "[ UDR DROP ] [ FSEQ %d ]\n\n", getFileSeq());
        writeDropLog(logbuf);
        writeDropLog(raw.c_str());
        writeDropLog("\n\n");
        //return FALSE;
    }
    
    //printf("convert end\n");
    return TRUE;
}

void SendBuffer::display(void) {
    
    cout << "==SEND UDR===================INFO=====" << endl;

    cout << "HD : " << "[ " << stConvUdr.stUdrHeader.get_nUdrType()     << " ] "
                    << "[ " << stConvUdr.stUdrHeader.get_nUdrFileSeq()  << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nUdrSeqNo()    << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nUdrStatus()   << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nServiceType() << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nSubsType()    << " ] " << endl; 
                           
    cout << "MI : " << "[ " << stConvUdr.stUdrHeader.get_nServiceProvider() << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nMin1()            << " ] " 
                    << "[ " << stConvUdr.stUdrHeader.get_nMin2()            << " ] " << endl;

    cout << "A  : " << "[ " << stConvUdr.stItemA.get_unMsId() << " ]" << endl;
    
    cout << "B  : " << "[ " << stConvUdr.stItemB.get_nIpAddr() << " ]" 
                    << "[ " << stConvUdr.stItemB.get_strNAI()  << " ]" << endl;

    cout << "C  : " << "[ " << stConvUdr.stItemC.get_strAccountSessionID() << " ]" 
                    << "[ " << stConvUdr.stItemC.get_strCorrelationID()    << " ]" << endl;

    cout << "D  : " << "[ " << stConvUdr.stItemD.get_nMIP_HomeAgent() << " ]"
                    << "[ " << stConvUdr.stItemD.get_nPDSN_FA_Addr()  << " ]" 
                    << "[ " << stConvUdr.stItemD.get_nServing_PCF()   << " ]" 
                    << "[ " << stConvUdr.stItemD.get_strBS_MSC_ID()   << " ]" << endl;

    cout << "E  : " << "[ " << stConvUdr.stItemE.get_nUserZone() << " ]" << endl;

    cout << "F  : " << "[ " << stConvUdr.stItemF.get_nForwardMUXOpt()        << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nReverseMUXOpt()        << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nForward_Fund_Rate()    << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nReverse_Fund_Rate()    << " ]"
                    << "[ " << stConvUdr.stItemF.get_nServiceOpt()           << " ]"
                    << "[ " << stConvUdr.stItemF.get_nForwardTrafficType()   << " ]"
                    << "[ " << stConvUdr.stItemF.get_nReverseTrafficType()   << " ]" << endl;
    cout << "     " << "[ " << stConvUdr.stItemF.get_nFundFrameSize()        << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nForwardFundamentalRC() << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nReverseFundamentalRC() << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nIP_Technology()        << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nCompulsoryTunnelInd()  << " ]" 
                    << "[ " << stConvUdr.stItemF.get_nReleaseIndicator()     << " ]" << endl;

    cout << "G  : " << "[ " << stConvUdr.stItemG.get_nDataOctetCntTerminat()   << " ]"
                    << "[ " << stConvUdr.stItemG.get_nDataOctetCntOrigin()     << " ]"
                    << "[ " << stConvUdr.stItemG.get_nBadPPPframeCnt()         << " ]"
                    << "[ " << stConvUdr.stItemG.get_nStartEventTime()         << " ]"
                    << "[ " << stConvUdr.stItemG.get_nEndEventTime()           << " ]"
                    << "[ " << stConvUdr.stItemG.get_nActiveTime()             << " ]" << endl;
    cout << "     " << "[ " << stConvUdr.stItemG.get_nNumOfActiveTransitions() << " ]"
                    << "[ " << stConvUdr.stItemG.get_nSDB_OctetCountTerminat() << " ]"
                    << "[ " << stConvUdr.stItemG.get_nSDB_OctetCountOrigin()   << " ]"
                    << "[ " << stConvUdr.stItemG.get_nNumOfSDBsTerminat()      << " ]"
                    << "[ " << stConvUdr.stItemG.get_nNumOfSDBsOrigin()        << " ]"
                    << "[ " << stConvUdr.stItemG.get_nNumOfPPPbytesReceived()  << " ]" << endl;
                                                                               
    cout << "H  : " << "[ " << stConvUdr.stItemH.get_nAlternateBillingId() << " ]" << endl;

    cout << "I  : " << "[ " << stConvUdr.stItemI.get_nIP_QoS()                           << " ]" 
                    << "[ " << stConvUdr.stItemI.get_nInterconnectIP_NetworkProviderID() << " ]" 
                    << "[ " << stConvUdr.stItemI.get_nInterconnectIP_NetworkServiceQoS() << " ]" 
                    << "[ " << stConvUdr.stItemI.get_nAirlinkQoS()                       << " ]" << endl;                      
        
    cout << "Y  : " << "[ " << stConvUdr.stItemY.get_nAirlinkRecType()          << " ]"
                    << "[ " << stConvUdr.stItemY.get_nRP_SessionID()            << " ]"           
                    << "[ " << stConvUdr.stItemY.get_nAirlinkSeqNum()           << " ]"
                    << "[ " << stConvUdr.stItemY.get_nMobileOrigin_TerminatID() << " ]" << endl;
 
    cout << "Z  : " << "[ " << stConvUdr.stItemZ.get_strContainer() << " ]" << endl;
}

Log::Log() { }

Log::~Log() {
    delete m_date;
    delete m_logfname;
}

Log::Log(const char* fname) {

	memset(mDropLog, 0x00, sizeof(mDropLog));
	sprintf(mDropLog, "/SVC%d/LOG/parser_drop.log", serviceId); 

    m_date = new ParserDateTime;	     

    char datebuf[m_date->lendate];

    memset(datebuf, 0x00, sizeof(datebuf));    
    memcpy(datebuf, m_date->get_date(), strlen(m_date->get_date()));

    m_flen = strlen(fname) + 1 + m_date->lendate;    // fname, dot, date
    m_logfname = new char[m_flen];
    memset(m_logfname, 0x00, m_flen);
    sprintf(m_logfname, "%s.%s", fname, datebuf);
}

int Log::write(const char* str) {

	get_LogFileName();
    m_fout.open(m_logfname, ios::out | ios::app);

//    if (!m_fout.good()) {
    if (!m_fout) {
		*clsLogM << "Error No = [" << clsLogM->ErrorCode(1051) << "] LOG : Can't open Drop Log file : [" << m_logfname << "]" << endl;
//          *clsLogM << "LOG : Can't open writelogfile : " << m_logfname << endl;
        return -2;  
    }

    if (strlen(str) > sizeof(m_wtbuf)) {
        cout << "write buffer overflow(" << sizeof(m_wtbuf) << ")" << endl;
    }
    memcpy(reinterpret_cast<char*>(&m_wtbuf), str, strlen(str));

    m_fout.seekp(ios::cur);
    m_fout.write(reinterpret_cast<char*>(&m_wtbuf), strlen(str));        
    memset(reinterpret_cast<char*>(&m_wtbuf), 0x00, sizeof(m_wtbuf));

    m_fout.close();  	
    return 0;
}


void Log::get_LogFileName()
{
    char datebuf[m_date->lendate];

    memset(datebuf, 0x00, sizeof(datebuf));    
    memcpy(datebuf, m_date->get_date(), strlen(m_date->get_date()));

    memset(m_logfname, 0x00, m_flen);
    sprintf(m_logfname, "%s.%s", mDropLog, datebuf);
}
