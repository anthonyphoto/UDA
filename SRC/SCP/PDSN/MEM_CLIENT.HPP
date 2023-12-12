#ifndef _MEM_CLIENT_HPP
#define _MEM_CLIENT_HPP

class CLIENT_Format {
	private:
		char	strIP[18];
		int		Identifier;
		
	public:
		CLIENT_Format()
		{
			Identifier=0;
			memset(strIP, 0x00, sizeof(strIP));
		}
		
		//--------------------------- PUT
		void put_Identifier(int num){Identifier=num;}
		void put_strIP(char *str){strcpy(strIP, str);}
		
		//--------------------------- GET
		int   get_Identifier(void){return Identifier;}
		char* get_strIP(void){return strIP;}
};


//----------------------------------------------------------------
class Mem_Client {
	private:
		int			lastPosi;	// ��������ġ
		int			cdrCount;	// ��������� CDR����
		int			maxCount;
		
		CLIENT_Format	*cdrFormat;
		
	public:
		// cdr_count : ������ MEM client ����
		Mem_Client(int max_count);
		~Mem_Client();
		
					//����(-1) ����(���� ��ġ:curPosi)
		int 		Search(char* sIP);
					//�˻��� identifier �� ���Ϲ޴´�. ����(-1)
		int 		getSearchId(char* sIP);
					// client�� �߰��Ѵ�. 0: ����
		int			putCLI(char* sIP, int num);
					// client�� �����Ѵ�. 0: ����
		int			delCLI(int post);
					// client�� �����´�.
		CLIENT_Format *getCLI(int posi);
		
		void		displayALL(void);
	
};




//----------------------------------------------------------------
Mem_Client::Mem_Client(int max_count)
{
	lastPosi=0;
	cdrCount=0;
	maxCount=0;
	
	if(max_count<=0) return;
	
	cdrFormat = new CLIENT_Format[max_count];
	maxCount = max_count;
}


//----------------------------------------------------------------
Mem_Client::~Mem_Client()
{
	
	if(maxCount<=0) return;
	delete[] cdrFormat;
}


//----------------------------------------------------------------
int Mem_Client::putCLI(char* sIP, int num)
{
	if(cdrCount>=maxCount) return 0;		// ���� ũ�⸦ �ʰ��ߴ�
	int 	i;
	
	if( (i=Search(sIP)) >= 0 ) 
	{	// ������ �����ϴ� Client
		cdrFormat[i].put_Identifier(num);
		return -1;
	}
	
	cout<<"[Mem_Client::putCLI1] "<<  lastPosi << ":" << cdrCount << endl;
	//memcpy((void *)&cdrFormat[lastPosi], (void *)&cformat, sizeof(CLIENT_Format));
	cdrFormat[lastPosi].put_Identifier(num);
	cdrFormat[lastPosi].put_strIP(sIP);
	
	lastPosi++;
	cdrCount++;
	
	cout<<"[Mem_Client::putCLI2] "<< lastPosi << ":" << cdrCount << endl;
	return 1;
}



//----------------------------------------------------------------
CLIENT_Format *Mem_Client::getCLI(int posi)
{
	if(posi > lastPosi || posi < 0) 
	{
		cout << "[Mem_Client::getCLI()] ERROR!! posi > lastPosi" << endl; 
		return NULL;
	}
	return &cdrFormat[posi];
}



//----------------------------------------------------------------
int Mem_Client::delCLI(int posi)
{
	if(posi >= lastPosi || posi < 0) 
	{
		cout << "[Mem_Client::getCLI()] ERROR!! posi >= lastPosi" << endl; 
		return 0;
	}
	
	if(cdrCount==1) 
	{
		lastPosi=0;
		cdrCount=0;
	}else 
	{
		memcpy((void *)&cdrFormat[posi], (void *)&cdrFormat[lastPosi-1], sizeof(CLIENT_Format));
		lastPosi--;
		cdrCount--;
	}
	cout<<"[Mem_Client::delCLI] "<<  lastPosi << ":" << cdrCount << endl;
	return 1;
}



//----------------------------------------------------------------
int Mem_Client::Search(char *sIP)
{
	if(cdrCount<=0) return -1;
	int i=cdrCount;	
	int len=strlen(sIP);
	
	while(i--) if(!strncmp(cdrFormat[i].get_strIP(), sIP, len)) return i;
	
	return -1;
}


	
//----------------------------------------------------------------
int Mem_Client::getSearchId(char *sIP)
{
	if(cdrCount<=0) return -1;
	int i=cdrCount;	
	int len=strlen(sIP);

	while(i--) if(!strncmp(cdrFormat[i].get_strIP(), sIP, len)) return cdrFormat[i].get_Identifier();
	
	return -1;
}


//----------------------------------------------------------------
void Mem_Client::displayALL(void)
{
	if(cdrCount<=0) return ;
	int i;
	
	cout << "--------------------------------------------" << endl;
	for(i=0; i<	cdrCount; i++)
	{
		cout << i+1 << "> " << "[IP] : " << cdrFormat[i].get_strIP() 
			<< "[Identifier] : " << cdrFormat[i].get_Identifier() << endl ;
	}
	cout << "--------------------------------------------" << endl<<endl;
}


#endif