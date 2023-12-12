#include <iostream.h>
#include <ConfigLog.hpp>
#include <logmessage.h>

int main(void) {

    MessageHeader logmsg;
    MessageQueue<MessageHeader> logbay("LogQueue.txt", 1000, 11, 1, 12, 1);
    LogManagement* plogmgr;

    cout << endl << "Log Collector Starting..." << endl;
    
    while (1) {

        if (logbay.recvMsg(&logmsg) == FLAG_MQ_IS_EMPTY) {
            sleep(1);
            continue;
        }
        
        plogmgr = new LogManagement(PATH_LOG_FILE,
                                    logmsg.getFname(),
                                    logmsg.getJobId(),
                                    logmsg.getProcId());

        if (plogmgr->getLogFlag() == ::ON) {  
  
            switch (logmsg.getType()) {
    
            case LogMessage::LM_CRITICAL:
      
                plogmgr->writeLog(const_cast<char*>(logmsg.getMessage()), ::LOG_CRITICAL);
                break;
    
            case LogMessage::LM_SHORT:
    
                plogmgr->writeLog(const_cast<char*>(logmsg.getMessage()), ::LOG_SHORT);
                break;
    
            case LogMessage::LM_DETAIL:
    
                plogmgr->writeLog(const_cast<char*>(logmsg.getMessage()), ::LOG_DETAIL);
                break;
    
            default:
    
                cerr << "Not Available Log Message Type : " << logmsg.getType() << endl; 
                break;      
            }
        }

        delete plogmgr;
        logmsg.clearAll();
    }  ////// END OF WHILE

    pslog.print();

    return EXIT_SUCCESS;
}
