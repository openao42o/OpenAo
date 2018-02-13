// Zipfile1.cpp: implementation of the Zipfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumMonitor.h"
#include "Zipfile1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  MAXBUFF  65536
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Zipfile::Zipfile()
{
	pTotal_header = NULL;
	pData_header = NULL;
	memset(pData,0x00,sizeof(pData));	
}

Zipfile::~Zipfile()
{
	delete pTotal_header;
	delete [] pData_header;
	for(int i=0;i<10000;i++)
		delete pData[i];
	
}

void Zipfile::make_header(CString OpenFile[], int FileCount)
{
	CFile ReadFile;
    	
	pTotal_header = new TotalHeader;
	pData_header = new DataHeader[FileCount];

	CString temp;
	for( int i=0 ; i<FileCount ; i++)
	{
		ReadFile.Open(OpenFile[i],CFile::modeRead);

		(pData_header+i)->m_DataSize = ReadFile.GetLength();
		pTotal_header->m_FileSize += (pData_header+i)->m_DataSize;
		(pData_header+i)->m_EncodeNum = OpenFile[i].GetLength();
		int n = OpenFile[i].Find('.');
		if(n<0) n=OpenFile[i].GetLength();
//		temp = OpenFile[i].Left(OpenFile[i].GetLength()-4);
		temp = OpenFile[i].Left(n);
		strcpy((pData_header+i)->m_FileName,(LPCTSTR) temp); 
		
		ReadFile.Close();
	}
	pTotal_header->m_DataNumber = FileCount;
}

#include "dbgout_c.h"

void Zipfile::make_zip_file(CString OpenFile[],CProgressCtrl& pProgress, BOOL bEncode)
{
	int ReadFile,WriteFile;
    char Data[MAXBUFF];
	int Parity=0;
	
    memset(Data,0x00,sizeof(Data));
	if(bEncode)
		WriteFile = open("temp", O_RDWR | O_CREAT  | O_BINARY, _S_IREAD | _S_IWRITE);
	else
		WriteFile = open(m_EncodeStrFilePath, O_RDWR | O_CREAT  | O_BINARY, _S_IREAD | _S_IWRITE);

//	sprintf(Data, "%4d%4d%12d%4d%1d",pTotal_header->m_EncodeNum,pTotal_header->m_Identification,pTotal_header->m_FileSize,pTotal_header->m_DataNumber,Parity);
//	write(WriteFile,Data,25);
	write(WriteFile, (void*)pTotal_header,sizeof(TotalHeader));
	pProgress.SetRange(0,(pTotal_header->m_FileSize)/(MAXBUFF/2));
	int count=0;
	for( int i=0 ; i<pTotal_header->m_DataNumber ; i++)
	{
		ReadFile = open(OpenFile[i],O_RDWR | O_BINARY);

//		sprintf(Data, "%4d%12d%1d%-10s",(pData_header+i)->m_EncodeNum,(pData_header+i)->m_DataSize,Parity,(pData_header+i)->m_FileName);
//		write(WriteFile,Data,strlen(Data));
		write(WriteFile,(void*)(pData_header+i),sizeof(DataHeader));
		int n=0;
		
		while(n=read(ReadFile,Data,MAXBUFF))
		{
			count++;
			write(WriteFile,Data,n);
			pProgress.SetPos(count);
	//		DbgOut("%d\n",count);
		}
		close(ReadFile);
	}
	close(WriteFile);
}

void Zipfile::make_encode_file(CProgressCtrl& pProgress)
{
	int ReadFile,WriteFile,EncodeFile;
    char Data[MAXBUFF];
	int Parity=0;
	
	memset(Data,0x00,sizeof(Data));
	EncodeFile = open("EncodeFile.txt", O_RDWR | O_CREAT,_S_IREAD | _S_IWRITE  );
	write(EncodeFile, m_EncodeString, m_EncodeString.GetLength());
	close(EncodeFile);
	WriteFile = open(m_EncodeStrFilePath,  O_RDWR | O_CREAT | O_BINARY,_S_IREAD | _S_IWRITE  );
	ReadFile = open("temp", O_RDONLY | O_BINARY);
    
	

	int n=0, encode_length=0;
	encode_length = m_EncodeString.GetLength();
	static int encode=(pTotal_header->m_FileSize)/MAXBUFF;
	while(n=read(ReadFile,Data,MAXBUFF))
	{
		for(int j=0; j<n; j++)
		{
			Data[j] ^= m_EncodeString.GetAt(j%encode_length); 
		}
		encode++;
		DbgOut("%d\n",encode);
		write(WriteFile,Data,n);
		memset(Data,0x00,sizeof(Data));
		pProgress.SetPos(encode);
	}
    
	close(ReadFile);
	//ReadFile.Remove("temp");
	close(WriteFile);

}