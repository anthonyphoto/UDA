#include <unistd.h>
#include <TempMsgQ_file.hpp>
#include <time.hpp>
#include <stdlib.h>

struct _DATANODE
{
	int iIndex;        // find disk_index node with return address.
	time_t tTime;
	int serialnumber;
	int weight;
	int price;
	char name[10];
	char address[50];
	char phone[13];
};

class	DataNode	: public _DATANODE
{
	public:
		void Display() const;
		operator struct _DATANODE* () const { return (_DATANODE*)this; }
		void putData()
		{
			iIndex 			= 0;
			serialnumber 	= 11;
			weight 			= 11;
			price			= 11;
			strcpy(name, "Hellow");
			strcpy(address, "Seoul");
			strcpy(phone, "01199783032");
		}
		void putDataIndex(int nIndex)	{	iIndex = nIndex;	}
};

void DataNode::Display() const
{
	cout << "== " << iIndex << " ================================"<< endl;
	cout << "iIndex =       ["	<< iIndex 		<< "]"	<< endl;
	cout << "serialnumber = ["	<< serialnumber	<< "]"	<< endl;
	cout << "weight =       ["	<< weight		<< "]"	<< endl;
	cout << "price =        ["	<< price		<< "]"	<< endl;
	cout << "name =         ["	<< name			<< "]"	<< endl;
	cout << "address =      ["	<< address		<< "]"	<< endl;
	cout << "phone =        ["	<< phone		<< "]"	<< endl;
}


int main(int argc, char* argv[])
{
	MessageQueue<DataNode>  DataNode1(1, 1, 5, 2, 0, 10,1);

	DataNode	OneRec;
	DateTime	CdateTime;

//	DataNode1 = new MessageQueue<DataNode>(	1, 1, 5, 2, 0, 10 );
	
	cout << "Rec Size = " << sizeof(DataNode) << endl;

	OneRec.putData();

	cout << "\nStart Recv Time = [" << CdateTime.get_time(1) << "]" << endl;
	int i = 0;
	memset (OneRec,0x00, sizeof(OneRec));
	while(1)
	{
		if (DataNode1.recvMsg(&OneRec) == FLAG_MQ_IS_EMPTY)
		{
			cout << "==MQ is Empty========================"<< endl;
			sleep (1);
			memset (OneRec,0x00, sizeof(OneRec));
			
			continue;
		}
		OneRec.Display();
		sleep(1);
		i++;
		memset (OneRec,0x00, sizeof(OneRec));

	}
	cout << "\nEnd Recv Time = [" << CdateTime.get_time(1) << "]" << endl;
	return 0;
}
