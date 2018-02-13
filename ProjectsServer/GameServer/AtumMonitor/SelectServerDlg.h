#pragma once

#include "AtumParam.h"

extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

/////////////////////////////////////////////////////////////////////////////
// CSelectServerDlg dialog

class CSelectServerDlg : public CDialog
{
// Construction
public:
	explicit CSelectServerDlg(CString i_strServerName, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_SELECT_SERVER };

	CComboBox	m_ComboServerList;



	protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

public:
	GAME_SERVER_INFO_FOR_ADMIN m_GameServerInfoForAdmin;
	
	CString							m_strServerName;


// Implementation
protected:

	BOOL OnInitDialog() override;
	void OnOK() override;
	DECLARE_MESSAGE_MAP()
};