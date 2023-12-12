#ifndef STARTDISPLAY_HPP
#define STARTDISPLAY_HPP

#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>

class CStartDisplay
{
	private:
		int 				flag;
		
		int 				DisplayMainMenu(void);
		int 				DisplayInterfaceMenu(void);
		int		 			DisplayProcessMenu(void);
		int		 			DisplayUtilMenu(void);
        void                Hi(void);
		
	public:
		CStartDisplay();
		~CStartDisplay();
		void				DisplayStart( void );

};

CStartDisplay::CStartDisplay()
{
	flag = 1;
	Hi();
}
CStartDisplay::~CStartDisplay()
{
}
void CStartDisplay::DisplayStart(void)
{
	while(flag)
	{
		switch( DisplayMainMenu() )
		{
			case 1: 
				switch( DisplayInterfaceMenu() )
				{
					case 1:
						system( "./RIFINIT1" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 2:
						system( "./RIFSTART1" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 3:
						system( "./RIFSTOP1" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 4:
						system( "./BIFINIT1" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 5:
						cout << "Press Anykey!!" << endl;
						getchar();
						system( "./BIFSTART1" );
						break;
					case 6:
						system( "./BIFSTOP1" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 7:
					default:
						break;
				};
				break;
			case 2:
				switch( DisplayProcessMenu() )
				{
					case 1:
						system( "./UDASTART2" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 2:
						system( "./UDASTOP" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 3:
					default:
						break;
				};
				break;
			case 3:
				switch( DisplayUtilMenu() )
				{
					case 1:
						system( "./IFCHECK" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 2:
						system( "./IFCHECK_RUN2" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 3:
						system( "./DISPLAYMEM" );
						break;
					case 4:
						system( "./CLEARMEM" );
						cout << "Press Anykey!!" << endl;
						getchar();
						break;
					case 5:
					default:
						break;
				};
				break;
			default:
				flag=0;
				break;
		}
	}
}
int CStartDisplay::DisplayMainMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n\n\n\n\n\n" << endl;
	cout << "\t\t********** UDA OPERATION MENU **********\n" << endl;
	cout << "\t\t*    1. Interface Operation            *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*    2. Process Operation              *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*    3. Utility Operation              *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*    4. Exit                           *\n" << endl;
	cout << "\t\t****************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}
int CStartDisplay::DisplayInterfaceMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t********** Interface Operation *********\n" << endl;
	cout << "\t\t*          <RADIUS Interface>          *\n" << endl;
	cout << "\t\t*    1. Radius I/F Initialize          *\n" << endl;
	cout << "\t\t*    2. Radius I/F START               *\n" << endl;
	cout << "\t\t*    3. Radius I/F STOP                *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*            <BDS Interface>           *\n" << endl;
	cout << "\t\t*    4. BDS I/F Initialize             *\n" << endl;
	cout << "\t\t*    5. BDS I/F START                  *\n" << endl;
	cout << "\t\t*    6. BDS I/F STOP                   *\n" << endl;
	cout << "\t\t*    7. Back                           *\n" << endl;
	cout << "\t\t****************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}
int CStartDisplay::DisplayProcessMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t*********** Process Operation **********\n" << endl;
	cout << "\t\t*    1. UDA START                      *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*    2. UDA STOP                       *\n" << endl;
	cout << "\t\t*                                      *\n" << endl;
	cout << "\t\t*    3. Back                           *\n" << endl;
	cout << "\t\t****************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}


int CStartDisplay::DisplayUtilMenu(void)
{
	int select;

	cout << "\33[2J\33[r";
	cout << "\n" << endl;
	cout << "\t\t************ Utility Menu ***********\n" << endl;
	cout << "\t\t* 1. I/F Checking                   *\n" << endl;
	cout << "\t\t* 2. I/F Check and Running          *\n" << endl; 
	cout << "\t\t* 3. Display Memory                 *\n" << endl; 
	cout << "\t\t* 4. Memory Clear                   *\n" << endl;
	cout << "\t\t* 5. Back                           *\n" << endl;
	cout << "\t\t*************************************\n" << endl;
	cout << "\n" << endl;
	cout << "\t\tselect menu: ";
	cin >> select;
	return select;
}
void CStartDisplay::Hi(void)
{
    printf("\33[2J\33[r");
	printf("\n\n\n\n");
    printf("\t\t                              ##         ##  \n");
	printf("\t\t                            ##  ##     ##  ##\n");
    printf("\t\t                                    *        \n\n");
	printf("\t\t        ###    ###         UU    UU  DDDD        AAA\n");
    printf("\t\t       ###    ###          UU    UU  DD  DD     AA AA\n");
	printf("\t\t      ###    ###           UU    UU  DD   DD    AA AA\n");
	printf("\t\t     ###    ###            UU    UU  DD   DD   AA   AA\n");
	printf("\t\t    ### ## ###             UU    UU  DD   DD   AA   AA\n");
	printf("\t\t   ###    ###   ###        UU    UU  DD   DD  AAAAAAAAA\n");
	printf("\t\t  ###    ###               UU    UU  DD   DD  AA     AA\n");
	printf("\t\t ###    ###   ###          UU    UU  DD  DD  AA       AA\n");
	printf("\t\t###    ###   ###   **  **    UUUU    DDDD    AA       AA\n\n\n\n");
	printf("\t\tPress any key ...");
	getchar();
}	
#endif
