//////////////////////////////////////////////////////////////////////////////
// I/F에서 MsgQueue를 이용해 parser로 send하는 기능을 simulate 하는 program //
//////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
//#include <TempMsgQ1.hpp>
#include <TempMsgQ_file.hpp>
#include <time.hpp>


class SendBuffer {
public: 
//    unsigned int radiustype; 

	unsigned int processType;	
	unsigned int processSN;	
	unsigned int processRemark;	
    unsigned int fseq;
    char xmlstring[32768];
};
MessageQueue<SendBuffer> *sendbufS;
int main(int argc, char* argv[]) {

    if (argc != 4) {
        cout << "Usage : Do you know? Maybe like this ] quesend startfilesequence endfilesequence repeattimes" << endl;
        return EXIT_SUCCESS;
    }
    
    int fstartseq   = atoi(argv[1]);
    int fendseq     = atoi(argv[2]);
    int repeattimes = atoi(argv[3]);

	SendBuffer sbuf;
	
	sendbufS = new MessageQueue<SendBuffer>(1, 1, 6, 3, 0, 10, 1000 );
    //MessageQueue<SendBuffer> sendbufS("IFOut.txt", 1000, 4, 0, 2, 0);
 	//MessageQueue<SendBuffer> sendbufS("IFOut2.txt", 1000, 12, 10, 12, 10);
	DateTime CdateTimeS;

	char     TimeBufS[LEN_TIME];

	memset(TimeBufS, NULL, LEN_TIME);

	int i = 0,
	nTime = 0;

    fstream fin;

    char udrfile[] = "RADIUS0_UDR1.";
    char strbuf[15];   
    int rtimes = 0; 
    int k = fstartseq;

    while (1) {

        if (k > fendseq) {
            //cout << "reading file compelete ! " << "[ " << rtimes << " ]" << endl;
            if ( rtimes >= repeattimes ) {
                break;
            }
            k = fstartseq;
            rtimes++;
        }

        memset(strbuf, 0x00, sizeof(strbuf));
        sprintf(strbuf, "%s%d", udrfile, k);

        fin.open(strbuf, ios::in);

        if (!fin.good()) {
            k++;
            if (k > fendseq) {
                return EXIT_SUCCESS;
            }
            continue;
        }

        fin.seekg(0, ios::beg);
        fin.read(reinterpret_cast<char*>(&sbuf.xmlstring), sizeof(sbuf.xmlstring));
        fin.close();
        
//        sbuf.radiustype = 1;
	 sbuf.processType = 1;	
	 sbuf.processSN = 1;	
	 sbuf.processRemark = 0;	
        sbuf.fseq = i;
        if (sendbufS->sendMsg(&sbuf) == FLAG_MQ_IS_FULL) {
            cout << "==Send=====================Waiting====="	<< endl;
            sleep(1);                
            continue;
        }
        
        cout << "SEND [ " << sbuf.fseq << " ] " << endl;
        k++;
        i++;
        memset((char*)&sbuf, 0x00, sizeof(sbuf));
        usleep(100000);
    }

    return 0;
}
