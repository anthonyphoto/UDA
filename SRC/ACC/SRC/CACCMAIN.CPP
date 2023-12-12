/*
 * $Log: CAccMain.cpp $
 * Revision 0.9.0  2001/05/09 10:02:15  dongcheol
 * argv[1] for process id is like "11ACC##" which has to start with same string '11' of 
 * process_id field into t_last_work_info. Because of if or not,
 * will show [TLastWorkInfo] Get_Record ERROR : 1403 --> occurs critical error with eternal looping
 * Revision 0.9.1  ...
 * ...
 */


#include <string>
#include <cstdlib>
#include <memory>
#include "signal.hpp"
#include "dblib.hpp"
#include "AccountGen.hpp"
#include "CTime.hpp"
#include "FileLogWrite.hpp"

const char* const USERNAME = "uda";
const char* const PASSWORD = "uda";

using namespace sig;
using namespace acc;

FileLogWrite* flog;   // later replace this with sigleton

int main(int argc, char* argv[]) {

    init_signal();

    string pid("ACC");
    char temp[3]; memset(temp, 0x00, sizeof(temp));
    sprintf(temp, "%02d", atoi(argv[2])); // service
    pid += temp;
    sprintf(temp, "%02d", atoi(argv[5])); // pdsn
    pid += temp;

    flog = new FileLogWrite(PATH_ACCOUNT_LOG, pid.c_str());  
    auto_ptr<FileLogWrite> log(flog);

/****** deprecated for skt-pms option
    if (argc <= 1) {
        log->Write("Usage:  CAccMain processid[ACC0100] 01:service, 00:pdsn");
        return EXIT_FAILURE;
    }
    string pid(argv[1]);
******/
    log->Write("account generator staring ...");

    auto_ptr<CDblib> dbconn(new CDblib());
    string username(USERNAME);
    string password(PASSWORD);
    dbconn->connectDB(const_cast<char*>(username.c_str()), const_cast<char*>(password.c_str()));
#ifdef _DEBUG_
    cout << "connected to oracle..." << endl;
#endif

    auto_ptr<AccountGen> ag(new AccountGen(pid));
    ag->print();

    dbconn->disconnectDB();

#ifdef _DEBUG_
    cout << "disconnected to oracle..." << endl;    
#endif
    log->Write("account generator work complete!");
    return EXIT_SUCCESS;
}
