/*
 * $Log: UDRManager.cpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * ...
 * Revision 0.9.1  me
 * ...
 */


#include "UDRManager.hpp"

#include <cstdlib>
#include "CTime.hpp"

namespace aad {

UDRManager::UDRManager() {}

UDRManager::~UDRManager() {}

string UDRManager::getNAI() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 2);

    return *beg;
}

int UDRManager::getForwardMUXOption() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 10);

    return atoi((*beg).c_str());
}

int UDRManager::getDataOctetCountForTerminating() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 24);

    return atoi((*beg).c_str());
}

int UDRManager::getDataOctetCountForOriginating() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 25);

    return atoi((*beg).c_str());
}

string UDRManager::getStartEventTime() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 27);

    return *beg;
}

string UDRManager::getEndEventTime() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 28);

    return *beg;
}

/**  deprecated
int UDRManager::getActiveTime() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 29);

    return atoi((*beg).c_str()) / 10;   //  1/10 段 --> 1段 
}
**/

int UDRManager::getActiveTime() {

    CTime tm;

    char duration[16]; 
    memset(duration, 0x00, 16);

    tm.Duration(duration, const_cast<char*>(getEndEventTime().c_str()), const_cast<char*>(getStartEventTime().c_str()));

    return atoi(duration) / 10;   //  1/10 段 --> 1段 
}

int UDRManager::getSpeedForTerminating() {

    return (0 == getActiveTime()) ? 0 : getDataOctetCountForTerminating() / getActiveTime();
}

int UDRManager::getSpeedForOriginating() {

    return (0 == getActiveTime()) ? 0 : getDataOctetCountForOriginating() / getActiveTime();
}

void UDRManager::print() {

    UDRManager::iterator beg = this->begin();
    UDRManager::iterator end = this->end();

    while (beg != end) {
        cout << *beg++ << ", ";
    }
    cout << endl;
}

template <typename Iterator, typename Stream>
void csv_print(Iterator I, Stream& S) {
    copy(I.begin(), I.end(), ostream_iterator<string>(S, ","));
    S << endl;
}

void UDRManager::fprint(fstream& fout) {
    csv_print(*this, fout);
}

} // end of namesapce add





