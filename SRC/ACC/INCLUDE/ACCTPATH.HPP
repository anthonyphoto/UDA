#ifndef _ACCTPATH_HPP_
#define _ACCTPATH_HPP_

namespace acc {

#ifdef _UDA1_
const char* const PATH_ACCOUNT_OUT = "/SVC1/LOG/ACCT/";
const char* const PATH_ACCOUNT_LOG = "/SVC1/LOG/";
#endif

#ifdef _UDA2_
const char* const PATH_ACCOUNT_OUT = "/SVC1/LOG/ACCT/";
const char* const PATH_ACCOUNT_LOG = "/SVC1/LOG/";
#endif

const char* const PATH_SNMP_LOG = "/home/uda1/SNMP/DATA/";

}; // end of namespace acc

#endif // _ACCTPATH_HPP_
