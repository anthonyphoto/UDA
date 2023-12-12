#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#include <string>

#ifndef	CONFIGLOG_HPP
	#ifndef SHAREDCONF_HPP
		#include <ConfigLog.hpp>
	#endif
#endif

class LogMessage {
public:

    ///// initializer, finalizer

    LogMessage();
    LogMessage(unsigned int);
    ~LogMessage();

    ///// operator

    LogMessage& operator<<(const char*);
    LogMessage& operator<<(const int);
    LogMessage& operator<<(const unsigned int);
    LogMessage& operator<<(ostream&(*)(ostream&));

    ///// accessor

    int size(void);

    ///// modifier

    const char* str(void);   // gives dynamic buffer to client who must delete[] when done
                             // use of ostrstream or strstream object is undefined once
                             // this is called.
    void print(void);    
    bool fprint(const char*, int);
    
private:

    ostrstream     m_ostring;
    LogManagement* mp_logman;
    unsigned int   m_logtype;   // LOG_CRITICAL, LOG_SHORT, LOG_DETAIL
};

LogMessage::LogMessage() {

    mp_logman = new LogManagement(PATH_LOG_FILE, "LOGTEST", 9, 6);
    m_logtype = LOG_SHORT;
}

LogMessage::LogMessage(unsigned int lt) {

    mp_logman = new LogManagement(PATH_LOG_FILE, "LOGTEST", 9, 6);
    m_logtype = lt;
}

LogMessage::~LogMessage() {
    delete mp_logman;
}

LogMessage& LogMessage::operator<<(const char* str) {
    m_ostring << str;
    return *this;
}

LogMessage& LogMessage::operator<<(const int n) {

    m_ostring << n;
    return *this;
}

LogMessage& LogMessage::operator<<(const unsigned int n) {

    m_ostring << n;
    return *this;
}

LogMessage& LogMessage::operator<<(ostream&(*)(ostream&)) { // if you know why it works? send mail to cufena@hanmail.net    
    m_ostring << endl;
    return *this;
}

const char* LogMessage::str(void) {
    return m_ostring.str();
}

int LogMessage::size(void) {
    return m_ostring.pcount();
}

void LogMessage::print(void) {
    cout << m_ostring.rdbuf() << endl;
}

bool LogMessage::fprint(const char* fname, int open_mode) {

    fstream fout(fname, open_mode);

    if (!fout.good()) {
        cout << "Can't open " << fname << " file." << endl;
        return FALSE;
    }

    fout.seekp(ios::cur);
    fout << m_ostring.rdbuf() << endl;
    fout.close();

    return TRUE;
}

int main(void) {

    LogMessage lout;
    
    lout << "WarMarine" << " " << "BattleZealot" << endl;
//    lout.print();

    int i = 1000;
    lout << i;

    const char* logstr = lout.str();
//    auto_ptr<const char> AutoPtr1(logstr);
    cout << *logstr << endl;
    delete[] logstr;
    cout << *logstr << endl;

//    delete[] logstr;  
//    cout << *logstr;


//    cout << *AutoPtr1 << "AAA" << endl;

    
    lout.fprint("test.log", ios::out|ios::app);
  
    cout << lout.str() << "BBB" << endl;

    return EXIT_SUCCESS;
}


