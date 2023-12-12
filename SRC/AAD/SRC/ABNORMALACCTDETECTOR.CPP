/*
 * $Log: AbnormalAcctDetector.cpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * ...
 * Revision 0.9.1  me
 * ...
 */

#include "AbnormalAcctDetector.hpp"
#include <iostream.h>
namespace aad {

AbnormalAcctDetector::AbnormalAcctDetector() { }

AbnormalAcctDetector::~AbnormalAcctDetector() { }

void AbnormalAcctDetector::loadUDRManager(UDRManager* um) {
    _UDRManager = um;
}

UDRManager* AbnormalAcctDetector::getHandleOfUDRManager() const {
    return _UDRManager;
}

bool AbnormalAcctDetector::checkAbnormal(UDRManager* um, float baseText, float baseOrigin, float baseMultiMedia) {

    loadUDRManager(um);
    // 144.0 * 10(144.0 --> 1440) * 1024(Kbps --> bps) / 10 / 8(bit --> byte)
    const int TEXTTHRESHOLD   = baseText       * 10 * 1024 / 10 / 8; // 8Kbps  --  defalut value 
    const int ORIGINTHRESHOLD = baseOrigin     * 10 * 1024 / 10 / 8; // 9.6Kbps 
    const int MULTITHRESHOLD  = baseMultiMedia * 10 * 1024 / 10 / 8; // 144Kbps

#ifdef _DEBUG_
    cout << "TEXTTHRESHOLD   " << TEXTTHRESHOLD   << endl;
    cout << "ORIGINTHRESHOLD " << ORIGINTHRESHOLD << endl;
    cout << "MULTITHRESHOLD  " << MULTITHRESHOLD  << endl;

    cout << "checkAbnormal " << endl;
    getHandleOfUDRManager()->print();
    cout << "NAI " << getHandleOfUDRManager()->getNAI() << endl;
    cout << "MUX " << getHandleOfUDRManager()->getForwardMUXOption() << endl;
    cout << "OCT " << getHandleOfUDRManager()->getDataOctetCountForTerminating() << endl;
    cout << "OCO " << getHandleOfUDRManager()->getDataOctetCountForOriginating() << endl;
    cout << "ACT " << getHandleOfUDRManager()->getActiveTime() << endl;
    cout << "SPT " << getHandleOfUDRManager()->getSpeedForTerminating() << endl;
    cout << "SPO " << getHandleOfUDRManager()->getSpeedForOriginating() << endl;
#endif

    if (isNtopService() == true) {

        if (1 == getHandleOfUDRManager()->getForwardMUXOption()) {  // n-Top 서비스 : Text 

            if (getHandleOfUDRManager()->getSpeedForTerminating() > TEXTTHRESHOLD ||
                getHandleOfUDRManager()->getSpeedForOriginating() > ORIGINTHRESHOLD ) {
                return true;
            }

        } else if (2337 == getHandleOfUDRManager()->getForwardMUXOption()) {  // n-Top 서비스 : Multimedia

            if (getHandleOfUDRManager()->getSpeedForTerminating() > MULTITHRESHOLD ||
                getHandleOfUDRManager()->getSpeedForOriginating() > ORIGINTHRESHOLD ) {
                return true;
            }
        }

    } else {  // non n-Top 서비스   

        if (getHandleOfUDRManager()->getSpeedForTerminating() > MULTITHRESHOLD ||
            getHandleOfUDRManager()->getSpeedForOriginating() > ORIGINTHRESHOLD ) {
            return true;
        }
    }

    return false;
}

bool AbnormalAcctDetector::isNtopService() const {

    string NAI_011_0("sktelecom@wap95c.n-top.com");  // warning !! hard coded !!
    string NAI_017_0("shinsegi@wap95c.n-top.com");
    string NAI_017_1("power017@itouch017.com");

/*****
    string NAI(getHandleOfUDRManager()->getNAI());
    
    if (NAI.find(NAI_011_0) != string::npos) {
    	return true;
    } else if (NAI.find(NAI_017_0) != string::npos) {
    	return true;
    } else if (NAI.find(NAI_017_1) != string::npos) {    	
    	return true;
    } else {
    	return false;
    }
*****/

    if ( getHandleOfUDRManager()->getNAI() >= NAI_011_0 ||
         getHandleOfUDRManager()->getNAI() >= NAI_017_0 ||
         getHandleOfUDRManager()->getNAI() >= NAI_017_1 ) {

        return true;
    } else {
        return false;
    }
}

} // end of namespace aad



