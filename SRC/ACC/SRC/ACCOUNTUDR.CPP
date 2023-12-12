/*
 * $Log: AccountUDR.cpp $
 * Revision 0.9.0  2001/05/09 10:02:15  dongcheol
 * ...
 * Revision 0.9.1  ...
 * ...
 */


#include "AccountUDR.hpp"
#include "Ctime.hpp"

namespace acc {

AccountUDR::AccountUDR( UdrFormat& u
                      , vector<string>& m
                      , AccountUDR::AuxInfoType& a ) : udr_(u), header_(m), auxInfo_(a) {
}

AccountUDR::~AccountUDR() { }

AccountUDR::iterator AccountUDR::begin() {
    return values_.begin();
}

AccountUDR::iterator AccountUDR::end() {
    return values_.end();
}

const string AccountUDR::IPToString(const unsigned int& ip) const {

    string ipaddr;

    for (int k(0); k < 32; k+=8) { 
        int temp = 0;
        temp |= (ip << k) >> 24;
        ipaddr += ltoa(temp);
        ipaddr += ".";
    }

    return ipaddr.substr(0, ipaddr.length()-1);
}

const string AccountUDR::UDRTypeToString(const int& type) const{

    string result("");

    switch (type) {
    case 0:
        return result.assign(UDR_START);
    case 1:
        return result.assign(UDR_INTERIM);
    case 2:
        return result.assign(UDR_STOP);
    default:
        return result.assign(UDR_DROP);
    }
}

const string AccountUDR::wrapTimeFormat(const long& time) const {

    DateTime dateTime;

    string s(dateTime.get_DateTimeT((time_t)time));
    string r;
    r += s.substr(0, 4);
    r += "/";
    r += s.substr(4, 2);
    r += "/";
    r += s.substr(6, 2);
    r += " ";
    r += s.substr(8, 2);
    r += ":";
    r += s.substr(10, 2);
    r += ":";
    r += s.substr(12, 2);

    return r;
}

const string AccountUDR::findRasClient(const string& s) {

    AccountUDR::AuxInfoType::iterator found = auxInfo_.find(s);

    if ( found == auxInfo_.end() ) {
        //cout << "Unregitered Ras Client : " << s << endl;
        return s;   // if not found
    } else {
        return found->second;
    }
}

const string AccountUDR::getUdrType() {

    vector<string>::iterator beg = values_.begin();
    advance(beg, 3);   // step 3
    return *beg;
}

const string AccountUDR::getRasClient() {

    return IPToString(udr_.get_ItemD()->get_nPDSN_FA_Addr());
}

void AccountUDR::make() {

    DateTime dateTime;
    /* Date                         */  values_.push_back(dateTime.get_date(1));
    /* Time                         */  values_.push_back(dateTime.get_time(1));
    /* RAS-Client                   */  values_.push_back(findRasClient(IPToString(udr_.get_ItemD()->get_nPDSN_FA_Addr())));
    /* Record-Type                  */  values_.push_back(UDRTypeToString(udr_.get_ItemHead()->get_nUdrType()));
    /* Full-Name                    */  values_.push_back("Unknown");
    /* Auth-Type                    */  values_.push_back("Unknown");
    /* Calling-Station-Id           */  values_.push_back(ltoa(udr_.get_ItemA()->get_unMsId()));    
    /* Framed-IP-Address            */  values_.push_back(IPToString(udr_.get_ItemB()->get_nIpAddr()));    
    /* User-Name                    */  values_.push_back(           udr_.get_ItemB()->get_strNAI () );    
    /* Acct-Session-Id              */  values_.push_back(udr_.get_ItemC()->get_strAccountSessionID());    
    /* Correlation-Id               */  values_.push_back(udr_.get_ItemC()->get_strCorrelationID   ());    
    /* Home-Agent-Address           */  values_.push_back(IPToString(udr_.get_ItemD()->get_nMIP_HomeAgent()));    
    /* NAS-IP-Address               */  values_.push_back(IPToString(udr_.get_ItemD()->get_nPDSN_FA_Addr ()));    
    /* Serving-PCF                  */  values_.push_back(IPToString(udr_.get_ItemD()->get_nServing_PCF  ()));    
    /* BS-MSC-ID                    */  values_.push_back(           udr_.get_ItemD()->get_strBS_MSC_ID  () );    
    /* User-Zone                    */  values_.push_back(ltoa(udr_.get_ItemE()->get_nUserZone()));    
    /* Forward-Mux-Option           */  values_.push_back(ltoa(udr_.get_ItemF()->get_nForwardMUXOpt       ()));    
    /* Reverse-Mux-Option           */  values_.push_back(ltoa(udr_.get_ItemF()->get_nReverseMUXOpt       ()));    
    /* Forward-Fundamental-Rate     */  values_.push_back(ltoa(udr_.get_ItemF()->get_nForward_Fund_Rate   ()));    
    /* Reverse-Fundamental-Rate     */  values_.push_back(ltoa(udr_.get_ItemF()->get_nReverse_Fund_Rate   ()));    
    /* Service-Option               */  values_.push_back(ltoa(udr_.get_ItemF()->get_nServiceOpt          ()));    
    /* Forward-Traffic-Type         */  values_.push_back(ltoa(udr_.get_ItemF()->get_nForwardTrafficType  ()));    
    /* Reverse-Traffic-Type         */  values_.push_back(ltoa(udr_.get_ItemF()->get_nReverseTrafficType  ()));    
    /* Fundamental-Frame-Size       */  values_.push_back(ltoa(udr_.get_ItemF()->get_nFundFrameSize       ()));    
    /* Forward-Fundamental-RC       */  values_.push_back(ltoa(udr_.get_ItemF()->get_nForwardFundamentalRC()));    
    /* Reverse-Fundamental-RC       */  values_.push_back(ltoa(udr_.get_ItemF()->get_nReverseFundamentalRC()));    
    /* IP-Technology                */  values_.push_back(ltoa(udr_.get_ItemF()->get_nIP_Technology       ()));    
    /* Compulsory-Tunnel-Indicator  */  values_.push_back(ltoa(udr_.get_ItemF()->get_nCompulsoryTunnelInd ()));    
    /* Release-Indicator            */  values_.push_back(ltoa(udr_.get_ItemF()->get_nReleaseIndicator    ()));    
    /* Acct-Output-Octets           */  values_.push_back(ltoa(udr_.get_ItemG()->get_nDataOctetCntTerminat  ()));    
    /* Acct-Input-Octets            */  values_.push_back(ltoa(udr_.get_ItemG()->get_nDataOctetCntOrigin    ()));    
    /* Dropped-Octet-Count          */  values_.push_back(ltoa(udr_.get_ItemG()->get_nBadPPPframeCnt        ()));    

    /* Event-Time */
    long jultime = udr_.get_ItemG()->get_nStartEventTime();
    if (0 == jultime) {
        jultime = udr_.get_ItemG()->get_nEndEventTime();   
        values_.push_back(wrapTimeFormat(jultime));    
    } else {
        values_.push_back(wrapTimeFormat(jultime));    
    }

    /* Acct-Session-Time            */  values_.push_back(ltoa(udr_.get_ItemG()->get_nActiveTime            ()));    
    /* Number-of-Active-Transitions */  values_.push_back(ltoa(udr_.get_ItemG()->get_nNumOfActiveTransitions()));    
    /* SDB-Octect-Count-Terminating */  values_.push_back(ltoa(udr_.get_ItemG()->get_nSDB_OctetCountTerminat()));    
    /* SDB-Octect-Count-Originating */  values_.push_back(ltoa(udr_.get_ItemG()->get_nSDB_OctetCountOrigin  ()));    
    /* Number-of-SDBs-Terminating   */  values_.push_back(ltoa(udr_.get_ItemG()->get_nNumOfSDBsTerminat     ()));    
    /* Number-of-SDBs-Originating   */  values_.push_back(ltoa(udr_.get_ItemG()->get_nNumOfSDBsOrigin       ()));    
    /* Number-of-PPP-Bytes-Received */  values_.push_back(ltoa(udr_.get_ItemG()->get_nNumOfPPPbytesReceived ()));    
    /* Alternate-Billing-Identifier */  values_.push_back(ltoa(udr_.get_ItemH()->get_nAlternateBillingId()));    
    /* IP-Quality-of-Service        */  values_.push_back(ltoa      (udr_.get_ItemI()->get_nIP_QoS                          ()));    
    /* Inter-IP-Network-Provider-ID */  values_.push_back(IPToString(udr_.get_ItemI()->get_nInterconnectIP_NetworkProviderID()));    
    /* Inter-IP-Network-Service-QOS */  values_.push_back(ltoa      (udr_.get_ItemI()->get_nInterconnectIP_NetworkServiceQoS()));    
    /* Airlink-Quality-of-Service   */  values_.push_back(ltoa      (udr_.get_ItemI()->get_nAirlinkQoS                      ()));    
    /* Accounting-Container         */  values_.push_back(udr_.get_ItemZ()->get_strContainer());    
}

}; // end of namespace acc
