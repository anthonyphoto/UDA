/*
 * $Log: TPdsnInfo.hpp $
 * Revision 0.9.0  2001/05/12 09:40:46  dongcheol
 * table id : t_pdsn_info
 * Revision 0.9.1  ...
 *
 */


#ifndef _TPDSNINFO_HPP_
#define _TPDSNINFO_HPP_

#include "CSQLException.hpp"
#include <vector>

namespace acc {

//#define  LEN_PDSN_IP_ADDR   15
//#define  LEN_PDSN_LOC       32

class TPdsnInfo {
public:
    explicit  TPdsnInfo();
    virtual  ~TPdsnInfo();

    void      setPdsnIpAddr(const string&);
    void      setPdsnLoc   (const string&);

    const string&  getPdsnIpAddr() const;
    const string&  getPdsnLoc   () const;

    void     clear();
    void     select(vector<TPdsnInfo>&) throw (CSQLException, bad_exception);

protected:
    void     saveTextOfSQL();

private:
    string  PDSN_IP_ADDR_;
    string  PDSN_LOC_    ;
};          
            
}; // end of namespace acc
#endif   // _TPDSNINFO_HPP_
