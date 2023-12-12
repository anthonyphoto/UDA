#ifndef _SCPTID_HPP
#define _SCPTID_HPP

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

class	ScpIF_TID
{
	private:
		
		char 	PATH_NAME_SCPIFR_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		char 	PATH_NAME_SCPIFS_TID[80];	//	=   "/SVC#/DATA/SCPIF_TID.cfg";
		
		char	TDay[10];			// date
 		char	STDay[10];			// date
		
	public:
		ScpIF_TID(int serviceid);
		~ScpIF_TID(){;}

		unsigned int	get_S_TID(void);	  
		unsigned int	get_R_TID(void);
		
		unsigned int	get_S_Day(void)
		{
			get_S_TID();
			return atol(TDay);
		}	  
		unsigned int	get_R_Day(void)
		{
			get_R_TID();
			return atol(STDay);
		}
		
		void			set_S_TID(unsigned int stid, unsigned int idate);
		void			set_R_TID(unsigned int rtid, unsigned int idate);
		
		void			init_TID(void)
		{
			set_S_TID(0,0);	
			set_R_TID(0,0);	
		}
		void			display_TID(void)
		{
			cout << "Schannel Transaction-ID[" << TDay << "] : " << get_S_TID() << endl;
			cout << "Rchannel Transaction-ID[" << STDay << "] : " << get_R_TID() << endl;
		}
};


ScpIF_TID::ScpIF_TID(int serviceid)
{
	
	sprintf(PATH_NAME_SCPIFS_TID, "/SVC%d/DATA/SCPIFS_TID.cfg", serviceid);
	sprintf(PATH_NAME_SCPIFR_TID, "/SVC%d/DATA/SCPIFR_TID.cfg", serviceid);
	memset(TDay, 0x00, sizeof(TDay));
	memset(STDay, 0x00, sizeof(STDay));
}


unsigned int ScpIF_TID::get_S_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFS_TID << endl;
		return 0;
	}else
	{
		fread(TDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}



unsigned int ScpIF_TID::get_R_TID(void)
{
	unsigned int i=0;
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "r+b"))) 
	{
		cout << "ERROR : READ : " << PATH_NAME_SCPIFR_TID << endl;
		return 0;
	}else
	{
		fread(STDay, 10, 1, fp);
		fread((void*)&i, 4, 1, fp);
		fclose(fp);
	}
	
	return i;
}


void ScpIF_TID::set_S_TID(unsigned int stid, unsigned int idate)
{
	FILE *fp;
	
	
	if(!(fp = fopen(PATH_NAME_SCPIFS_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFS_TID << endl;
		return;
	}else
	{
		sprintf(TDay, "%8ld", idate);
		
		fwrite(TDay, 1, 10, fp);
		fwrite((void*)&stid, 1, 4, fp);
		fclose(fp);
	}
	
}


void ScpIF_TID::set_R_TID(unsigned int rtid, unsigned int idate)
{
	FILE *fp;
	
	if(!(fp = fopen(PATH_NAME_SCPIFR_TID, "w+b"))) 
	{
		cout << "ERROR : WRITE : " << PATH_NAME_SCPIFR_TID << endl;
		return;
	}else
	{
		sprintf(STDay, "%8ld", idate);
		fwrite(STDay, 1, 10, fp);
		fwrite((void*)&rtid, 1, 4, fp);
		fclose(fp);
	}
}

#endif
