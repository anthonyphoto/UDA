#ifndef __PARSER_H_
#define __PARSER_H_

#define DEBUG 1
#include <time.hpp>
#include <UDR.hpp>
#include <string>




const int  IN_FILE_SIZE  = 32768;   // udr from interface
const int  OUT_FILE_SIZE = 2048; 
const int  UDR_REC_CNT   = 25;      // in one block file
const char START_TAG[]   = "<UDRDOC>";
const char END_TAG[]     = "</UDR>";
const char UDRTYPE[]     = "UDRTYPE";
const char BETWEENTAG[]  = "><";
const char UDR_START[]   = "START";
const char UDR_INTERIM[] = "INTERIM";
const char UDR_STOP[]    = "STOP";
const char UNAUTHENTICATED[] = "unauthenticated";


class CONV_UDR_TYPE {
public:
    UDRHEAD_TYPE    stUdrHeader;
    A_INT_TYPE      stItemA;
    B_TYPE          stItemB;
    C_TYPE          stItemC;
    D_TYPE          stItemD;
    E_TYPE          stItemE;
    F_TYPE          stItemF;
    G_TYPE          stItemG;
    H_TYPE          stItemH;
    I_TYPE          stItemI;
    Y_TYPE          stItemY;
    Z_TYPE          stItemZ;
    LIMIT_INFO		stLimitInfo;
};


class UdrItem {
public:

    /////// constructor, destructor 
	int 	mPos; 
    int		mStartPotion[CNT_UDR_ITEM];
    int     mEndPosition[CNT_UDR_ITEM];
    UdrItem();
    ~UdrItem();

    /////// accessor

    bool        get(const string& tag, string& item) const;
    const char* getTag(int) const;
    const char* getItem(int) const;
    int         getItemLength(int) const;
    int         getItemType(int) const;
    int         maxCapacity(void) const ;
    int         getSkipTag(int item) const;  
 

    /////// operator

    void        put(int i, const string& item);
    void        putSkipTag(int item);
    void        print(void);

private:

    int     mCapacity;
    int     mConfigItemLength[CNT_UDR_ITEM];
    int     mConfigItemType[CNT_UDR_ITEM];
    
    string* mTagSlot;
    string* mItemSlot;
    int     mTagSkip[CNT_UDR_ITEM];
};


class ReadBuffer {
public:
//    unsigned int  radiustype;       // temperary
	unsigned int processType;	
	unsigned int processSN;	
	unsigned int processRemark;	
    unsigned int  fileseqno;
    char xmlstring[IN_FILE_SIZE];   // input file size 32K
    void display(void);
};

class SendBuffer {
public:
    CONV_UDR_TYPE stConvUdr;
    void          display(void);

};

class ParserDateTime : public DateTime { 
public:
    enum datetimelength { lendate = 20, lentime = 20 };
};

class Log {
public:
    
    /////// constuctor, destructor

    Log();    
    Log(const char*);
    ~Log();
    
    /////// operator
    
    int  create(const char*);
    bool read(void);
    int  write(const char*);
	void get_LogFileName();
	
public:

    enum writebuflength { lenbuf = 2048 };

private:

    fstream         m_fout;
    char*           m_logfname;
    int             m_flen;
    char            m_wtbuf[IN_FILE_SIZE+100];
    ParserDateTime* m_date;
    char			mDropLog[100];
};


class ConvUdr {
public:

    /////// constructor, destructor

    ConvUdr();
    ~ConvUdr();

    /////// accessor 

    void         getByIndex(SendBuffer&, int) const;
    unsigned int getCount(const char*) const;
        
    /////// modifier

    void         setCount(unsigned int);
    void         setCount(const char*);
    void         clear(void);

    /////// operator

    void         read(void);
    void         readQueue(ReadBuffer&);
    bool         convert(void);
    void         writeLog(const char*, const char*);
    bool         resolve(UdrItem&, int, const char*);
    const char*  validateStr(const UdrItem&, int);
    int          validateInt(const UdrItem&, int);
    unsigned int validateIp(const UdrItem&, int);
    const char*  setDefaultStr(const UdrItem&, int);
    int          setDefaultInt(void);
    unsigned int setDefaultUInt(void);
    void         print(void);    
    void         writeReadLog(const char*);
    void         writeSendLog(const char*);
    void         writeDropLog(const char*);        
    void         writeReadLog(const int);
    void         writeSendLog(const int);
    void         writeDropLog(const int);        
    unsigned int getFileSeq(void);
    unsigned int getProcessType(void);
	unsigned int getProcessSN(void);    
    unsigned int getProcessRemark(void);
    
    int          getUdrSeq(void);
    int          writeSeqStatusLog(void);
    int          readSeqStatusLog(void);
    void         putRadiusType(unsigned int);
	void 		 putProcessType(unsigned int);
	void 		 putProcessSN(unsigned int);
	void 		 putProcessRemark(unsigned int);
    int          getRadiusType(void);
    int	getServiceProvider(int index)	{ return stConvUdr[index].stUdrHeader.get_nServiceProvider(); }				

protected:

    void         setFileSeq(unsigned int);
    void         putXmlStr(const char*);
    void         setUdrSeq(int);
    void         updateUdrSeq(void);

public:

    Log* mReadLog;
    Log* mDropLog;
    Log* mSendLog;

private:

    CONV_UDR_TYPE stConvUdr[UDR_REC_CNT];   // buffer to send filter
    unsigned int  m_udrtotalcnt;
    unsigned int  m_udrprocesscnt;
    unsigned int  m_udrdropcnt;
    char          m_rdbuf[IN_FILE_SIZE];    // input file size 32K
    char          m_wtbuf[OUT_FILE_SIZE];   // 1K  
    int           m_radiustype;             // Radius Type 00.9.30 Test
  	unsigned int  m_processType;	
	unsigned int  m_processSN;	
	unsigned int  m_processRemark;	  
    unsigned int  m_fileseq;                // Udr Block File Sequence
    int           m_udrseq;                 // Udr Sequence in Block File
};


class XString {

    friend ostream& operator<<(ostream&, const XString&);

public:

    /////// constructor, destructor

    XString();                         // construct empty string ""
    virtual ~XString();                // destructor
    XString(const char*);              // construct from string literal
    XString(const XString& str);       // copy constructor

    /////// assignment

    const XString& operator=(const XString& str); 
    const XString& operator=(const char* s);

    /////// indexing

    char operator[](int) const;

    /////// accessor

    int          find(char) const;
    int 		 find(char, int) const;
    int          find(const XString&) const;
    int          find(const XString&, int) const;
    const char*  udrType(int pos) const;
    XString      substr(int, int) const;    // substring of len chars
    int          length() const;                  
    const char*  c_str() const;
    //int          getPos(void) const; 
    
    /////// modifier

    //void         setPos(int);
    const        XString& operator+=(const XString&);
    
    /////// operator

    void         replace(char, char, int);
    XString      extractItem(const XString&) const; 
    XString      extractItem(const char*) const;
    //bool         extractItem(char*,int);
    bool         parse(ConvUdr&) const;
    unsigned int IPstrtoi(void);
    
private:

    char mpString[32769];
    int   mLength;
    //int   mPos;
    int   mCapacity;
};    


#endif

