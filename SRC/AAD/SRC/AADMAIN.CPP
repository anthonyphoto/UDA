/*
 * $Log: AADMain.cpp $
 * Revision 0.9.0  KST 2001/07/12 11:34:37  dongcheol 
 * ...
 * Revision 0.9.1  me
 * ...
 */


#include "BDRManager.hpp"

#include <iostream.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <limits.h>
#include "BlockFile.hpp"
#include "FileLogWrite.hpp"
#include "signal.hpp"

#include <SendLogManage.hpp>       // useless code for linking LogManager in library
SendLogManage*  clsLogM;           // useless code for linking LogManager in library
const char* FILE_NAME_COL_BDS_UDR; // useless code for linking LogManager in library

using namespace aad;
using namespace sig;

FileLogWrite* flog;

int main(int argc, char* argv[]) {

    init_signal();

    //
    // set path info, warning !! hard coded !!
    //

#ifdef _UDA1_
    string PATH_AAD_OUT("/SVC1/BDS/ABNORMAL/");   
    string FILE_NAME_COL_BDS_UDR  ("/SVC1/BDS/BDS_UDR."          );
    string PATH_NAME_BDS_BLOCK_CFG("/SVC1/DATA/BDS_BlockInfo.cfg");  
    string PATH_NAME_BDS_FILE_CFG ("/SVC1/DATA/BDS_FileConf.cfg" );
    const char* const AAD_CFG_FILE = "/SVC1/DATA/aad.cfg";
    const char* const AAD_SEQ_FILE = "/SVC1/DATA/aad.seq";
    const char* const PATH_AAD_LOG = "/SVC1/LOG/";
#endif

#ifdef _UDA2_
    string PATH_AAD_OUT("/SVC3/BDS/ABNORMAL/");   
    string FILE_NAME_COL_BDS_UDR  ("/SVC3/BDS/BDS_UDR."          );
    string PATH_NAME_BDS_BLOCK_CFG("/SVC3/DATA/BDS_BlockInfo.cfg");  
    string PATH_NAME_BDS_FILE_CFG ("/SVC3/DATA/BDS_FileConf.cfg" );
    const char* const AAD_CFG_FILE = "/SVC3/DATA/aad.cfg";
    const char* const AAD_SEQ_FILE = "/SVC3/DATA/aad.seq";
    const char* const PATH_AAD_LOG = "/SVC3/LOG/";
#endif

    flog = new FileLogWrite(PATH_AAD_LOG, "AAD");  
    auto_ptr<FileLogWrite> log(flog);
    
#ifdef _UDA1_
    log->Write("uda1 AAD Starting ... ");
#endif

#ifdef _UDA2_
    log->Write("uda2 AAD Starting ... ");
#endif


    //
    //  load limit
    //

    fstream fin(AAD_CFG_FILE, ios::in);

    if (!fin) {
        log->Write("failure in reading aad.cfg");
        return EXIT_SUCCESS;
    }

    float baseText       = 0.0;
    float baseOrigin     = 0.0;
    float baseMultiMedia = 0.0;
    
    fin >> baseText >> baseOrigin >> baseMultiMedia;

    fin.close();
    
    if (!fin) {
        log->Write("failure in closing aad.cfg");
        return EXIT_SUCCESS;
    }

    //
    // load bds file configuration
    //

    CFileConfigData bdsConfig(PATH_NAME_BDS_FILE_CFG.c_str());
    bdsConfig.read_FileConfigFile();
    int beginFileSeq = bdsConfig.get_unFileSeq() - 1;
    int lastFileSeq = 0;

    //
    //  read last file sequence
    //

    fstream finSeq(AAD_SEQ_FILE, ios::in);

    if (!finSeq) {
        log->Write("failure in opening aad.seq");
        return EXIT_SUCCESS;
    }

    int seq = 0;
    finSeq >> seq;
    log->Write("last seq : %d", seq);

    if (0 == seq) {
        lastFileSeq = beginFileSeq; // ???? 왜이렇게했지?
    } else {
        beginFileSeq = seq;
    }

    finSeq.close();
    
    if (!finSeq) {
        log->Write("failure in closing aad.seq");
        return EXIT_SUCCESS;
    }


    auto_ptr<BDRManager> bdrManager(new BDRManager());
    bdrManager->setPath(PATH_AAD_OUT);


    // 
    // one file test
    //

    if (2 == argc) {
        bdrManager->detect(baseText, baseOrigin, baseMultiMedia, atoi(argv[1]));
        cout << "AAD Work Complete !! " << endl;
        return EXIT_SUCCESS;              
    }

    //
    // search for abnormal account
    //

    while (g_stop) {

        bdsConfig.read_FileConfigFile();
        lastFileSeq = bdsConfig.get_unFileSeq() - 1;        

        if (lastFileSeq == beginFileSeq) {
            sleep(10);
            continue;
        }

        while (beginFileSeq != lastFileSeq) {

            bdrManager->detect(baseText, baseOrigin, baseMultiMedia, beginFileSeq);
            log->Write("%d", beginFileSeq);

            if (beginFileSeq == UINT_MAX) { // unsigned int limit : 4294967295
                beginFileSeq = 0;
            } else {
                beginFileSeq++;
            }
        }
    
        lastFileSeq = beginFileSeq;
    } // end of while
    
    //
    //  write last file sequence
    //

    fstream fout(AAD_SEQ_FILE, ios::out|ios::trunc);

    if (!fout) {
        log->Write("failure in writing aad.seq");
        return EXIT_SUCCESS;
    }

    fout << lastFileSeq;

    fout.close();
    
    if (!fout) {
        log->Write("failure in closing aad.seq");
        return EXIT_SUCCESS;
    }

    log->Write("AAD Work Complete !! ");
    return EXIT_SUCCESS;
}



