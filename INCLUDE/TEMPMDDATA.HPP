#ifndef TEMPMDDATA_HPP
#define TEMPMDDATA_HPP

#include <iostream.h>
#include <fstream.h>

template <class T>
class MdData
{
	public:
		MdData(const char *strFile, int itsSize);
		MdData(const char *strFile, const MdData& rhs);
		~MdData();
		
		MdData& 	operator 	=	(const MdData&);
		T& 			operator 	[] 	(int offset)		{ return pType[offset]; }
		const T&	operator	[] 	(int offset) const 	{ return pType[offset]; }
		
		int 	GetSize() const { return itsSize; }
		int		GetRecSize() const { return sizeof(T); }
		
		void	putConfigData2File();
		void	getConfigFile2Data();
		
		void	putConfigData2FileByIndex(int nPosition);
		void	getConfigFile2DataByIndex(int nPosition);
		
	private:
		FILE_H_TYPE		stFileHeader;
		T 				*pType;
		T 				*pOneRecType;
		int 			itsSize;
		char			*strFileName;
};

template <class T>
MdData<T>::MdData(const char *strFile, int size) : itsSize(size)
{
	pType = new T[size];
	memset ((char*)pType, NULL, size * sizeof(T));
	
	pOneRecType = new T[1];
	memset ((char*)pOneRecType, NULL, sizeof(T));
	
	strFileName = new char[strlen(strFile)+1];
	memcpy(strFileName, strFile, strlen(strFile));
	strFileName[strlen(strFile)] = NULL;
	
}

template <class T>
MdData<T>::MdData(const char *strFile, const MdData& rhs)
{
	itsSize = rhs.GetSize();
	pType = new T[itsSize];
	for(int i = 0; i < itsSize; i++)
		pType[i] = rhs[i];
	
	pOneRecType = new T[1];
	memset ((char*)pOneRecType, NULL, sizeof(T));
	
	strFileName = new char[strlen(strFile)+1];
	memcpy(strFileName, strFile, strlen(strFile));
	strFileName[strlen(strFile)] = NULL;
}

template <class T>
MdData<T>::~MdData()
{
	delete [] pType;
	delete [] pOneRecType;
	delete [] strFileName;
}
	
// operator =
template <class T>
MdData<T>& MdData<T>::operator= (const MdData& rhs)
{
	if (this == &rhs)
		return *this;
		
	delete [] pType;
	itsSize = rhs.GetSize();
	pType = new T[itsSize];
	for(int i = 0; i < itsSize; i++)
		pType[i] = rhs[i];
		
	return *this;
}

template <class T>
void MdData<T>::putConfigData2File()
{
	ofstream	outfile(strFileName, ios::binary);
	
	for (int i = 0; i < GetSize(); i++)
		outfile.write((char*)&pType[i], sizeof(T));
}

template <class T>
void MdData<T>::getConfigFile2Data()
{
	ifstream	infile(strFileName, ios::binary);
	
	for (int i = 0; i < GetSize(); i++)
		infile.read((char*)&pType[i], sizeof(T));
}

template <class T>
void MdData<T>::putConfigData2FileByIndex(int nPosition)
{
	ofstream	outfile(strFileName, ios::binary);
	
	outfile.seekp(nPosition, ios::cur);
	outfile.write((char*)&pOneRecType[0], sizeof(T));
}

template <class T>
void MdData<T>::getConfigFile2DataByIndex(int nPosition)
{
	ifstream	infile(strFileName, ios::binary);
	
	infile.seekg(nPosition, ios::cur);
	infile.read((char*)&pOneRecType[0], sizeof(T));
}

#endif
