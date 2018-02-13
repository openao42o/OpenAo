#ifndef _ATUM_FIELDDATAMANAGER_H_
#define _ATUM_FIELDDATAMANAGER_H_

class CFieldIOCP;
class CFieldIOCPSocket;
class CAtumFieldDBManager;
struct CHARACTER;

class CFieldDataManager  
{
public:
	CFieldDataManager();

	virtual void InitFieldDataManager(CFieldIOCPSocket *i_pFieldIOCPSocket);

protected:
	CFieldIOCPSocket	*m_pFieldIOCPSocket;
	CAtumFieldDBManager	*m_pFieldDBManager;
	CHARACTER			*m_pCharacter;

public:
	static CFieldIOCP	*ms_pFieldIOCP2;
};

#endif // _ATUM_FIELDDATAMANAGER_H_
