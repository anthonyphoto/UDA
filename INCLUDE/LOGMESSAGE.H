#ifndef _LOGMESSAGE_H_
#define _LOGMESSAGE_H_
#include <strstream.h>
#include <string>
#include <TempMsgQ1.hpp>
#include <iostream.h>
#include <fstream.h>
#include <time.hpp>


class MessageHeader {
public:

    ///// initializer, finalizer
  
    ///// accessor

    unsigned int getJobId(void);
    unsigned int getProcId(void);
    unsigned int getType(void);
    const char*  getFname(void);
    const char*  getDate(void);
    const char*  getTime(void);
    const char*  getMessage(void);

    ///// modifier    

    void clearAll(void);
    void setJobId(unsigned int);
    void setProcId(unsigned int);
    void setType(unsigned int);
    void setFname(const char*);
    void setDate(const char*);
    void setTime(const char*);
    void setMessage(const char*);

private:

    unsigned int m_jobid;      
    unsigned int m_procid;     
    unsigned int m_type;      
    char         m_fname[64];
    char         m_date[8];
    char         m_time[8];
    char         m_msgstring[2048];
};

class LogMessage {
public:

    ///// initializier, finalizier
    
    LogMessage();
    LogMessage(unsigned int, unsigned int, unsigned int, string&);
    ~LogMessage();    

    ///// accessor

    unsigned int getJobId(void);
    unsigned int getProcId(void);
    unsigned int getType(void);
    string&      getFname(void);
    string&      getContents(void);
//    const char*  getFname(void);
//    const char*  getContents(void);

    ///// modifier

    void putJobId(unsigned int);
    void putProcId(unsigned int);
    void putType(unsigned int);
    void putFname(string&);
    void putContents(string&);

public:

    enum log_type { LM_CRITICAL = 1, LM_SHORT, LM_DETAIL };

private:

    unsigned int m_jobid;      
    unsigned int m_procid;     
    unsigned int m_type;         // LM_CRITICAL, LM_SHORT, LM_DETAIL    
    string       m_fname;  
    string       m_contents;
};

#endif

#ifndef _LOGFACTORY_H_
#define _LOGFACTORY_H_

class LogFactory {
public:

    ///// initializer, finalizer

    LogFactory(unsigned int, unsigned int, unsigned int);
    ~LogFactory();

    ///// operator

    LogFactory& operator<<(const char*);
    LogFactory& operator<<(int);
    LogFactory& operator<<(unsigned int);
    LogFactory& operator<<(ostream&(*)(ostream&));
    LogFactory& operator<<(LogFactory&(LogFactory&));
    LogFactory& operator<<(char);
    LogFactory& operator<<(short);
    LogFactory& operator<<(long);
    LogFactory& operator<<(float);
    LogFactory& operator<<(double);
    LogFactory& operator<<(unsigned char);
    LogFactory& operator<<(unsigned short);
    LogFactory& operator<<(unsigned long);
    LogFactory& operator<<(void*);
    LogFactory& operator<<(streambuf*);
//    LogFactory& operator<<(ios& (*)(ios&));  // not implement

    ///// accessor

    int size(void);

    ///// modifier

    const char* str(void);   // gives dynamic buffer to client who must delete[] when done
                             // use of ostrstream or strstream object is undefined once
                             // this is called.
    void        print(void);    
    bool        fprint(const char*, int);
    void        putMessage(string&);

    bool        loadMessageQueue(void);
    LogFactory& packaging();
    LogFactory& sendmq(LogFactory&(LogFactory&));

private:
    
    ostrstream                   m_ostring;
    MessageHeader*               mp_msghd;
    LogMessage*                  mp_logmsg;
    MessageQueue<MessageHeader>* mp_logqueue; 
};

ostream& nl(ostream& os) {
    return os << '\n';
}

static LogFactory pslog(5, 0, LogMessage::LM_SHORT);
static LogFactory pclog(5, 0, LogMessage::LM_CRITICAL);
static LogFactory pdlog(5, 0, LogMessage::LM_DETAIL);


