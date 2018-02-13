#if !defined(AFX_QUESTEDITORDLG_H__676C90EF_520E_4A6A_8865_8351893E8C5D__INCLUDED_)
#define AFX_QUESTEDITORDLG_H__676C90EF_520E_4A6A_8865_8351893E8C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QuestEditorDlg.h : header file
//
#include "QuestLoaderParser.h"
#include "Quest.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// CQuestEditorDlg dialog
class CPreviewDlg;
class CQuestLoadDlg;

class CQuestEditorDlg : public CDialog
{
// Construction
public:
	void InitQuestEditor(); //컨트롤 초기화
	void ReflectQuestEditor(); //선택된 퀘스트 이름으로 퀘스트를 QuestLoad 다이얼로그에 반영
	CQuestEditorDlg(CWnd* pParent = NULL);   // standard constructor
	~CQuestEditorDlg();
	int m_nVscrollPos, m_nHscrollPos; //스크롤 상하/좌우 값 변수
	CPreviewDlg* m_pPreviewDlg; //미리보기 다이얼로그 포인터
	
	int nHeightOfEntireScreen ,nHeightOfMyDlg; //다이얼로그 길이 계산 변수
	int nAdjHeight; 
	int nMaxDesiredHeight; 

	BOOL GetQuestFilesPath(char *o_szQuestDefinedPath, vector<string> *o_pVecQuestFiles); //퀘스트 파일 경로 저장

