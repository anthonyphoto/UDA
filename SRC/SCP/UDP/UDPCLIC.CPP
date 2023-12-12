#include "udpC.hpp"

int getDateNTime(char *buf);
	
int main(int argc, char **argv)
{
	
	char  strBuf[1024];
	SockC *_socket;
	int SockStatus;
	
	_socket = new SockC((char*)SERV_UDP_ADDR, SERV_UDP_PORT);
	cout << "[NOTICE] CreateUDPClient => " << 
		_socket->CreateUDPClient((char*)SERV_UDP_ADDR, SERV_UDP_PORT, (char*)CLI_UDP_ADDR, CLI_UDP_PORT) << endl;

	while (1)
	{
		char strTimeStamp[15];
		getDateNTime(strTimeStamp);
		
		memset(strBuf,0x00,sizeof(strBuf));
		memcpy(strBuf,strTimeStamp,14);
		
		cout << "Send strBuf => " << strBuf << endl;
		cout << "SERV_UDP_ADDR => " << SERV_UDP_ADDR << endl;
		cout << "SERV_UDP_PORT => " << SERV_UDP_PORT << endl;
		//cout << "[NOTICE] WriteByte => " << 
			//_socket->SendtoUDPSocket(strBuf, strlen(strBuf), (char*)SERV_UDP_ADDR, SERV_UDP_PORT) << endl;

		SockStatus = _socket->SendtoUDPSocket(strBuf, strlen(strBuf), (char*)SERV_UDP_ADDR, SERV_UDP_PORT);
		if (SockStatus > 0)
		{
			cout << "[NOTICE] SendByte => " << SockStatus << endl;
			cout << "Send strBuf => " << strBuf << endl;
		}
		else//((SockStatus <0)
		{
			cout << "[ERROR] Send ERROR" << endl;
		}

		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		cout << "_socket->send_addr.sin_addr.s_addr => " << inet_ntoa((in_addr &)_socket->send_addr.sin_addr.s_addr) << endl;
		cout << "_socket->send_addr.sin_port => " << _socket->send_addr.sin_port << endl;
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		
		cout << "sleep(2)..." << endl;
		sleep(2);
		cout << endl << endl;





/******
		memset(strBuf,0x00,sizeof(strBuf));
	
		cout << "Initial Recv strBuf => " << strBuf << endl;
		
		//cout << "[NOTICE] RecvByte => " << 
			//_socket->RecvfromUDPSocket(strBuf, 1024, 0) << endl;
			
		//cout << "[NOTICE] RecvByte => " << 
			//_socket->RecvfromUDPSocketEx(strBuf, 1024, MSG_PEEK) << endl;

		//cout << "[NOTICE] RecvByte => " << 
			//_socket->RecvfromUDPSocketEx(strBuf, 1024, 0) << endl;
			
		SockStatus = _socket->RecvfromUDPSocketEx(strBuf, 1024, 0);
		if (SockStatus > 0)
		{
			cout << "[NOTICE] RecvByte => " << SockStatus << endl;
			cout << "Recv strBuf => " << strBuf << endl;
		}
		else if (SockStatus ==0)
		{
			cout << "[NOTICE] No Data... " << endl;
		}
		else//((SockStatus <0)
		{
			cout << "[ERROR] Recv ERROR" << endl;
		}
		
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		cout << "_socket->recv_addr.sin_addr.s_addr => " << inet_ntoa((in_addr &)_socket->recv_addr.sin_addr.s_addr) << endl;
		cout << "_socket->recv_addr.sin_port => " << _socket->recv_addr.sin_port << endl;
		cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		
		cout << "sleep(2)..." << endl;
		sleep(2);
		cout << endl << endl;
******/



	}

}

int getDateNTime(char *buf)
{
	time_t	  t;
	struct tm *s;

	time(&t);
	s = (struct tm *) localtime( &t );
	sprintf(buf,"%04d%02d%02d%02d%02d%02d",
			s->tm_year+1900, s->tm_mon+1,s->tm_mday,s->tm_hour,s->tm_min,s->tm_sec);
	return(1);
}




	

	

