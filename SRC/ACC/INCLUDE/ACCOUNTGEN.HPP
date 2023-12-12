/*
 * $Log: AccountGen.hpp $
 * Revision 0.9.0  2001/05/09 10:02:15  dongcheol
 * created
 * Revision 0.9.1  me
 * 통계관련 기능 not support
 */


#ifndef _ACCOUNTGEN_HPP_
#define _ACCOUNTGEN_HPP_

#include <string>
#include <fstream.h>
#include <vector>
#include <map>
#include "UDR.hpp"
#include "AccountUDR.hpp"
#include "AcctPath.hpp"

namespace acc {

const int COUNT_LIMIT = 2000;   // 생성 File의 최대 udr 수  
/**
UDA Filter에서 Account File 을 생성한다.
*/
class AccountGen {
public:
    /// constructor
    explicit  AccountGen(string& p);
    /// destructor
    virtual  ~AccountGen();
    virtual void print();

private:
    bool            loadMetaInfo();
    bool            loadPdsnInfo();
    void            printHeader();
    void            printContents(AccountUDR& au);
    const string    getCurrentTimeStamp() const;
    const string    getHour() const;
    const string    makeFileName() const;
    bool            readyToSubcribe(); 
    bool            stopToSubcribe();
    bool            update();
    void            setProcessId(const string& p);
    const string&   getProcessId() const;
    void            setPreviousTime(const string& t);
    const string&   getPreviousTime() const;
    void            setFileName(const string& s);
    const string&   getFileName() const;
    fstream&        getFileStream();
    vector<string>& getHeader();
    const string    getFileSeq() const;
public:
    typedef map<string, string, less<string> > PdsnInfoType;
private:
    PdsnInfoType&   getPdsnInfo();

private:
    string          pid_;
    string          prevTime_;
    string          fname_;
    fstream         fout_;
    unsigned int    fseq_;            // file sequence, initail value '0000'
    unsigned int    ucount_;          // UDR count 
    PdsnInfoType    pdsnInfo_;
    vector<string>  header_;          // not support, $File에 Header를 삽입하지 않는다. 
};

class UdrTypeComparison {
public:
    UdrTypeComparison(const string& s) : withWhat_(s) { ; }
    bool operator()(const string& s) { return s == withWhat_; }
private:
    string withWhat_;
};

}; // end of namespace acc
#endif   // _ACCOUNTGEN_HPP_