	ez_map<INT, CQuest*> QuestInfoMap;
	ez_map<INT, CQuest*>::iterator itr;
	
// Dialog Data
	//{{AFX_DATA(CQuestEditorDlg)
	enum { IDD = IDD_DIALOG_QUESTEDITOR };
	CListCtrl	m_ctrlList_reqMonsterList;
	CComboBox	m_ctrlQuestLoad;
	CListCtrl	m_ctrlListCtrlCAMixItem;
	CListCtrl	m_ctrlListCtrlCARandomPayItem;
	CListCtrl	m_ctrlListCtrlCAPayItem;
	CListCtrl	m_ctrlListCtrlCADelItem;
	CListCtrl	m_ctrlListCtrlCReqItem;
	CListCtrl	m_ctrlListCtrlSDelItem;
	CListCtrl	m_ctrlListCtrlPayItem;
	CListCtrl	m_ctrlListCtrlRecvReqIndex;
	CListCtrl	m_ctrlListCtrlReqIndex;
	CListCtrl	m_ctrlListCtrlReqItem;
	CComboBox	m_ctrlComboQuestEType;
	CComboBox	m_ctrlComboQuestSType;
	CComboBox	m_ctrlComboQuestType;
	CComboBox	m_ctrlComboQuestKind;
	CString	m_valueEditQuestIndex;
	CString	m_valueEditQuestName;
	CString	m_valueEditNPCIndex;
	CString	m_valueEditPreTalk;
	CString	m_valueEditMidTalk;
	CString	m_valueEditSuccessTalk;
	CString	m_valueEditDescription;
	CString	m_valueEditFailTalk;
	CString	m_valueEditEventMap;
	CString	m_valueEditNextQuestIndex;
	CString	m_valueEditPartyQuestMemberCount;
	CString	m_valueEditDropItemToAllCharacter;
	CString	m_valueEditTimeLimitInMinites;
	CString	m_valueEditDiscardableQuest;
	BOOL	m_valueCheckBGEAR;
	BOOL	m_valueCheckMGEAR;
	BOOL	m_valueCheckAGEAR;
	BOOL	m_valueCheckIGEAR;
	CString	m_valueEditReqItemName;
	CString	m_valueEditReqItemCount;
	CString	m_valueEditReqItems;
	CString	m_valueEditReqIndex;
	CString	m_valueEditRecvReqIndex;
	CString	m_valueEditReqLevelMin;
	CString	m_valueEditReqLevelMax;
	CString	m_valueEditReqAttackMin;
	CString	m_valueEditReqAttackMax;
	CString	m_valueEditReqDefenseMin;
	CString	m_valueEditReqDefenseMax;
	CString	m_valueEditReqFuelMin;
	CString	m_valueEditReqFuelMax;
	CString	m_valueEditReqSoulMin;
	CString	m_valueEditReqSoulMax;
	CString	m_valueEditReqShieldMin;
	CString	m_valueEditReqShieldMax;
	CString	m_valueEditReqDodgeMin;
	CString	m_valueEditReqDodgeMax;
	CString	m_valueEditReqGuild;
	BOOL	m_valueCheckNormal;
	BOOL	m_valueCheckVCN;
	BOOL	m_valueCheckANI;
	BOOL	m_valueCheckRRP;
	BOOL	m_valueCheckALL;
	CString	m_valueEditPayItemNum;
	CString	m_valueEditPayItemCount;
	CString	m_valueEditPayItemPerItem;
	BOOL	m_valueCheckPayItemBGear;
	BOOL	m_valueCheckPayItemIGear;
	BOOL	m_valueCheckPayItemMGear;
	BOOL	m_valueCheckPayItemAGear;
	BOOL	m_valueCheckPayItemALL;
	CString	m_valueEditSDelItemNum;
	CString	m_valueEditSDelItemCount;
	CString	m_valueEditStartMapIndex;
	CString	m_valueEditCReqNPCIndex;
	CString	m_valueEditCReqItemNum;
	CString	m_valueEditCReqItemCount;
	CString	m_valueEditCReqPerItem;
	BOOL	m_valueCheckCReqBGear;
	CString	m_valueEditCReqMonsterUniqueNum;
	CString	m_valueEditCReqMinCount;
	CString	m_valueEditCReqMaxCount;
	CString	m_valueEditCReqProbability;
	CString	m_valueEditMeetObjectIndex;
	CString	m_valueEditMeetObjectMapIndex;
	CString	m_valueEditPayBonusStat;
	CString	m_valueEditPayBonusSkillPoint;
	CString	m_valueEditPayFame;
	CString	m_valueEditPayStatus;
	CString	m_valueEditPayLevel;
	CString	m_valueEditPayExperience;
	CString	m_valueEditPayWarpMapIndex;
	BOOL	m_valueCheckCADelItemAll;
	CString	m_valueEditCADelItemNum;
	CString	m_valueEditCADelItemCount;
	CString	m_valueEditCAPayItemNum;
	CString	m_valueEditCAPayItemCount;
	CString	m_valueEditCAPayPropensity;
	CString	m_valueEditCACinemaNum;
	BOOL	m_valueCheckCANormal;
	BOOL	m_valueCheckCAVCN;
	BOOL	m_valueCheckCAANI;
	BOOL	m_valueCheckCARRP;
	BOOL	m_valueCheckCAALL;
	BOOL	m_valueCheckBIQuestAttrisCitiwar;
	BOOL	m_valueCheckBIQuestAttriNeverCompletionByUser;
	CString	m_valueEditCARandomPayItemNum;
	CString	m_valueEditCARandomPayItemCount;
	CString	m_valueEditCARandomPayProb100;
	CString	m_valueEditCARandomPayPerItemNum;
	CString	m_valueEditCAMixItemNum;
	CString	m_valueEditCAMixItemCount;
	CString	m_valueEditCAMixItemTargetItem;
	CString	m_valueEditCAMixItemProb;
	CString	m_valueEditReqMapNum;
	CString	m_valueEditReqMapX;
	CString	m_valueEditReqMapY;
	CString	m_valueEditReqMapR;
	CString	m_valueEditCReqMeetArea;
	CString	m_valueEditCReqMeetAreaX;
	CString	m_valueEditCReqMeetAreaY;
	CString	m_valueEditCReqMeetAreaR;
	int		m_editnMonsterUnikind;
	int		m_editnMonsterCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuestEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuestEditorDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonQuestmake();
	afx_msg void OnButtonReqitem();
	afx_msg void OnDblclkListReqItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonReqIndex();
	afx_msg void OnButtonRecvreqindex();
	afx_msg void OnDblclkListReqindex(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListRecvreqindex(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSpayitem();
	afx_msg void OnButtonSdelitem();
	afx_msg void OnButtonQuestend();
	afx_msg void OnBUTTONCReqItem();
	afx_msg void OnButtonCadelitem();
	afx_msg void OnButtonCapayitem();
	afx_msg void OnButtonCarandompayitem();
	afx_msg void OnDblClickListSpayitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSdelitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCreqitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCadelitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCapayitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCarandompayitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonQuestrefresh();
	afx_msg void OnKeydownListReqitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCamixitem();
	afx_msg void OnDblclkListCamixitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonQuestSelect();
	afx_msg void OnButtonInsertReqMonster();
	afx_msg void OnDblclkListReqMonster(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUESTEDITORDLG_H__676C90EF_520E_4A6A_8865_8351893E8C5D__INCLUDED_)