void MessageHeader::clearAll(void) {

    m_jobid  = 0;      
    m_procid = 0;     
    m_type   = 0;      
    memset(m_fname,      0x00, sizeof(m_fname));
    memset(m_date,       0x00, sizeof(m_date));
    memset(m_time,       0x00, sizeof(m_date));
    memset(m_msgstring,  0x00, sizeof(m_date));
}

void MessageHeader::setJobId(unsigned int jid) {
    MessageHeader::m_jobid = jid;     
}

void MessageHeader::setProcId(unsigned int pid) {
    MessageHeader::m_procid = pid;     
}

void MessageHeader::setType(unsigned int type) {
    MessageHeader::m_type = type;      
}

void MessageHeader::setFname(const char* str) {
    memcpy(MessageHeader::m_fname, str, strlen(str));
}

void MessageHeader::setDate(const char* date) {
    memcpy(MessageHeader::m_date, date, strlen(date));
}

void MessageHeader::setTime(const char* time) {
    memcpy(MessageHeader::m_time, time, strlen(time));
}

void MessageHeader::setMessage(const char* str) {
    if ( strlen(str) > sizeof(m_msgstring) ) {
        memcpy(MessageHeader::m_msgstring, str, sizeof(m_msgstring));
    } else {
        memcpy(MessageHeader::m_msgstring, str, strlen(str));
    }
}

unsigned int MessageHeader::getJobId(void) {
    return m_jobid;
}

unsigned int MessageHeader::getProcId(void) {
    return m_procid;
}

unsigned int MessageHeader::getType(void) {
    return m_type;
}

const char*  MessageHeader::getFname(void) {
    return m_fname;
}

const char*  MessageHeader::getDate(void) {
    return m_date;
}

const char*  MessageHeader::getTime(void) {
    return m_time;
}

const char*  MessageHeader::getMessage(void) {
    return m_msgstring;
}

LogMessage::LogMessage(unsigned int jid, unsigned int pid, unsigned int ty, string& fn) {

    m_jobid  = jid;
    m_procid = pid;
    m_type   = ty;
    m_fname  = fn;
}

LogMessage::~LogMessage() { }

void LogMessage::putJobId(unsigned int j) {
    m_jobid = j;
}
void LogMessage::putProcId(unsigned int p) {
    m_procid = p;
}
void LogMessage::putType(unsigned int t) {
    m_type = t;
}
void LogMessage::putFname(string& str) {
    m_fname = str;
}

void LogMessage::putContents(string& str) {
    m_contents = str;
}

unsigned int LogMessage::getJobId(void) {
    return m_jobid;
}

unsigned int LogMessage::getProcId(void) {
    return m_procid;
}

unsigned int LogMessage::getType(void) {
    return m_type;
}

string& LogMessage::getFname(void) {
    return m_fname;
}

string& LogMessage::getContents(void) {
    return m_contents;
}

LogFactory::LogFactory(unsigned int jid, unsigned int pid, unsigned int ty) {

    string fname;
    
    ///// make log file name;

    if (jid == 4 && pid == 0) {          // interface module
        fname = "IF_";
    } else if (jid == 2 && pid == 0) {   // parser module
        fname = "PS_";
    } else if (jid == 5 && pid == 0) {   // filter module
        fname = "FT_";
    } else if (jid == 7 && pid == 0) {   // aggregator module
        fname = "AG_";
    } else if (jid == 2 && pid == 1) {   // collector module
        fname = "CO_";
    } else if (jid == 7 && pid == 1) {   // collector module
        fname = "OB_";
    } else {
        cout << "undefined jobid and procid" << endl;
        fname = "undefined";
    }
    
    switch (ty) {
    case LogMessage::LM_CRITICAL:
        fname += "ERR";
        break;
    case LogMessage::LM_SHORT:
        fname += "MSG";
        break;         
    case LogMessage::LM_DETAIL:
        fname += "DET";
    default:
        cout << "undefined log type" << endl;
        break;
    }

    mp_logmsg   = new LogMessage(jid, pid, ty, fname);
    mp_logqueue = new MessageQueue<MessageHeader>("LogQueue.txt", 3000, 2, 0, 5, 0);
    mp_msghd    = new MessageHeader();
}

