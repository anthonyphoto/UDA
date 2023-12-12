#include <unistd.h>
#include <iostream.h>
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
			serialnumber 	= 22222;
			weight 			= 222222;
			price			= 22222;
			strcpy(name, "jjk33");
			strcpy(address, "j33");
			strcpy(phone, "333333");
		}
		void putDataIndex(int nIndex)	{	iIndex = nIndex;	}
};

void DataNode::Display() const
{
	cout << "== " << iIndex << " ================================"	<< endl;
	cout << "iIndex =       ["	<< iIndex 		<< "]"	<< endl;
	cout << "serialnumber = ["	<< serialnumber	<< "]"	<< endl;
	cout << "weight =       ["	<< weight		<< "]"	<< endl;
	cout << "price =        ["	<< price		<< "]"	<< endl;
	cout << "name =         ["	<< name			<< "]"	<< endl;
	cout << "address =      ["	<< address		<< "]"	<< endl;
	cout << "phone =        ["	<< phone		<< "]"	<< endl;
}

MessageQueue<DataNode>*  DataNode1;

int main(int argc, char* argv[])
{
	cout << "Test Record Size = " << sizeof(DataNode) << endl;


//	MessageQueue<DataNode>  DataNode1(	atoi(argv[1]), atoi(argv[2]), atoi(argv[3]),                \
//										atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

//	DataNode1 = new MessageQueue<DataNode>(	atoi(argv[1]), atoi(argv[2]), atoi(argv[3]),                \
//										atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

	DataNode1 = new MessageQueue<DataNode>(	1, 1, 5, 2, 0, 10,1 );


	DataNode	OneRec;
	DateTime	CdateTime;

	OneRec.putData();

	int i = 0,
	nTime = 0;

	cout << "\nStart Send Time = [" << CdateTime.get_time(1) << "]" << endl;

	while(1)
	{
		OneRec.putDataIndex(i);

		if (DataNode1->sendMsg(&OneRec) == FLAG_MQ_IS_FULL)
		{
			OneRec.Display();
			cout << "==MQ is Full========================"	<< endl;
			cout << "==Send================================\n"	<< endl;
			
			continue;
		}

		OneRec.Display();
		usleep (10);
		cout << "==Send================================\n"	<< endl;

		i++;

	}
	cout << "\nEnd Send Time = [" << CdateTime.get_time(1) << "]" << endl;

	return 0;
}

