/*
 * $Log: CSqlException.hpp $
 * Revision 0.0.1  2001/02/20 10:20:18  designed by dongcheol
 *  sql exception
 * Revision 1.0.1  2001/02/20 14:07:56  dongcheol
 *  added in unexpected_handler
 * Revision 1.0.2  2001/04/16 19:45:17  dongcheol
 *  compile option set -AA for using namespace std
 */

#ifndef _SQLEXCEPTION_HPP_
#define _SQLEXCEPTION_HPP_

//using namespace std;

#include <stdexcept>
#include <string>

class CSQLException : public exception {
public:
             CSQLException(const string& what_arg) throw();
    virtual ~CSQLException() throw() { ; }

    virtual const char* what() const throw() { return str_.data(); }

private:
    string str_;
};

typedef void(*PFV)(); 
//PFV set_unexpected(PFV); 

static void my_unexpected() {
    cout << "CRITICAL EXCEPTION : called unexpected()" << endl;
    throw;      // Rethrow the current exception. 
} 

#endif