LogFactory::~LogFactory() {
    delete mp_logmsg;
    delete mp_logqueue;
    delete mp_msghd;
}

LogFactory& LogFactory::operator<<(const char* str) {
    m_ostring << str;
    return *this;
}

LogFactory& LogFactory::operator<<(int n) {
    m_ostring << n;
    return *this;
}

LogFactory& LogFactory::operator<<(unsigned int n) {
    m_ostring << n;
    return *this;
}

LogFactory& LogFactory::operator<<(char c) {
    m_ostring << c;
    return *this;
}

LogFactory& LogFactory::operator<<(short s) {
    m_ostring << s;
    return *this;
}

LogFactory& LogFactory::operator<<(long l) {
    m_ostring << l;
    return *this;
}

LogFactory& LogFactory::operator<<(float f) {
    m_ostring << f;
    return *this;
}

LogFactory& LogFactory::operator<<(double d) {
    m_ostring << d;
    return *this;
}

LogFactory& LogFactory::operator<<(unsigned char c) {
    m_ostring << c;
    return *this;
}

LogFactory& LogFactory::operator<<(unsigned short s) {
    m_ostring << s;
    return *this;
}

LogFactory& LogFactory::operator<<(unsigned long l) {
    m_ostring << l;
    return *this;
}

LogFactory& LogFactory::operator<<(void* v) {
    m_ostring << v;
    return *this;
}

LogFactory& LogFactory::operator<<(streambuf* sb) {
    m_ostring << sb;
    return *this;
}

LogFactory& LogFactory::operator<<(ostream& os(ostream&)) {
    m_ostring << os;
    return *this;
}

LogFactory& LogFactory::operator<<(LogFactory& lf(LogFactory&)) {

    string logcontents(m_ostring.str());  // streambuf is frozen
                                          // desturctor won't delete 
                                          // the streambuf storage on the heap
//    m_ostring.rdbuf()->freeze(0);         // unfreeze it
//                                          // now destructor releases memory
    delete m_ostring.str();
    putMessage(logcontents);    

    ostrstream newostream;
    newostream << m_ostring.rdbuf() << endl;   // old streambuf flush;

//    cout << "STREAMBUF : " << m_ostring.rdbuf() << endl;

    if (packaging().loadMessageQueue() == FALSE) {
        fprint(mp_logmsg->getFname().c_str(), ios::out|ios::app);
    }

    return *this;
}

LogFactory& sendmq(LogFactory& lf) {
    return lf;
}

const char* LogFactory::str(void) {
    return m_ostring.str();
}

int LogFactory::size(void) {
    return m_ostring.pcount();
}

void LogFactory::putMessage(string& str) {
    mp_logmsg->putContents(str);
}

LogFactory& LogFactory::packaging(void) {

    mp_msghd->setJobId(mp_logmsg->getJobId());
    mp_msghd->setProcId(mp_logmsg->getProcId());
    mp_msghd->setType(mp_logmsg->getType());
    mp_msghd->setFname(mp_logmsg->getFname().c_str());

    DateTime logoccured;
    mp_msghd->setDate(logoccured.get_date());
    mp_msghd->setTime(logoccured.get_time(1));
    mp_msghd->setMessage(mp_logmsg->getContents().c_str());
    
    return *this;
}

bool LogFactory::loadMessageQueue(void) {

    if (mp_logqueue->sendMsg(mp_msghd) == FLAG_MQ_IS_FULL) {
        return FALSE;
    }
    return TRUE;    
}

void LogFactory::print(void) {
    cout << m_ostring.rdbuf() << endl;
}

bool LogFactory::fprint(const char* fname, int open_mode) {

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

/********
int main(void) {


    string fname("log");    

    pslog << "Bad connecting" << ends << "after 5 min will disconnect" << nl;
    pslog.fprint(fname.c_str(), ios::out|ios::app);    

    pslog << sendmq << "MQ";

    int i = 1000;
    pslog << i;

    pslog.fprint(fname.c_str(), ios::out|ios::app);

    return EXIT_SUCCESS;
}
*********/
#endif
