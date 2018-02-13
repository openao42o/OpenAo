// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__8D3F90AC_2CFE_4FCF_A845_1B7747927B9D__INCLUDED_)
#define AFX_LEFTVIEW_H__8D3F90AC_2CFE_4FCF_A845_1B7747927B9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPreServerNode;
class CMonitorWinSocket;
class CAtumMonitorDoc;
typedef list<CMonitorWinSocket*>	listMonitorWinSocketPtr;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CAtumMonitorDoc* GetDocument();

	CFont		m_font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	//CMonitorWinSocket *m_pMonitorWinSocket;

	BOOL						m_bNoticePlaySound;
	CImageList m_ImgList;
	HTREEITEM m_hRootTempItem, m_hPreItem, m_hFieldItem, m_hIMItem, m_hNPCItem, m_hLogItem;
	vector<HTREEITEM>	m_hItemVector;
	CTreeCtrl *m_pTreeCtrl;
	listMonitorWinSocketPtr		m_listMonitorWinSocketPtr;
	HTREEITEM					m_hRootItem;
	UINT						m_uiAliveTimer;

	HTREEITEM m_hSelectedItem;
	HTREEITEM m_hSelectedParentItem;

	int InitLeftView();

	void GetOnlyPathWithoutFileName(CString &i_szFilePath);
	BOOL InsertListMonitorSocket(CMonitorWinSocket *pSocket);
	BOOL DeleteListMonitorSocket(CMonitorWinSocket *pSocket);
	CMonitorWinSocket * FindMonitorSocket(SOCKET hSocket);
	BOOL GetVersionInfoFromFTP(CPreServerNode *i_pPreNode, ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);

	BOOL GetVersionInfoByHTTP(CPreServerNode *i_pPreNode, ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);

	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTcmConnectField();
	afx_msg void OnTcmDisconnectField();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcmNumClientsField();
	afx_msg void OnTcmPingField();
	afx_msg void OnTcmShutdownField();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnTcmPrintmapField();
	afx_msg void OnTcmMapblockinfoField();
	afx_msg void OnTcmConnectNpc();
	afx_msg void OnTcmPingNpc();
	afx_msg void OnTcmDisconnectNpc();
	afx_msg void OnTcmMapblockinfoNpc();
	afx_msg void OnTcmConnectPre();
	afx_msg void OnTcmDisconnectPre();
	afx_msg void OnTcmNumClientsPre();
	afx_msg void OnTcmPingPre();
	afx_msg void OnTcmReloadVersionInfoPre();
	afx_msg void OnTcmPrintDbg();
	afx_msg void OnTcmChangeBandwidthflagPre();
	afx_msg void OnTcmChangeBandwidthflagField();
	afx_msg void OnTcmChangeBandwidthflagNpc();
	afx_msg void OnTcmConnectIm();
	afx_msg void OnTcmDisconnectIm();
	afx_msg void OnTcmNumClientsIm();
	afx_msg void OnTcmPingIm();
	afx_msg void OnTcmPrintmapNpc();
	afx_msg void OnTcmShutdownIm();
	afx_msg void OnTcmShutdownNpc();
	afx_msg void OnTcmChangeBandwidthflagIm();
	afx_msg void OnTcmPrintDbgIm();
	afx_msg void OnTcmSetMsgPrintLevelField();
	afx_msg void OnTcmSetMsgPrintLevelIm();
	afx_msg void OnTcmSetMsgPrintLevelNpc();
	afx_msg void OnTcmSetMsgPrintLevelPre();
	afx_msg void OnTcmGetCharacterInfoField();
	afx_msg void OnTcmPausePre();
	afx_msg void OnTcmStartPre();
	afx_msg void OnFileNew();
	afx_msg void OnTcmRenameRoot();
	afx_msg void OnTcmDeleteRoot();
	afx_msg void OnTcmGetUserCountsIm();
	afx_msg void OnTcmGetUserCountsField();
	afx_msg void OnTcmGetServerGroupInfoPre();
	afx_msg void OnTcmReloadItemInfoField();
	afx_msg void OnTcmResetNpcConnectionField();
	afx_msg void OnTcmPreSetLimitUserCount();
	afx_msg void OnTcmGetUserCountsNpc();
	afx_msg void OnTcmNpcSetLimitMonsterCount();
	afx_msg void OnInfoGen();
	afx_msg void OnSqlReader();
	afx_msg void OnToolsStopNoticePlaysound();
	afx_msg void OnToolsSetNoticeFilePath();
	afx_msg void OnTcmReloadBlockedAccountPre();
	afx_msg void OnClientFtpUpdateManager();
	afx_msg void OnTcmReloadQuestInfoField();
	afx_msg void OnTcmFieldEnableChannel();
	afx_msg void OnTcmFieldDisableChannel();
	afx_msg void OnZipFileGen();
	afx_msg void OnTcmGetUserMonsterCount();
	afx_msg void OnTcmSetMgameEvent();
	afx_msg void OnTcmPreChangeEnableServerGroup();
	afx_msg void OnTcmFieldCitywarChangeTime();
	afx_msg void OnTcmPrepareShutdown();
	afx_msg void OnTcm3PrepareShutdown();
	//afx_msg void OnImportExcelfile();
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CAtumMonitorDoc* CLeftView::GetDocument()
   { return (CAtumMonitorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__8D3F90AC_2CFE_4FCF_A845_1B7747927B9D__INCLUDED_)
