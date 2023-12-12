#include <iostream.h>
#include <MsgFormat.hpp>
#include <SendLogManage.hpp>

int main()
{
/*	HeaderOfMessage		clsHeaderOfMessage;
	MsgPmsStatus		clsMsgPmsStatus;
	MsgLog				clsMsgLog;
	ProcPerformData		clsProcPerformData;
	ProcEventData		clsProcEventData;
	ProcErrInfo			clsProcErrInfo;
*/
	cout << "Size of HeaderOfMessage = " << sizeof(HeaderOfMessage	) << endl;
	cout << "Size of MsgPmsStatus	 = " << sizeof(MsgPmsStatus		) << endl;
	cout << "Size of MsgLog			 = " << sizeof(MsgLog			) << endl;
	cout << "Size of ProcPerformData = " << sizeof(ProcPerformData	) << endl;
	cout << "Size of ProcEventData	 = " << sizeof(ProcEventData	) << endl;
	cout << "Size of ProcErrInfo	 = " << sizeof(ProcErrInfo	    ) << endl;
}

