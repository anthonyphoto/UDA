/*
 * $Log: BDRManager.hpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * BDR에서 UDR을 추출하는 기능을 관리 
 * Revision 0.9.1  me
 * ...
 */


#ifndef _BDRMANAGER_HPP_
#define _BDRMANAGER_HPP_

#include <string>
#include <fstream.h>

namespace aad {

class BDRManager {
public:
    explicit  BDRManager();
    virtual  ~BDRManager();

    virtual void detect(float, float, float, const int s);

    string       getDate() const;
    string       makeFileName() const;
    string       getPath() const;
    void         setPath(string& s);

private:
    fstream _fout;
    string  _path;

};

} // end of namesapce add

#endif // _BDRMANAGER_HPP_



