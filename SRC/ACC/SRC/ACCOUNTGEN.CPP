/*
 * $Log: AccountGen.cpp $
 * Revision 0.9.0  2001/05/09 10:02:15  dongcheol
 * created
 * Revision 0.9.1  me
 * update() modified and stat related function is deprecated
 */


#include <unistd.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <strstream.h>
#include "AccountGen.hpp"
#include "TMetaAction.hpp"
#include "CTime.hpp"
#include "configMGR.hpp"
#include "FileLogWrite.hpp"
#include "TPdsnInfo.hpp"

extern FileLogWrite* flog;
extern int g_stop = 1;

namespace acc {

AccountGen::AccountGen(string& processId = string("")) : pid_(processId), prevTime_(getHour()), fseq_(0), ucount_(0) {
    //prevTime_ = getHour();
    setFileName(makeFileName());
}

AccountGen::~AccountGen() { 
    stopToSubcribe();
}

bool AccountGen::stopToSubcribe() {

    fout_.close();

    if (!fout_) {
        flog->Write("%s : %s", "failure in file close", getFileName().c_str());    
        return false;
    }

    return true;
}

bool AccountGen::readyToSubcribe() {

    fout_.open(getFileName().c_str(), ios::app);

    if (!fout_) {
        flog->Write("%s : %s", "critical error : failure in file open", getFileName().c_str());    
        return false;
    }

    if (fout_.tellp() <= 0) {  // check to create file by other processes
        int fd = fout_.rdbuf()->fd();
        if(lockf(fd, F_TLOCK, 0L) == -1) {
            flog->Write("%s : %s", "critical error : failure in file lock on", getFileName().c_str());    
            return false;
        }

        printHeader();    

        if(lockf(fd, F_ULOCK, 0L) == -1) {
            flog->Write("%s : %s", "critical error : failure in file lock on", getFileName().c_str());    
            return false;
        }
    }

    return true;
}

const string AccountGen::getCurrentTimeStamp() const {

    CTime tm;
    char temp[13]; memset(temp, 0x00, sizeof(temp));
    tm.Time_Display(temp, 7);
    string time(temp);
    return time;
}

const string AccountGen::getHour() const {
    // time format : yyyymmddhhm0   
    string str;
    str += getCurrentTimeStamp().substr(0, 11);
    str += "0";  
    return str;
}

void AccountGen::setProcessId(const string& p) {
    pid_ = p;
}

const string& AccountGen::getProcessId() const {
    return pid_;
}

void AccountGen::setPreviousTime(const string& p) {
    prevTime_ = p;
}

const string& AccountGen::getPreviousTime() const {
    return prevTime_;
}

const string AccountGen::getFileSeq() const {

    char temp[5];
    memset(temp, 0x00, sizeof(temp));
    sprintf(temp, "%04d", fseq_);
    string str(temp);
    return str;
}

const string AccountGen::makeFileName() const {

    string name(PATH_ACCOUNT_OUT);
    name += getHour();
    name += ".";
    name += getFileSeq();
    return name;
}

void AccountGen::setFileName(const string& s) {
    fname_ = s;
}

const string& AccountGen::getFileName() const {
    return fname_;
}

fstream& AccountGen::getFileStream() {
    return fout_;
}

vector<string>& AccountGen::getHeader() {
    return header_;
}

AccountGen::PdsnInfoType& AccountGen::getPdsnInfo() {
    return pdsnInfo_;
}

bool AccountGen::loadMetaInfo() {

    vector<TMetaAction> madata;
    try {
        auto_ptr<TMetaAction> metaaction(new TMetaAction());

        metaaction->setMappingName("ACC0");
        metaaction->setVer("1");
        metaaction->select(madata);

    } catch (bad_alloc& e) {
        flog->Write("%s", e.what());
        return false;
    } catch (CSQLException& e) {
        flog->Write("%s", e.what());
        return false;
    } catch (...) {
        flog->Write("%s", "Caught unexpected exception into loadPdsnInfo");
        // unexpected();
        return false;
    }

    if ( 0 == madata.size() ) {
        flog->Write("%s", "critical : nothing meta info");
        return false;
    }

    vector<TMetaAction>::iterator beg = madata.begin();
    vector<TMetaAction>::iterator end = madata.end();

    while ( beg != end ) {
        getHeader().push_back((*beg++).getOutFieldName());
    }
    return true;
}

bool AccountGen::loadPdsnInfo() {

    vector<TPdsnInfo> data;
    try {
        auto_ptr<TPdsnInfo> pdsnInfo(new TPdsnInfo());
        pdsnInfo->select(data);

    } catch (bad_alloc& e) {
        flog->Write("%s", e.what());
        return false;
    } catch (CSQLException& e) {
        flog->Write("%s", e.what());
        return false;
    } catch (...) {
        flog->Write("%s", "Caught unexpected exception into loadPdsnInfo");
        return false;
    }

    if ( 0 == data.size() ) {
        flog->Write("%s", "critical : nothing pdsn info");
        return false;
    }

    vector<TPdsnInfo>::iterator beg = data.begin();
    vector<TPdsnInfo>::iterator end = data.end();

    while ( beg != end ) {
        getPdsnInfo().insert(PdsnInfoType::value_type(beg->getPdsnIpAddr(), beg->getPdsnLoc()));
        beg++;
    }
    return true;
}

bool AccountGen::update() {

    ucount_++;   // increase by udr reading

    if (ucount_ == COUNT_LIMIT) {   // 2000개마다 File 생성 
        fseq_++;
        ucount_ = 0;
        if ( stopToSubcribe() == true) {
            setFileName(makeFileName());
            return readyToSubcribe();
        } else {
            return false;
        }
    }

    string currTime(getHour());  
    
    if (getPreviousTime() < currTime) {  // 10분마다 파일 생성 
        setPreviousTime(currTime);

        fseq_   = 0;
        ucount_ = 0;

        if ( stopToSubcribe() == true) {
            setFileName(makeFileName());
            return readyToSubcribe();
        } else {
            return false;
        }
    }
    return true;
}

void AccountGen::print() {

    if ( loadPdsnInfo() == false || loadMetaInfo() == false || readyToSubcribe() == false ) {
        return;  // stop
    }

    // loop reading udr
    string pid(getProcessId());  // ACC0100
    int serviceNumber = 0;       // 01 : SVC1  02 : SVC2
    int pdsnNumber    = 0;       // 00 : PDSN0  01 : PDSN1
    serviceNumber = atoi(pid.substr(3, 2).c_str());  
    pdsnNumber    = atoi(pid.substr(5, 2).c_str());  

    auto_ptr<ConfigMGR> filteredUdr(new ConfigMGR(serviceNumber, pdsnNumber)); 

    while (g_stop) {

        UdrFormat udrbuf;
        int error = 0;

         error = filteredUdr->readUdrOfAcct(&udrbuf);
      
        if (0 != error) {

            AccountUDR theAccountUdr(udrbuf, getHeader(), getPdsnInfo());
            theAccountUdr.make();

            int fd = fout_.rdbuf()->fd();
            if(lockf(fd, F_TLOCK, 0L) == -1) {
                flog->Write("%s : %s", "critical error : failure in file lock on", getFileName().c_str());    
                return;
            }
    
            printContents(theAccountUdr);

            if(lockf(fd, F_ULOCK, 0L) == -1) {
                flog->Write("%s : %s", "critical error : failure in file lock on", getFileName().c_str());    
                return;
            }
            sleep(1);
            //flog->Write("%s", "generated");

        } else {
            // flog->Write("%s", "."); // waiting
            sleep(1); 
        }

        if (update() == false) {
            return;   // stop
        }
    } // end of while
}

template <typename Iterator, typename Stream>
void csv_print(Iterator I, Stream& S) {
    copy(I.begin(), I.end(), ostream_iterator<string>(S, ","));
    S << endl;
}

void AccountGen::printContents(AccountUDR& au) {
    csv_print(au, getFileStream());
}

void AccountGen::printHeader() {
    csv_print(getHeader(), getFileStream());
}

}; // end of namespace acc


