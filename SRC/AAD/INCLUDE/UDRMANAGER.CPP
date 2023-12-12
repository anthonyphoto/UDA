/*
 * $Log: UDRManager.cpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * ...
 * Revision 0.9.1  me
 * ...
 */


#include "UDRManager.hpp"

#include <iostream.h>

namespace aad {

UDRManager::UDRManager() {}

UDRManager::~UDRManager() {}

bool UDRManager::checkAbnormal() {

    cout << "checkAbnormal " << endl;

    print();

    int i = 1;
    if ( i == 1) 
        return true;
    else 
        return false;
}

string UDRManager::getNAI() {

    UDRManager::iterator beg = this->begin();

    advance(beg, 2);

    return *beg;
}

void UDRManager::print() {

    UDRManager::iterator beg = this->begin();
    UDRManager::iterator end = this->end();

    while (beg != end) {
        cout << *beg++ << ", ";
    }
    cout << endl;
}

} // end of namesapce add





