#ifndef _USOCKETC_HPP
#define _USOCKETC_HPP

#include <socketC.hpp>



class USockC : public SockC {

	
	public :	
	
		USockC(){;}
		USockC(char *ip, int port) ;
		void Prn_nBackLog(void){printf("\n[ %d|%d ]\n", nBackLog, nPort);}
	
};


USockC::USockC(char *ip, int port) : SockC(ip, port)
{
	nBackLog=5;
}

#endif
