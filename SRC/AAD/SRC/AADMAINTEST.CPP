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
#include "BlockFile.hpp"
#include "FileLogWrite.hpp"

#include <SendLogManage.hpp>       // useless code for linking LogManager in library
SendLogManage*  clsLogM;           // useless code for linking LogManager in library
const char* FILE_NAME_COL_BDS_UDR; // useless code for linking LogManager in library

using namespace aad;

FileLogWrite* flog;

int main(int argc, char* argv[]) {

    if (argc < 4) {
        cout << "Usage : AADMain 8.0 9.6 144.0 [start sequence] [end sequence] !! " << endl;
        return EXIT_SUCCESS;
    }

#ifdef _UDA1_
    string PATH_AAD_OUT("/SVC1/BDS/ABNORMAL/");   
    string FILE_NAME_COL_BDS_UDR  ("/SVC1/BDS/BDS_UDR."          );
    string PATH_NAME_BDS_BLOCK_CFG("/SVC1/DATA/BDS_BlockInfo.cfg");  
    string PATH_NAME_BDS_FILE_CFG ("/SVC1/DATA/BDS_FileConf.cfg" );
    const char* const AAD_CFG_FILE = "/SVC1/DATA/aad.cfg";
    const char* const PATH_AAD_LOG = "/SVC1/LOG/";
#endif

#ifdef _UDA2_
    string PATH_AAD_OUT("/SVC3/BDS/ABNORMAL/");   
    string FILE_NAME_COL_BDS_UDR  ("/SVC3/BDS/BDS_UDR."          );
    string PATH_NAME_BDS_BLOCK_CFG("/SVC3/DATA/BDS_BlockInfo.cfg");  
    string PATH_NAME_BDS_FILE_CFG ("/SVC3/DATA/BDS_FileConf.cfg" );
    const char* const AAD_CFG_FILE = "/SVC3/DATA/aad.cfg";
    const char* const PATH_AAD_LOG = "/SVC3/LOG/";
#endif

    flog = new FileLogWrite(PATH_AAD_LOG, "AAD");  
    auto_ptr<FileLogWrite> log(flog);

    cout << "AAD Starting ... " << endl;

    CFileConfigData bdsConfig(PATH_NAME_BDS_FILE_CFG.c_str());
    bdsConfig.read_FileConfigFile();
    int lastFileSeq = bdsConfig.get_unFileSeq() - 1;

    int currFileSeq = lastFileSeq;

    auto_ptr<BDRManager> bdrManager(new BDRManager());
    bdrManager->setPath(PATH_AAD_OUT);

    if (5 == argc) {
        bdrManager->detect(atof(argv[1]), atof(argv[2]), atof(argv[3]), atoi(argv[4]));
        cout << "AAD Work Complete !! " << endl;
    }

    int seq = 0;
    if (6 == argc) {
        for (seq = atoi(argv[4]); seq <= atoi(argv[5]); seq++) {
            bdrManager->detect(atof(argv[1]), atof(argv[2]), atof(argv[3]), seq);
            cout << " seq = " << seq << endl;
            log->Write("%d", seq);
        }
        cout << "AAD Work Complete !! " << endl;
        return EXIT_SUCCESS;        
    }

    bdrManager->detect(atof(argv[1]), atof(argv[2]), atof(argv[3]), lastFileSeq);

    while (true) {

        bdsConfig.read_FileConfigFile();
        currFileSeq = bdsConfig.get_unFileSeq() - 1;        

        if (currFileSeq > lastFileSeq) {
            lastFileSeq = currFileSeq;
            bdrManager->detect(atof(argv[1]), atof(argv[2]), atof(argv[3]), lastFileSeq);           
        } else {
            sleep(10);
        }
    }

    cout << "AAD Work Complete !! " << endl;

    return EXIT_SUCCESS;
}



