/*
 * $Log: UDRManager.hpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * UDRManager 
 * Revision 0.9.1  me
 * ...
 */


#ifndef _UDRManager_HPP_
#define _UDRManager_HPP_

#include <vector>
#include <string>
#include <fstream.h>

namespace aad {

class UDRManager : public vector<string> {
public:
    explicit  UDRManager();
    virtual  ~UDRManager();

    
    string    getNAI();
    int       getForwardMUXOption();
    int       getDataOctetCountForTerminating();
    int       getDataOctetCountForOriginating();
    string    getStartEventTime();   // maybe useless method
    string    getEndEventTime();     // maybe useless method
    int       getActiveTime();
    int       getSpeedForTerminating();
    int       getSpeedForOriginating();

    void      print();
    void      fprint(fstream& fout);

private:

};

} // end of namesapce add

#endif // _UDRManager_HPP_



