/*
 * $Log: UdrReader.hpp $
 * Revision 0.9.0  2001/05/24 17:17:55  dongcheol
 * ...
 * Revision 0.9.1  ...
 * ...
 */

#ifndef _UDRREADER_HPP_
#define _UDRREADER_HPP_

#include <memory>
#include "configMGR.hpp"
#include "UDR.hpp"

namespace acc {

class UdrReader : private ConfigMGR {
public:
    /// constructor
    UdrReader();
    /// destructor
    virtual  ~UdrReader();

    int read(UdrFormat& u);

private:
    auto_ptr<UdrFormat> udr_;
};

};  // end of namspace acc

#endif // _UDRREADER_HPP_
