#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "CSnmpCli.hpp"

CSnmpCli	*snmpC;

int main(int argc, char** argv)
{
	int nRet;
	int nTime;
	int nSend;
	
	if(argc<4){
		printf("Usage: ip, port, proess alias\n");
		exit (1);
	}
	printf("Start...PDSN_CLIENT - [%s]!....", argv[3]);
	printf("IP : [%s] , PORT : [%s]\n\n", argv[1], argv[2]);
	
	snmpC = new CSnmpCli(argv[1], (unsigned int)atoi(argv[2]), argv[3]);
	
	snmpC->InitValue();
//	snmpC->FileChkNMake();
while(1){
	
	nRet = snmpC->FindMyNas();
	snmpC->Set_SndUdrCnt(snmpC->Get_SndUdrCnt()+1);
	snmpC->WritePerform();
	sleep(3);
}
	delete snmpC;
	exit (1);
}


