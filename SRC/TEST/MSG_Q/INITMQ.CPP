#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <MqInfo_file.hpp>

int main(int argc, char* argv[])
{
	MsgQfileInfo	MqFile;

	int nRet = MqFile.create_MqFile(	atoi(argv[1]),	//	nSystemId
										atoi(argv[2]),    //	nServiceId
										atoi(argv[3]),    //	nGroupId
										atoi(argv[4]),    //	nProcessType
										atoi(argv[5]),    //	nSerialNo
										atoi(argv[6]),    //	nRemark
										atoi(argv[7]),    //	nRecSize
										atoi(argv[8])	);  //	nRecCnt

	if (nRet < 0)
	{
		cout << "Mq initialize FAIL !! #" << nRet << endl;
		cout << "MQ File Name is [" << MqFile.get_strMsgQueueName() << "]" << endl;

		exit(-1);
	}

	exit(0);
}

