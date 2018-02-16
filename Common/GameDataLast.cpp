//Copyright[2002] MasangSoft
// GameDataLast.cpp: implementation of the CGameDataLast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <io.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/stat.h>
#include <stdio.h>
#include "sha256.h"        // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 

#include "CipherRA.h"

bool CGameData::SetFile(char *filename, bool encode, char* encodeString, int encodeSize, bool bAllLoading)
{
    //    FLOG( "CGameData::SetFile(char *filename,  BOOL encode, char* encodeString, int encodeSize)" );
    
    strcpy(m_ZipFilePath, filename);

    if (encode) SetEncodeString(encodeString, encodeSize);
    m_bEncode = encode;

    if (bAllLoading) return make_parse_file_ext();

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        체크섬 처리
/// \author        // 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
///                // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 함수 수정
/// \date        2007-04-06 ~ 2007-04-06
/// \warning    
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
bool CGameData::GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath)
{
    // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
    //*o_puiCheckSum    = 0;            // 2007-05-28 by cmkwon
    memset(o_byObjCheckSum, 0x00, 32);    // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
    *o_pnFileSize    = 0;            // 2007-05-28 by cmkwon
    
    UINT uiCheckSum = 0;
    
    if (strlen(pFilePath) <=0) return FALSE;
    
    FILE *fp;
    fp = fopen(pFilePath, "rb");        // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - binary로 읽도록 수정
    
    if (NULL == fp) return FALSE;

    fseek(fp, 0L, SEEK_END);
    long lFileSize = ftell(fp);
    *o_pnFileSize = lFileSize;            // 2007-05-28 by cmkwon
    fseek(fp, 0L, SEEK_SET);    

    BYTE *pFileData = new BYTE [lFileSize];
    memset(pFileData, 0x00, lFileSize);
    fread(pFileData, lFileSize, 1, fp);
// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
//    for(int i=0; i < lFileSize/sizeof(UINT); i++)
//    {
//        uiCheckSum ^= ((UINT*)pFileData)[i];
//    }
    ///////////////////////////////////////////////////////////////////////////////
    // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
    sha256_encode(pFileData, lFileSize, o_byObjCheckSum);
    
    fclose(fp);
    delete[] pFileData;

    // 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 기존 소스
    //*o_puiCheckSum    = uiCheckSum;            // 2007-05-28 by cmkwon
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DataHeader* CGameData::FindFromFile(char* strName)
/// \brief        파일에서 원하는 데이타를 찾아서 로딩한다.
/// \author        dhkwon
/// \date        2004-06-03 ~ 2004-06-03
/// \warning    m_bEncode가 TRUE이면 실패한다.
///                리턴된 pDataHeader는 외부에서 지워줘야 한다.
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
DataHeader* CGameData::FindFromFile(char* strName)
{
#ifndef CIPHER2015
    if (m_bEncode) return NULL;
#endif

    TotalHeader totalHeader;
    memset(&totalHeader, 0x00, sizeof(totalHeader));
    
    int ReadFile = open(m_ZipFilePath, O_RDONLY | O_BINARY);
    if (ReadFile == -1) return NULL;
    
    _lseek(ReadFile, 0, SEEK_END);
    
    int readPointer = 0;
    
    _lseek(ReadFile, readPointer, SEEK_SET);
    read(ReadFile, (char*)&totalHeader, sizeof(totalHeader));
#ifdef CIPHER2015
    CipherRA((char*)&totalHeader, sizeof(totalHeader), readPointer);
#endif
    readPointer += sizeof(TotalHeader);
    
    DataHeader* pDataHeader = new DataHeader;
    
    for (int i = 0; i < totalHeader.m_DataNumber; i++)
    {
        memset(pDataHeader, 0x00, sizeof(DataHeader));
        
        _lseek(ReadFile, readPointer, SEEK_SET);
        read(ReadFile, (char*)pDataHeader, sizeof(DataHeader) - 4);            // 4: data pointer
#ifdef CIPHER2015
        CipherRA((char*)pDataHeader, sizeof(DataHeader) - 4, readPointer);
#endif
        readPointer += sizeof(DataHeader) - 4;        // 4: data pointer
        
        if (strcmp(pDataHeader->m_FileName, strName) == 0)
        {
            pDataHeader->m_pData = new char[pDataHeader->m_DataSize + 1];
            memset(pDataHeader->m_pData, 0x00, pDataHeader->m_DataSize + 1);    // 2006-04-03 by ispark
            
            read(ReadFile, pDataHeader->m_pData, pDataHeader->m_DataSize);
#ifdef CIPHER2015
            CipherRA((char*)pDataHeader->m_pData, pDataHeader->m_DataSize, readPointer);
#endif
        
            close(ReadFile);
            return pDataHeader;
        }
        
        readPointer += pDataHeader->m_DataSize;
    }
    
    close(ReadFile);
//    delete pDataHeader;
    util::del(pDataHeader);
    DBGOUT("====> Not Data File (%s)\n", strName);
    return NULL;
}

bool CGameData::make_parse_file_ext()
{
    //    FLOG( "CGameData::make_parse_file_ext()" );
    int EncodeFile,ReadFile;
    char Data[300],temp[300], DataBuff[MAXBUFF],Encode[1024];
    int maxsize;
    // read encode string
#ifndef CIPHER2015
    if (m_bEncode && strlen(m_EncodeStrFilePath) > 0)
    {
        memset(Encode,0x00,sizeof(Encode));
        EncodeFile = open(m_EncodeStrFilePath, O_RDONLY );

        read( EncodeFile, m_EncodeString, sizeof(m_EncodeString) );
        close(EncodeFile);
    }
#endif
    pTotal_header = new TotalHeader;
    memset(temp,0x00,sizeof(temp));
    memset(Data,0x00,sizeof(Data));
    ReadFile = open(m_ZipFilePath, O_RDONLY | O_BINARY);
    if (ReadFile == -1)
    {
        char buf[512];
        wsprintf(buf, "ERROR CGameData:: resource read(%s)",m_ZipFilePath);
//        FLOG( buf );
//        DBGOUT("ERROR CGameData:: resource read(%s)\n",m_ZipFilePath);
        util::del(pTotal_header);
        return FALSE;
    }
    maxsize = strlen(m_EncodeString);
    long length = _lseek(ReadFile, 0, SEEK_END);
    char* pTemp = new char[length];
    int readPointer=0;
    _lseek(ReadFile, 0, SEEK_SET);
    int n;
    
    while((n = read(ReadFile, DataBuff, sizeof(DataBuff))) != 0)
    {
        if(n == -1)
        {
            DBGOUT("%s ERROR ReadFile(%s - %d)\n", m_ZipFilePath, strerror( errno ), errno);
            util::del_array(pTemp);
            util::del(pTotal_header);
            close(ReadFile);
            return FALSE;
        }
        
#ifndef CIPHER2015
        if (m_bEncode) for (int j = 0; j < n; j++) DataBuff[j] ^= m_EncodeString[j % maxsize]; 
#endif

        memcpy(&pTemp[readPointer], DataBuff,n);
        readPointer += n;
        memset(DataBuff,0x00,sizeof(DataBuff));
    }
    close(ReadFile);

#ifdef CIPHER2015
    CipherRA(pTemp, length, 0);
#endif

    // read total header
    readPointer = 0;
    memcpy( pTotal_header, &pTemp[readPointer], sizeof(TotalHeader) );
    readPointer += sizeof(TotalHeader);
    // read data header
    for( int i=0 ; i < (pTotal_header->m_DataNumber) ; i++)
    {
        auto pHeader = new DataHeader;
        memcpy((void*) pHeader, &pTemp[readPointer], sizeof(DataHeader)-4);
        readPointer += sizeof(DataHeader)-4;
        pHeader->m_pData = new char[pHeader->m_DataSize+1] { }; //memset(pHeader->m_pData, 0x00, pHeader->m_DataSize+1);
        
        memcpy((void*) pHeader->m_pData, &pTemp[readPointer], pHeader->m_DataSize );
        readPointer += pHeader->m_DataSize;
        m_mapDataHeader[pHeader->m_FileName] = pHeader;
    }    
//    delete pTemp;
    util::del_array(pTemp);
    return TRUE;
}

bool CGameDataOpt::make_parse_file_ext()
{
    if (CGameData::make_parse_file_ext())
    {
        for (auto pair : m_mapDataHeader)
        {
            unsigned number = 0;
            for (auto ptr = pair.first.c_str(); *ptr != '\0'; ptr++)
            {
                if (*ptr >= '0' && *ptr <= '9') number = number * 10 + (*ptr - '0');
                else
                {
                    number = ~0;
                    break;
                }
            }

            if (number != ~0) m_mapDataHeaderOpt[number] = pair.second;
        }

        return true;
    }

    return false;
}

DataHeader* CGameDataOpt::find_by_number(unsigned number)
{
    auto itr = m_mapDataHeaderOpt.find(number);

    if (itr != m_mapDataHeaderOpt.end()) return itr->second;

    return nullptr;
}
