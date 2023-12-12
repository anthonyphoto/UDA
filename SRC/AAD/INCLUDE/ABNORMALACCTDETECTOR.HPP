/*
 * $Log: AbnormalAcctDetector.hpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * BDR에서 비정상 과금 UDR을 찾는다
 * Revision 0.9.1  me
 * ...
 */


#ifndef _ABNORMALACCTDETECTOR_HPP_
#define _ABNORMALACCTDETECTOR_HPP_

#include "UDRManager.hpp"

namespace aad {

class AbnormalAcctDetector {
public:
    explicit  AbnormalAcctDetector();
    virtual  ~AbnormalAcctDetector();

    bool         checkAbnormal(UDRManager* um, float, float, float);
    void         loadUDRManager(UDRManager* um);
    UDRManager*  getHandleOfUDRManager() const;
    bool         isNtopService() const;

    void loadConfig();
    void writeLog();

private:
    UDRManager* _UDRManager;
};

} // end of namesapce add

#endif // _ABNORMALACCTDETECTOR_HPP_



