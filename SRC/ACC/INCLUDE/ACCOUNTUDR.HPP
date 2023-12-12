/*
 * $Log: AccountUDR.hpp $
 * Revision 0.9.0  2001/05/09 10:02:15  dongcheol
 * build 50
 * Revision 0.9.1  2001/05/27 10:21:17  dongcheol
 * add getRasClient method
 */

#ifndef _ACCOUNTUDR_HPP_
#define _ACCOUNTUDR_HPP_

#include <string>
#include <vector>
#include <map>
#include "UDR.hpp"

namespace acc {

const char* const UDR_START    = "Start";
const char* const UDR_INTERIM  = "Interim";
const char* const UDR_STOP     = "Stop";
const char* const UDR_DROP     = "Drop";
const char* const UDR_TOTAL    = "Total";

/**
Filterd UDR managed class.
This is ...
*/
class AccountUDR {
public:
    typedef map<string, string, less<string> > AuxInfoType;

public:
    /// constructor
    AccountUDR(UdrFormat& u, vector<string>& m, AuxInfoType& a);
    /// destructor
    virtual  ~AccountUDR();

    void          make();
    const string  getUdrType();
    const string  getRasClient();

public:
    typedef vector<string>::iterator iterator;
    iterator begin();
    iterator end();

private:
    const string  IPToString(const unsigned int& ip) const;
    const string  UDRTypeToString(const int& t) const;
    const string  wrapTimeFormat(const long& time) const;
    const string  findRasClient(const string& s);

private:
    /// udr format
    UdrFormat&            udr_;
    /// meta action
    vector<string>&       header_;
    vector<string>        values_;
    AuxInfoType&          auxInfo_;
};

}; // end of namespace acc
#endif   // _ACCOUNTUDR_HPP_
