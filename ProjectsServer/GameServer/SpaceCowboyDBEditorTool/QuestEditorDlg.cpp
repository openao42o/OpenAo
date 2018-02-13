// QuestEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "QuestEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuestEditorDlg dialog
#include "QuestLoaderParser.h"
#include "Quest.h"
#include <assert.h>


CQuestEditorDlg::CQuestEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestEditorDlg)
	m_valueEditQuestIndex = _T("");
	m_valueEditQuestName = _T("");
	m_valueEditNPCIndex = _T("");
	m_valueEditPreTalk = _T("");
	m_valueEditMidTalk = _T("");
	m_valueEditSuccessTalk = _T("");
	m_valueEditDescription = _T("");
	m_valueEditFailTalk = _T("");
	m_valueEditEventMap = _T("");
	m_valueEditNextQuestIndex = _T("");
	m_valueEditPartyQuestMemberCount = _T("");
	m_valueEditDropItemToAllCharacter = _T("");
	m_valueEditTimeLimitInMinites = _T("");
	m_valueEditDiscardableQuest = _T("");
	m_valueCheckBGEAR = FALSE;
	m_valueCheckMGEAR = FALSE;
	m_valueCheckAGEAR = FALSE;
	m_valueCheckIGEAR = FALSE;
	m_valueEditReqItemName = _T("");
	m_valueEditReqItemCount = _T("");
	m_valueEditReqItems = _T("");
	m_valueEditReqIndex = _T("");
	m_valueEditRecvReqIndex = _T("");
	m_valueEditReqLevelMin = _T("");
	m_valueEditReqLevelMax = _T("");
	m_valueEditReqAttackMin = _T("");
	m_valueEditReqAttackMax = _T("");
	m_valueEditReqDefenseMin = _T("");
	m_valueEditReqDefenseMax = _T("");
	m_valueEditReqFuelMin = _T("");
	m_valueEditReqFuelMax = _T("");
	m_valueEditReqSoulMin = _T("");
	m_valueEditReqSoulMax = _T("");
	m_valueEditReqShieldMin = _T("");
	m_valueEditReqShieldMax = _T("");
	m_valueEditReqDodgeMin = _T("");
	m_valueEditReqDodgeMax = _T("");
	m_valueEditReqGuild = _T("");
	m_valueCheckNormal = FALSE;
	m_valueCheckVCN = FALSE;
	m_valueCheckANI = FALSE;
	m_valueCheckRRP = FALSE;
	m_valueCheckALL = FALSE;
	m_valueEditPayItemNum = _T("");
	m_valueEditPayItemCount = _T("");
	m_valueEditPayItemPerItem = _T("");
	m_valueCheckPayItemBGear = FALSE;
	m_valueCheckPayItemIGear = FALSE;
	m_valueCheckPayItemMGear = FALSE;
	m_valueCheckPayItemAGear = FALSE;
	m_valueCheckPayItemALL = FALSE;
	m_valueEditSDelItemNum = _T("");
	m_valueEditSDelItemCount = _T("");
	m_valueEditStartMapIndex = _T("");
	m_valueEditCReqNPCIndex = _T("");
	m_valueEditCReqItemNum = _T("");
	m_valueEditCReqItemCount = _T("");
	m_valueEditCReqPerItem = _T("");
	m_valueCheckCReqBGear = FALSE;
	m_valueEditCReqMonsterUniqueNum = _T("");
	m_valueEditCReqMinCount = _T("");
	m_valueEditCReqMaxCount = _T("");
	m_valueEditCReqProbability = _T("");
	m_valueEditMeetObjectIndex = _T("");
	m_valueEditMeetObjectMapIndex = _T("");
	m_valueEditPayBonusStat = _T("");
	m_valueEditPayBonusSkillPoint = _T("");
	m_valueEditPayFame = _T("");
	m_valueEditPayStatus = _T("");
	m_valueEditPayLevel = _T("");
	m_valueEditPayExperience = _T("");
	m_valueEditPayWarpMapIndex = _T("");
	m_valueCheckCADelItemAll = FALSE;
	m_valueEditCADelItemNum = _T("");
	m_valueEditCADelItemCount = _T("");
	m_valueEditCAPayItemNum = _T("");
	m_valueEditCAPayItemCount = _T("");
	m_valueEditCAPayPropensity = _T("");
	m_valueEditCACinemaNum = _T("");
	m_valueCheckCANormal = FALSE;
	m_valueCheckCAVCN = FALSE;
	m_valueCheckCAANI = FALSE;
	m_valueCheckCARRP = FALSE;
	m_valueCheckCAALL = FALSE;
	m_valueCheckBIQuestAttrisCitiwar = FALSE;
	m_valueCheckBIQuestAttriNeverCompletionByUser = FALSE;
	m_valueEditCARandomPayItemNum = _T("");
	m_valueEditCARandomPayItemCount = _T("");
	m_valueEditCARandomPayProb100 = _T("");
	m_valueEditCARandomPayPerItemNum = _T("");
	m_valueEditCAMixItemNum = _T("");
	m_valueEditCAMixItemCount = _T("");
	m_valueEditCAMixItemTargetItem = _T("");
	m_valueEditCAMixItemProb = _T("");
	m_valueEditReqMapNum = _T("");
	m_valueEditReqMapX = _T("");
	m_valueEditReqMapY = _T("");
	m_valueEditReqMapR = _T("");
	m_valueEditCReqMeetArea = _T("");
	m_valueEditCReqMeetAreaX = _T("");
	m_valueEditCReqMeetAreaY = _T("");
	m_valueEditCReqMeetAreaR = _T("");
	m_editnMonsterUnikind = 0;
	m_editnMonsterCount = 0;
	//}}AFX_DATA_INIT
	m_pPreviewDlg = NULL;
}

CQuestEditorDlg::~CQuestEditorDlg()
{
	util::del(m_pPreviewDlg);
	//QuestInfoMap 종료 처리
	itr = QuestInfoMap.begin();
	while (itr != QuestInfoMap.end())
	{
		delete itr->second;
		itr++;
	}
}

void CQuestEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestEditorDlg)
	DDX_Control(pDX, IDC_LIST_REQ_MONSTER, m_ctrlList_reqMonsterList);
	DDX_Control(pDX, IDC_COMBO_QUESTLIST2, m_ctrlQuestLoad);
	DDX_Control(pDX, IDC_LIST_CAMIXITEM, m_ctrlListCtrlCAMixItem);
	DDX_Control(pDX, IDC_LIST_CARANDOMPAYITEM, m_ctrlListCtrlCARandomPayItem);
	DDX_Control(pDX, IDC_LIST_CAPAYITEM, m_ctrlListCtrlCAPayItem);
	DDX_Control(pDX, IDC_LIST_CADELITEM, m_ctrlListCtrlCADelItem);
	DDX_Control(pDX, IDC_LIST_CREQITEM, m_ctrlListCtrlCReqItem);
	DDX_Control(pDX, IDC_LIST_SDELITEM, m_ctrlListCtrlSDelItem);
	DDX_Control(pDX, IDC_LIST_SPAYITEM, m_ctrlListCtrlPayItem);
	DDX_Control(pDX, IDC_LIST_RECVREQINDEX, m_ctrlListCtrlRecvReqIndex);
	DDX_Control(pDX, IDC_LIST_REQINDEX, m_ctrlListCtrlReqIndex);
	DDX_Control(pDX, IDC_LIST_REQITEM, m_ctrlListCtrlReqItem);
	DDX_Control(pDX, IDC_COMBO13, m_ctrlComboQuestEType);
	DDX_Control(pDX, IDC_COMBO3, m_ctrlComboQuestSType);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlComboQuestType);
	DDX_Control(pDX, IDC_COMBO_QUESTKIND, m_ctrlComboQuestKind);
	DDX_Text(pDX, IDC_EDIT_QUESTINDEX, m_valueEditQuestIndex);
	DDX_Text(pDX, IDC_EDIT_QUESTNAME, m_valueEditQuestName);
	DDX_Text(pDX, IDC_EDIT4, m_valueEditNPCIndex);
	DDX_Text(pDX, IDC_EDIT5, m_valueEditPreTalk);
	DDX_Text(pDX, IDC_EDIT6, m_valueEditMidTalk);
	DDX_Text(pDX, IDC_EDIT7, m_valueEditSuccessTalk);
	DDX_Text(pDX, IDC_EDIT30, m_valueEditDescription);
	DDX_Text(pDX, IDC_EDIT18, m_valueEditFailTalk);
	DDX_Text(pDX, IDC_EDIT31, m_valueEditEventMap);
	DDX_Text(pDX, IDC_EDIT8, m_valueEditNextQuestIndex);
	DDX_Text(pDX, IDC_EDIT33, m_valueEditPartyQuestMemberCount);
	DDX_Text(pDX, IDC_EDIT35, m_valueEditDropItemToAllCharacter);
	DDX_Text(pDX, IDC_EDIT36, m_valueEditTimeLimitInMinites);
	DDX_Text(pDX, IDC_EDIT37, m_valueEditDiscardableQuest);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL, m_valueCheckBGEAR);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY, m_valueCheckMGEAR);
	DDX_Check(pDX, IDC_CHECK_BIT_PATTERN_MONSTER, m_valueCheckAGEAR);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER, m_valueCheckIGEAR);
	DDX_Text(pDX, IDC_EDIT1, m_valueEditReqItemName);
	DDX_Text(pDX, IDC_EDIT2, m_valueEditReqItemCount);
	DDX_Text(pDX, IDC_EDIT_REQINDEX, m_valueEditReqIndex);
	DDX_Text(pDX, IDC_EDIT_RECVREQINDEX, m_valueEditRecvReqIndex);
	DDX_Text(pDX, IDC_EDIT10, m_valueEditReqLevelMin);
	DDX_Text(pDX, IDC_EDIT17, m_valueEditReqLevelMax);
	DDX_Text(pDX, IDC_EDIT11, m_valueEditReqAttackMin);
	DDX_Text(pDX, IDC_EDIT40, m_valueEditReqAttackMax);
	DDX_Text(pDX, IDC_EDIT12, m_valueEditReqDefenseMin);
	DDX_Text(pDX, IDC_EDIT19, m_valueEditReqDefenseMax);
	DDX_Text(pDX, IDC_EDIT13, m_valueEditReqFuelMin);
	DDX_Text(pDX, IDC_EDIT20, m_valueEditReqFuelMax);
	DDX_Text(pDX, IDC_EDIT14, m_valueEditReqSoulMin);
	DDX_Text(pDX, IDC_EDIT21, m_valueEditReqSoulMax);
	DDX_Text(pDX, IDC_EDIT15, m_valueEditReqShieldMin);
	DDX_Text(pDX, IDC_EDIT22, m_valueEditReqShieldMax);
	DDX_Text(pDX, IDC_EDIT16, m_valueEditReqDodgeMin);
	DDX_Text(pDX, IDC_EDIT23, m_valueEditReqDodgeMax);
	DDX_Text(pDX, IDC_EDIT27, m_valueEditReqGuild);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL2, m_valueCheckNormal);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY2, m_valueCheckVCN);
	DDX_Check(pDX, IDC_CHECK_BIT_PATTERN_MONSTER2, m_valueCheckANI);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER3, m_valueCheckRRP);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER4, m_valueCheckALL);
	DDX_Text(pDX, IDC_EDIT24, m_valueEditPayItemNum);
	DDX_Text(pDX, IDC_EDIT26, m_valueEditPayItemCount);
	DDX_Text(pDX, IDC_EDIT_REQINDEX2, m_valueEditPayItemPerItem);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL3, m_valueCheckPayItemBGear);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER5, m_valueCheckPayItemIGear);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY3, m_valueCheckPayItemMGear);
	DDX_Check(pDX, IDC_CHECK_BIT_PATTERN_MONSTER3, m_valueCheckPayItemAGear);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER6, m_valueCheckPayItemALL);
	DDX_Text(pDX, IDC_EDIT29, m_valueEditSDelItemNum);
	DDX_Text(pDX, IDC_EDIT43, m_valueEditSDelItemCount);
	DDX_Text(pDX, IDC_EDIT44, m_valueEditStartMapIndex);
	DDX_Text(pDX, IDC_EDIT45, m_valueEditCReqNPCIndex);
	DDX_Text(pDX, IDC_EDIT46, m_valueEditCReqItemNum);
	DDX_Text(pDX, IDC_EDIT47, m_valueEditCReqItemCount);
	DDX_Text(pDX, IDC_EDIT_REQINDEX3, m_valueEditCReqMonsterUniqueNum);
	DDX_Text(pDX, IDC_EDIT_REQINDEX4, m_valueEditCReqMinCount);
	DDX_Text(pDX, IDC_EDIT_REQINDEX5, m_valueEditCReqMaxCount);
	DDX_Text(pDX, IDC_EDIT_REQINDEX6, m_valueEditCReqProbability);
	DDX_Text(pDX, IDC_EDIT48, m_valueEditMeetObjectIndex);
	DDX_Text(pDX, IDC_EDIT49, m_valueEditMeetObjectMapIndex);
	DDX_Text(pDX, IDC_EDIT50, m_valueEditPayBonusStat);
	DDX_Text(pDX, IDC_EDIT51, m_valueEditPayBonusSkillPoint);
	DDX_Text(pDX, IDC_EDIT52, m_valueEditPayFame);
	DDX_Text(pDX, IDC_EDIT53, m_valueEditPayStatus);
	DDX_Text(pDX, IDC_EDIT54, m_valueEditPayLevel);
	DDX_Text(pDX, IDC_EDIT55, m_valueEditPayExperience);
	DDX_Text(pDX, IDC_EDIT56, m_valueEditPayWarpMapIndex);
	DDX_Check(pDX, IDC_CHECK1, m_valueCheckCADelItemAll);
	DDX_Text(pDX, IDC_EDIT57, m_valueEditCADelItemNum);
	DDX_Text(pDX, IDC_EDIT58, m_valueEditCADelItemCount);
	DDX_Text(pDX, IDC_EDIT59, m_valueEditCAPayItemNum);
	DDX_Text(pDX, IDC_EDIT60, m_valueEditCAPayItemCount);
	DDX_Text(pDX, IDC_EDIT65, m_valueEditCAPayPropensity);
	DDX_Text(pDX, IDC_EDIT66, m_valueEditCACinemaNum);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL5, m_valueCheckCANormal);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY5, m_valueCheckCAVCN);
	DDX_Check(pDX, IDC_CHECK_BIT_PATTERN_MONSTER5, m_valueCheckCAANI);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER9, m_valueCheckCARRP);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER10, m_valueCheckCAALL);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL6, m_valueCheckBIQuestAttrisCitiwar);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY6, m_valueCheckBIQuestAttriNeverCompletionByUser);
	DDX_Text(pDX, IDC_EDIT61, m_valueEditCARandomPayItemNum);
	DDX_Text(pDX, IDC_EDIT62, m_valueEditCARandomPayItemCount);
	DDX_Text(pDX, IDC_EDIT63, m_valueEditCARandomPayProb100);
	DDX_Text(pDX, IDC_EDIT64, m_valueEditCARandomPayPerItemNum);
	DDX_Text(pDX, IDC_EDIT67, m_valueEditCAMixItemNum);
	DDX_Text(pDX, IDC_EDIT68, m_valueEditCAMixItemCount);
	DDX_Text(pDX, IDC_EDIT70, m_valueEditCAMixItemTargetItem);
	DDX_Text(pDX, IDC_EDIT71, m_valueEditCAMixItemProb);
	DDX_Text(pDX, IDC_EDIT69, m_valueEditReqMapNum);
	DDX_Text(pDX, IDC_EDIT74, m_valueEditReqMapX);
	DDX_Text(pDX, IDC_EDIT75, m_valueEditReqMapY);
	DDX_Text(pDX, IDC_EDIT76, m_valueEditReqMapR);
	DDX_Text(pDX, IDC_EDIT72, m_valueEditCReqMeetArea);
	DDX_Text(pDX, IDC_EDIT77, m_valueEditCReqMeetAreaX);
	DDX_Text(pDX, IDC_EDIT78, m_valueEditCReqMeetAreaY);
	DDX_Text(pDX, IDC_EDIT79, m_valueEditCReqMeetAreaR);
	DDX_Text(pDX, IDC_EDIT_MONSTER_UNIKIND, m_editnMonsterUnikind);
	DDX_Text(pDX, IDC_EDIT_MONSTER_COUNT, m_editnMonsterCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CQuestEditorDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_QUESTMAKE, OnButtonQuestmake)
	ON_BN_CLICKED(IDC_BUTTON_REQITEM, OnButtonReqitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REQITEM, OnDblclkListReqItem)
	ON_BN_CLICKED(IDC_BUTTON_REQINDEX, OnButtonReqIndex)
	ON_BN_CLICKED(IDC_BUTTON_RECVREQINDEX, OnButtonRecvreqindex)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REQINDEX, OnDblclkListReqindex)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECVREQINDEX, OnDblclkListRecvreqindex)
	ON_BN_CLICKED(IDC_BUTTON_SPAYITEM, OnButtonSpayitem)
	ON_BN_CLICKED(IDC_BUTTON_SDELITEM, OnButtonSdelitem)
	ON_BN_CLICKED(IDC_BUTTON_QUESTEND, OnButtonQuestend)
	ON_BN_CLICKED(IDC_BUTTON_CReqItem, OnBUTTONCReqItem)
	ON_BN_CLICKED(IDC_BUTTON_CADELITEM, OnButtonCadelitem)
	ON_BN_CLICKED(IDC_BUTTON_CAPAYITEM, OnButtonCapayitem)
	ON_BN_CLICKED(IDC_BUTTON_CARANDOMPAYITEM, OnButtonCarandompayitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPAYITEM, OnDblClickListSpayitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SDELITEM, OnDblclkListSdelitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CREQITEM, OnDblclkListCreqitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CADELITEM, OnDblclkListCadelitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAPAYITEM, OnDblclkListCapayitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CARANDOMPAYITEM, OnDblclkListCarandompayitem)
	ON_BN_CLICKED(IDC_BUTTON_QUESTREFRESH, OnButtonQuestrefresh)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_REQITEM, OnKeydownListReqitem)
	ON_BN_CLICKED(IDC_BUTTON_CAMIXITEM, OnButtonCamixitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CAMIXITEM, OnDblclkListCamixitem)
	ON_BN_CLICKED(IDC_BUTTON_QUEST_SELECT, OnButtonQuestSelect)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_REQ_MONSTER, OnButtonInsertReqMonster)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REQ_MONSTER, OnDblclkListReqMonster)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestEditorDlg message handlers

void CQuestEditorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CRect rectEntireScreen; 
	HWND hWndEntireScreen = NULL; 
	CRect rectMyDlg; 
	
	hWndEntireScreen = ::GetDesktopWindow(); 
	BOOL bSuccess = ::GetWindowRect(hWndEntireScreen,&rectEntireScreen); 
	if (bSuccess != FALSE) 
	{ 
		nHeightOfEntireScreen = rectEntireScreen.Height(); 
		
		GetWindowRect(&rectMyDlg); 
		nHeightOfMyDlg = rectMyDlg.Height(); 
		
		nAdjHeight = min(nHeightOfEntireScreen,nHeightOfMyDlg); 
		
		//In my case, I want the height to be no more than 90% of the entire screen 
		nMaxDesiredHeight = (int)(0.9 * (double)nHeightOfEntireScreen); 
		nMaxDesiredHeight = nMaxDesiredHeight/100*100;
		if (nAdjHeight >= nMaxDesiredHeight) 
		{ 
			nAdjHeight = nMaxDesiredHeight; 
		} 
		MoveWindow(rectMyDlg.left,rectMyDlg.top, 
			rectMyDlg.Width(),nAdjHeight); 
	} 
}

void CQuestEditorDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	INT uTemp;
    uTemp = nPos;
	uTemp = GetScrollPos(SB_VERT);
	int vsize;

	RECT rt;
	GetWindowRect(&rt);
	vsize = (nHeightOfMyDlg-rt.bottom+rt.top+50)/10;

    switch(nSBCode)
    {
    case SB_LINEUP:
		if(uTemp==1)
		{
			uTemp= 1;
			ScrollWindow(0,vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, -1, TRUE);
		}
        else if(uTemp>0)
		{
			uTemp--;
			ScrollWindow(0,vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else
		{
			uTemp = 0;
			ScrollWindow(0,vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        break;
    case SB_PAGEUP:
        if(uTemp >=10) 
		{
			uTemp -=10;
			ScrollWindow(0,vsize/10*10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else 
		{
			ScrollWindow(0,uTemp*vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, 0, TRUE);
		}
        break;

    case SB_LINEDOWN:
		TRACE("LINEDOWN %d %d\n",uTemp, vsize);
        if(uTemp <100) 
		{
			uTemp++;
			ScrollWindow(0,-vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else 
		{
			uTemp = 100;
			
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        break;

    case SB_PAGEDOWN:
		TRACE("PAGEDOWN %d\n",vsize/10*10);
        if(uTemp <=90)
		{
			uTemp +=10;
			ScrollWindow(0,-vsize/10*10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else 
		{
			ScrollWindow(0,uTemp-100, NULL, NULL);
			uTemp = 100;
			SetScrollPos(SB_VERT, uTemp*vsize/10, TRUE);
		}
        break;

	case SB_THUMBPOSITION:
        break;

    case SB_THUMBTRACK:
		int a = (uTemp-nPos)*(vsize/10);
        ScrollWindow(0,a, NULL, NULL);
		TRACE("%d %d %d %d\n",nPos, uTemp, vsize, a);
		SetScrollPos(SB_VERT, nPos, TRUE);
        break;
    }

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CQuestEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nHscrollPos = 0;
    m_nVscrollPos = 0;
	
	//ComboBox에 데이터 입력
	m_ctrlComboQuestKind.InsertString(0,"$QUEST_KIND_NORMAL");
	m_ctrlComboQuestKind.InsertString(1,"$QUEST_KIND_SIDESTORY");	// 2006-03-15 by cmkwon, QUEST_KIND_COLLECTION를 변경함
	m_ctrlComboQuestKind.InsertString(2,"$QUEST_KIND_SCENARIO");

	m_ctrlComboQuestType.InsertString(0,"$QUEST_TYPE_SINGLE");
	m_ctrlComboQuestType.InsertString(1,"$QUEST_TYPE_SERIES");
	m_ctrlComboQuestType.InsertString(2,"$QUEST_TYPE_REPEATABLE");

	m_ctrlComboQuestSType.InsertString(0,"$QUEST_START_TYPE_LEVEL_CHANGE");
	m_ctrlComboQuestSType.InsertString(1,"$QUEST_START_TYPE_EXP_CHANGE");
	m_ctrlComboQuestSType.InsertString(2,"$QUEST_START_TYPE_MEET_NPC");
	m_ctrlComboQuestSType.InsertString(3,"$QUEST_START_TYPE_ENTER_BUILDING");
	m_ctrlComboQuestSType.InsertString(4,"$QUEST_START_TYPE_ITEM_CLICK");
	m_ctrlComboQuestSType.InsertString(5,"$QUEST_START_TYPE_COUPON");

	m_ctrlComboQuestEType.InsertString(0,"$QUEST_END_TYPE_IMMEDIATE");
	m_ctrlComboQuestEType.InsertString(1,"$QUEST_END_TYPE_REQESTED_BY_CLIENT");
	m_ctrlComboQuestEType.InsertString(2,"$QUEST_END_TYPE_LEVEL_UP");
	m_ctrlComboQuestEType.InsertString(3,"$QUEST_END_TYPE_TIME_OVER");
	m_ctrlComboQuestEType.InsertString(4,"$QUEST_END_TYPE_SELECT_INFLUENCE");

// 2005-10-26 by cmkwon, 처음에는 필요 없음
//	//퀘스트 파일 로드
//	char szQuestDefinedPath[MAX_PATH];
//	util::zero(szQuestDefinedPath, MAX_PATH);
//	vector<string> vecQuestFiles;
//
//	if(FALSE == GetQuestFilesPath(szQuestDefinedPath, &vecQuestFiles))
//	{
//		AfxMessageBox("File Open Fail");
//		EndDialog(0);
//		return 0;
//	}
//
//	//로드된 퀘스트 파일 파싱
//	if (!QuestLoaderParser::ParseFiles(szQuestDefinedPath, &vecQuestFiles, &QuestInfoMap))
//	{
//		return FALSE;
//	}
//
//	itr = QuestInfoMap.begin();
//
//	while (QuestInfoMap.end() != itr)
//	{
//		CQuest *pQuest = itr->second;
//		m_ctrlQuestLoad.AddString(pQuest->QuestName);
//
//		itr++;
//	}
//
//	m_ctrlQuestLoad.SetCurSel(0);
//	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//퀘스트 파일 다시 로드
void CQuestEditorDlg::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	m_ctrlQuestLoad.ResetContent();	
	int a = m_ctrlQuestLoad.GetCount();
	char szQuestDefinedPath[MAX_PATH];
	util::zero(szQuestDefinedPath, MAX_PATH);
	vector<string> vecQuestFiles;

	QuestInfoMap.clear();

	if(FALSE == GetQuestFilesPath(szQuestDefinedPath, &vecQuestFiles))
	{
		AfxMessageBox("File Open Fail");
		return;
	}

	if (!QuestLoaderParser::ParseFiles(szQuestDefinedPath, &vecQuestFiles, &QuestInfoMap))
	{
		AfxMessageBox("File Parsing Fail");
		return;
	}

	itr = QuestInfoMap.begin();

	while (QuestInfoMap.end() != itr)
	{
		CQuest *pQuest = itr->second;
		m_ctrlQuestLoad.AddString(pQuest->QuestName);

		itr++;
	}

	m_ctrlQuestLoad.SetCurSel(0);
	UpdateData(FALSE);

}

void CQuestEditorDlg::OnButtonQuestmake() 
{
	// TODO: Add your control notification handler code here
	if (m_pPreviewDlg != NULL)
	{
		util::del(m_pPreviewDlg);
	}

	if (m_pPreviewDlg == NULL)
	{
		//미리보기 다이얼로그
		m_pPreviewDlg = new CPreviewDlg(this);
		if(m_pPreviewDlg->Create(IDD_DIALOG_PREVIEW, this)== TRUE)
		{
			m_pPreviewDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("Dialog Open Fail");
		}
	}
}

void CQuestEditorDlg::OnButtonReqitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditReqItemName == "" || m_valueEditReqItemCount == "")
		return;
	CString tempString =  m_valueEditReqItemName + "(" + m_valueEditReqItemCount + ")";
	m_ctrlListCtrlReqItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonReqIndex() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditReqIndex == "")
		return;
	m_ctrlListCtrlReqIndex.InsertItem(0,m_valueEditReqIndex);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonRecvreqindex() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditRecvReqIndex == "")
		return;
	m_ctrlListCtrlRecvReqIndex.InsertItem(0,m_valueEditRecvReqIndex);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonSpayitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditPayItemNum == "" || m_valueEditPayItemCount == "" || m_valueEditPayItemPerItem == "")
		return;
	CString tempString =  m_valueEditPayItemNum + "(" + m_valueEditPayItemCount + ")" + " PER ";
	
	int tempBit = 0;

	if(m_valueCheckPayItemBGear == TRUE)
		tempBit += UNITKIND_BGEAR_MASK;
	if(m_valueCheckPayItemIGear == TRUE)
		tempBit += UNITKIND_IGEAR_MASK;
	if(m_valueCheckPayItemMGear == TRUE)
		tempBit += UNITKIND_MGEAR_MASK;
	if(m_valueCheckPayItemAGear == TRUE)
		tempBit += UNITKIND_AGEAR_MASK;
	if(m_valueCheckPayItemALL == TRUE)
		tempBit = UNITKIND_ALL_MASK;

	CString tempValue;
	tempValue.Format("%d", tempBit);
	if(tempBit != 0)
	{
		tempString += m_valueEditPayItemPerItem + " TO ";
		tempString += tempValue;
	}
	else 
		tempString += m_valueEditPayItemPerItem + " TO ";

	m_ctrlListCtrlPayItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonSdelitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditSDelItemNum == "" || m_valueEditSDelItemCount == "")
		return;
	CString tempString =  m_valueEditSDelItemNum + "(" + m_valueEditSDelItemCount + ")";
	m_ctrlListCtrlSDelItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonQuestend() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CQuestEditorDlg::OnBUTTONCReqItem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditCReqItemNum == "" || m_valueEditCReqItemCount == "" || m_valueEditCReqMonsterUniqueNum == "" || m_valueEditCReqMinCount == "" || m_valueEditCReqMaxCount == "" || m_valueEditCReqProbability == "")
		return;
	CString tempString =  m_valueEditCReqItemNum + "(" + m_valueEditCReqItemCount + ") FROM MONSTER(";
	tempString += m_valueEditCReqMonsterUniqueNum + "," + m_valueEditCReqMinCount +"~" + m_valueEditCReqMaxCount +"," + m_valueEditCReqProbability + ")";
	m_ctrlListCtrlCReqItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonCadelitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditCADelItemNum == "" || m_valueEditCADelItemCount == "")
		return;
	CString tempString =  m_valueEditCADelItemNum + "(" + m_valueEditCADelItemCount + ")";
	if(m_valueCheckCADelItemAll == TRUE)
		tempString =  m_valueEditCADelItemNum + "(ALL)";
	m_ctrlListCtrlCADelItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonCapayitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditCAPayItemNum == "" || m_valueEditCAPayItemCount == "")
		return;
	CString tempString =  m_valueEditCAPayItemNum + "(" + m_valueEditCAPayItemCount + ")";
	m_ctrlListCtrlCAPayItem.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonCarandompayitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditCARandomPayItemNum == "" || m_valueEditCARandomPayItemCount == "")
		return;
	CString tempString;
	if(m_ctrlListCtrlCARandomPayItem.GetItemCount() != 0)
	{
		tempString += " OR ";
		tempString +=  m_valueEditCARandomPayItemNum + "(" + m_valueEditCARandomPayItemCount + "," + m_valueEditCARandomPayProb100 + ")";
	}
	else
		tempString =  m_valueEditCARandomPayItemNum + "(" + m_valueEditCARandomPayItemCount + "," + m_valueEditCARandomPayProb100 + ")";
	m_ctrlListCtrlCARandomPayItem.InsertItem(m_ctrlListCtrlCARandomPayItem.GetItemCount(), tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonCamixitem() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_valueEditCAMixItemNum == "" || m_valueEditCAMixItemCount == "")
		return;
	CString tempString;
	if(m_ctrlListCtrlCAMixItem.GetItemCount() != 0)
	{
		tempString += " + ";
		tempString +=  m_valueEditCAMixItemNum + "(" + m_valueEditCAMixItemCount + ")";
	}
	else
		tempString =  m_valueEditCAMixItemNum + "(" + m_valueEditCAMixItemCount + ")";
	m_ctrlListCtrlCAMixItem.InsertItem(m_ctrlListCtrlCAMixItem.GetItemCount(), tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnDblClickListSpayitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlPayItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlPayItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlPayItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListSdelitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlSDelItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlSDelItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlSDelItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListCreqitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlCReqItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlCReqItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlCReqItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListCadelitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlCADelItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlCADelItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlCADelItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListCapayitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlCAPayItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlCAPayItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlCAPayItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListCarandompayitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlCARandomPayItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlCARandomPayItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlCARandomPayItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListReqItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlReqItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlReqItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlReqItem.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListReqindex(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlReqIndex.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlReqIndex.GetNextSelectedItem(pos);

	m_ctrlListCtrlReqIndex.DeleteItem(nItem);
	*pResult = 0;
}

void CQuestEditorDlg::OnDblclkListRecvreqindex(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlRecvReqIndex.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlRecvReqIndex.GetNextSelectedItem(pos);

	m_ctrlListCtrlRecvReqIndex.DeleteItem(nItem);
	*pResult = 0;
}


void CQuestEditorDlg::OnDblclkListCamixitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlListCtrlCAMixItem.GetFirstSelectedItemPosition();
	int nItem = m_ctrlListCtrlCAMixItem.GetNextSelectedItem(pos);

	m_ctrlListCtrlCAMixItem.DeleteItem(nItem);
	*pResult = 0;
}


void CQuestEditorDlg::OnButtonQuestrefresh() 
{
	// TODO: Add your control notification handler code here
	InitQuestEditor();
}

void CQuestEditorDlg::InitQuestEditor()
{
	m_valueEditQuestIndex = _T("");
	m_valueEditQuestName = _T("");
	m_valueEditNPCIndex = _T("");
	m_valueEditPreTalk = _T("");
	m_valueEditMidTalk = _T("");
	m_valueEditSuccessTalk = _T("");
	m_valueEditDescription = _T("");
	m_valueEditFailTalk = _T("");
	m_valueEditEventMap = _T("");
	m_valueEditNextQuestIndex = _T("");
	m_valueEditPartyQuestMemberCount = _T("");
	m_valueEditDropItemToAllCharacter = _T("");
	m_valueEditTimeLimitInMinites = _T("");
	m_valueEditDiscardableQuest = _T("");
	m_valueCheckBGEAR = FALSE;
	m_valueCheckMGEAR = FALSE;
	m_valueCheckAGEAR = FALSE;
	m_valueCheckIGEAR = FALSE;
	m_valueEditReqItemName = _T("");
	m_valueEditReqItemCount = _T("");
	m_valueEditReqItems = _T("");
	m_valueEditReqIndex = _T("");
	m_valueEditRecvReqIndex = _T("");
	m_valueEditReqLevelMin = _T("");
	m_valueEditReqLevelMax = _T("");
	m_valueEditReqAttackMin = _T("");
	m_valueEditReqAttackMax = _T("");
	m_valueEditReqDefenseMin = _T("");
	m_valueEditReqDefenseMax = _T("");
	m_valueEditReqFuelMin = _T("");
	m_valueEditReqFuelMax = _T("");
	m_valueEditReqSoulMin = _T("");
	m_valueEditReqSoulMax = _T("");
	m_valueEditReqShieldMin = _T("");
	m_valueEditReqShieldMax = _T("");
	m_valueEditReqDodgeMin = _T("");
	m_valueEditReqDodgeMax = _T("");
	m_valueEditReqGuild = _T("");
	m_valueCheckNormal = FALSE;
	m_valueCheckVCN = FALSE;
	m_valueCheckANI = FALSE;
	m_valueCheckRRP = FALSE;
	m_valueCheckALL = FALSE;
	m_valueEditPayItemNum = _T("");
	m_valueEditPayItemCount = _T("");
	m_valueEditPayItemPerItem = _T("");
	m_valueCheckPayItemBGear = FALSE;
	m_valueCheckPayItemIGear = FALSE;
	m_valueCheckPayItemMGear = FALSE;
	m_valueCheckPayItemAGear = FALSE;
	m_valueCheckPayItemALL = FALSE;
	m_valueEditSDelItemNum = _T("");
	m_valueEditSDelItemCount = _T("");
	m_valueEditStartMapIndex = _T("");
	m_valueEditCReqNPCIndex = _T("");
	m_valueEditCReqItemNum = _T("");
	m_valueEditCReqItemCount = _T("");
	m_valueEditCReqPerItem = _T("");
	m_valueCheckCReqBGear = FALSE;
	m_valueEditCReqMonsterUniqueNum = _T("");
	m_valueEditCReqMinCount = _T("");
	m_valueEditCReqMaxCount = _T("");
	m_valueEditCReqProbability = _T("");
	m_valueEditMeetObjectIndex = _T("");
	m_valueEditMeetObjectMapIndex = _T("");
	m_valueEditPayBonusStat = _T("");
	m_valueEditPayBonusSkillPoint = _T("");
	m_valueEditPayFame = _T("");
	m_valueEditPayStatus = _T("");
	m_valueEditPayLevel = _T("");
	m_valueEditPayExperience = _T("");
	m_valueEditPayWarpMapIndex = _T("");
	m_valueCheckCADelItemAll = FALSE;
	m_valueEditCADelItemNum = _T("");
	m_valueEditCADelItemCount = _T("");
	m_valueEditCAPayItemNum = _T("");
	m_valueEditCAPayItemCount = _T("");
	m_valueEditCAPayPropensity = _T("");
	m_valueEditCACinemaNum = _T("");
	m_valueCheckCANormal = FALSE;
	m_valueCheckCAVCN = FALSE;
	m_valueCheckCAANI = FALSE;
	m_valueCheckCARRP = FALSE;
	m_valueCheckCAALL = FALSE;
	m_valueCheckBIQuestAttrisCitiwar = FALSE;
	m_valueCheckBIQuestAttriNeverCompletionByUser = FALSE;
	m_valueEditCARandomPayItemNum = _T("");
	m_valueEditCARandomPayItemCount = _T("");
	m_valueEditCARandomPayProb100 = _T("");
	m_valueEditCARandomPayPerItemNum = _T("");
	m_valueEditCAMixItemNum = _T("");
	m_valueEditCAMixItemCount = _T("");
	m_valueEditCAMixItemTargetItem = _T("");
	m_valueEditCAMixItemProb = _T("");
	m_valueEditReqMapNum = _T("");
	m_valueEditReqMapX = _T("");
	m_valueEditReqMapY = _T("");
	m_valueEditReqMapR = _T("");
	m_valueEditCReqMeetArea = _T("");
	m_valueEditCReqMeetAreaX = _T("");
	m_valueEditCReqMeetAreaY = _T("");
	m_valueEditCReqMeetAreaR = _T("");

	m_ctrlListCtrlCAMixItem.DeleteAllItems();
	m_ctrlListCtrlCARandomPayItem.DeleteAllItems();
	m_ctrlListCtrlCAPayItem.DeleteAllItems();
	m_ctrlListCtrlCADelItem.DeleteAllItems();
	m_ctrlListCtrlCReqItem.DeleteAllItems();
	m_ctrlListCtrlSDelItem.DeleteAllItems();
	m_ctrlListCtrlPayItem.DeleteAllItems();
	m_ctrlListCtrlRecvReqIndex.DeleteAllItems();
	m_ctrlListCtrlReqIndex.DeleteAllItems();
	m_ctrlListCtrlReqItem.DeleteAllItems();

	m_ctrlComboQuestEType.SetCurSel(-1);
	m_ctrlComboQuestSType.SetCurSel(-1);
	m_ctrlComboQuestType.SetCurSel(-1);
	m_ctrlComboQuestKind.SetCurSel(-1);

	UpdateData(FALSE);
}

BOOL CQuestEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CQuestEditorDlg::OnKeydownListReqitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

BOOL CQuestEditorDlg::GetQuestFilesPath(char *o_szQuestDefinedPath, vector<string> *o_pVecQuestFiles)
{
	CFileDialog pDlg(TRUE, "cfg", "*.cfg", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,"Quest File(*.cfg)|*.cfg|");
	pDlg.m_ofn.lpstrFile	= new TCHAR[102400];
	pDlg.m_ofn.lpstrFile[0]	= '\0';
	pDlg.m_ofn.nMaxFile		= 102400;

	int rt = pDlg.DoModal();
	if(IDOK == rt)
	{
		POSITION pos = pDlg.GetStartPosition();
		while(pos)
		{
			CString strPath = pDlg.GetNextPathName( pos );
			char* szFileName = strrchr( (char*)(LPCTSTR)strPath, '\\') + 1;

			if(0 == strcmp(szFileName, QUEST_USER_DEFINED_VARIABLES_FILE_NAME))
			{
				strcpy(o_szQuestDefinedPath, strPath);
			}
			else
			{
				o_pVecQuestFiles->push_back((LPCTSTR)strPath);
			}
		}
	}
	delete pDlg.m_ofn.lpstrFile;

	if(0 == strcmp(o_szQuestDefinedPath, "")
		|| o_pVecQuestFiles->size() == 0)
	{
		return FALSE;
	}

	return TRUE;
}

void CQuestEditorDlg::OnButtonQuestSelect() 
{
	// TODO: Add your control notification handler code here
	ReflectQuestEditor();
}

void CQuestEditorDlg::ReflectQuestEditor()
{
	InitQuestEditor(); // QuestEditor 다이얼로그 초기화
	CString szSelectQuestName;
	m_ctrlQuestLoad.GetWindowText(szSelectQuestName);
	//선택된 퀘스트 이름으로 퀘스트를 QuestLoad 다이얼로그에 반영시킨다.
	itr = QuestInfoMap.begin();
	while (itr != QuestInfoMap.end())
	{
		CQuest *pQuest = itr->second;
		if(strcmp(pQuest->QuestName, (char*)(LPCTSTR)szSelectQuestName) == 0)
		{
			CString szTemp;

			// Basic Info
			m_valueEditQuestIndex.Format("%d", pQuest->QuestIndex);
			m_valueEditQuestName = pQuest->QuestName;
			
			if(pQuest->QuestKind == QUEST_KIND_NORMAL)
				m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_NORMAL");
			else if(pQuest->QuestKind == QUEST_KIND_SIDESTORY)					// 2006-03-15 by cmkwon, QUEST_KIND_COLLECTION를 변경함
				m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_SIDESTORY");
			else if(pQuest->QuestKind == QUEST_KIND_SCENARIO)
				m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_SCENARIO");

			if(pQuest->QuestType == QUEST_TYPE_SINGLE)
				m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_SINGLE");
			else if(pQuest->QuestType == QUEST_TYPE_SERIES)
				m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_SERIES");
			else if(pQuest->QuestType == QUEST_TYPE_REPEATABLE)
				m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_REPEATABLE");

			if(pQuest->QuestStartType == QUEST_START_TYPE_LEVEL_CHANGE)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_LEVEL_CHANGE");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_EXP_CHANGE)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_EXP_CHANGE");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_MEET_NPC)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_MEET_NPC");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_ENTER_BUILDING)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_ENTER_BUILDING");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_ITEM_CLICK)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_ITEM_CLICK");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_COUPON)
				m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_COUPON");

			if(pQuest->QuestEndType == QUEST_END_TYPE_IMMEDIATE)
				m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_IMMEDIATE");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_REQESTED_BY_CLIENT)
				m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_REQESTED_BY_CLIENT");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
				m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_LEVEL_UP");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_TIME_OVER)
				m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_TIME_OVER");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_SELECT_INFLUENCE)
				m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_SELECT_INFLUENCE");

			m_valueEditNPCIndex.Format("%d", pQuest->QuestNPCInfo.NPCIndex);
			m_valueEditPreTalk = pQuest->QuestNPCInfo.PreTalk;
			m_valueEditMidTalk = pQuest->QuestNPCInfo.MidTalk;
			m_valueEditSuccessTalk = pQuest->QuestNPCInfo.SuccessTalk;
			m_valueEditFailTalk = pQuest->QuestNPCInfo.FailTalk;
			m_valueEditDescription = pQuest->QuestDescription;

			if(pQuest->EventMapIndex != 0)
			m_valueEditEventMap.Format("%d", pQuest->EventMapIndex);
			if(pQuest->NextQuestIndex != 0)
			m_valueEditNextQuestIndex.Format("%d", pQuest->NextQuestIndex);

			if(pQuest->PartyQuestMemberCount != 0)
			m_valueEditPartyQuestMemberCount.Format("%d", pQuest->PartyQuestMemberCount);
			if(pQuest->DropItemToAllAttackCharacter != 0)
			m_valueEditDropItemToAllCharacter.Format("%d", pQuest->DropItemToAllAttackCharacter);
			if(pQuest->TimeLimitInMinutes != 0)
			m_valueEditTimeLimitInMinites.Format("%d", pQuest->TimeLimitInMinutes);
			if(pQuest->IsDiscardable != 0)
			m_valueEditDiscardableQuest.Format("%d", pQuest->IsDiscardable);
			if(pQuest->IsCityWar == TRUE)
				m_valueCheckBIQuestAttrisCitiwar = TRUE;
			if(pQuest->IsNeverCompletionByUser == TRUE)
			m_valueCheckBIQuestAttriNeverCompletionByUser = TRUE;

			// Start Requirements
			int tempBit = pQuest->ReqUnitKind; //unitkind
			if(IS_BGEAR((USHORT)tempBit) == TRUE)
				m_valueCheckBGEAR = TRUE;
			if(IS_MGEAR((USHORT)tempBit) == TRUE)
				m_valueCheckMGEAR = TRUE;
			if(IS_AGEAR((USHORT)tempBit) == TRUE)
				m_valueCheckAGEAR = TRUE;
			if(IS_IGEAR((USHORT)tempBit) == TRUE)
				m_valueCheckIGEAR = TRUE;

			if(pQuest->ReqLevel.Min != 0)
				m_valueEditReqLevelMin.Format("%d", pQuest->ReqLevel.Min);
			if(pQuest->ReqLevel.Max != 0x7fffffff)
				m_valueEditReqLevelMax.Format("%d", pQuest->ReqLevel.Max);

			if(pQuest->ReqAttackPart.Min != 0)
				m_valueEditReqAttackMin.Format("%d", pQuest->ReqAttackPart.Min);
			if(pQuest->ReqAttackPart.Max != 0x7fffffff)
				m_valueEditReqAttackMax.Format("%d", pQuest->ReqAttackPart.Max);

			if(pQuest->ReqDefensePart.Min != 0)
				m_valueEditReqDefenseMin.Format("%d", pQuest->ReqDefensePart.Min);
			if(pQuest->ReqDefensePart.Max != 0x7fffffff)
				m_valueEditReqDefenseMax.Format("%d", pQuest->ReqDefensePart.Max);

			if(pQuest->ReqFuelPart.Min != 0)
				m_valueEditReqFuelMin.Format("%d", pQuest->ReqFuelPart.Min);
			if(pQuest->ReqFuelPart.Max != 0x7fffffff)
				m_valueEditReqFuelMax.Format("%d", pQuest->ReqFuelPart.Max);

			if(pQuest->ReqSoulPart.Min != 0)
				m_valueEditReqSoulMin.Format("%d", pQuest->ReqSoulPart.Min);
			if(pQuest->ReqSoulPart.Max != 0x7fffffff)
				m_valueEditReqSoulMax.Format("%d", pQuest->ReqSoulPart.Max);

			if(pQuest->ReqShieldPart.Min != 0)
				m_valueEditReqShieldMin.Format("%d", pQuest->ReqShieldPart.Min);
			if(pQuest->ReqShieldPart.Max != 0x7fffffff)
				m_valueEditReqShieldMax.Format("%d", pQuest->ReqShieldPart.Max);

			if(pQuest->ReqDodgePart.Min != 0)
				m_valueEditReqDodgeMin.Format("%d", pQuest->ReqDodgePart.Min);
			if(pQuest->ReqDodgePart.Max != 0x7fffffff)
				m_valueEditReqDodgeMax.Format("%d", pQuest->ReqDodgePart.Max);

			m_valueEditReqGuild.Format("%d",pQuest->ReqGuild);

			int i;
			for (i = 0; i < pQuest->ReqItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->ReqItemVector[i].ItemNum, pQuest->ReqItemVector[i].Count);
				m_ctrlListCtrlReqItem.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->ReqQuestIndexVector.size(); i++)
			{
				szTemp.Format("%d",pQuest->ReqQuestIndexVector[i]);
				m_ctrlListCtrlReqIndex.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->ReqRecvQuestIndexVector.size(); i++)
			{
				szTemp.Format("%d",pQuest->ReqRecvQuestIndexVector[i]);
				m_ctrlListCtrlRecvReqIndex.InsertItem(i, szTemp);
			}

			tempBit = pQuest->ReqInfluenceType; 
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_NORMAL) == TRUE)
				m_valueCheckNormal = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_VCN) == TRUE)
				m_valueCheckVCN = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_ANI) == TRUE)
				m_valueCheckANI = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_RRP) == TRUE)
				m_valueCheckRRP = TRUE;
			if((USHORT)tempBit == (BYTE)0x00FF)
				m_valueCheckALL = TRUE;

			// Start Action
			for (i = 0; i < pQuest->StartPayItemVector.size(); i++)
			{
				szTemp.Format("%d(%d) PER %d TO %d"
					,pQuest->StartPayItemVector[i].ItemNum, pQuest->StartPayItemVector[i].Count
					,pQuest->StartPayItemVector[i].PerItem, pQuest->StartPayItemVector[i].ToUnitKind);
				m_ctrlListCtrlPayItem.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->StartDelItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->StartDelItemVector[i].ItemNum, pQuest->StartDelItemVector[i].Count);
				m_ctrlListCtrlSDelItem.InsertItem(i, szTemp);
			}
			m_valueEditQuestIndex.Format("%d", pQuest->QuestIndex);

			// Completion Requirements
			m_valueEditCReqNPCIndex.Format("%d", pQuest->TargetMeetNPCInfo.NPCIndex);

			for (i = 0; i < pQuest->TargetMonsterItemVector.size(); i++)
			{
				szTemp.Format("%d(%d) FROM MONSTER(%d,%d~%d,%d)"
					,pQuest->TargetMonsterItemVector[i].ItemWithCount.ItemNum, pQuest->TargetMonsterItemVector[i].ItemWithCount.Count
					,pQuest->TargetMonsterItemVector[i].MonsterUniqueNumber, pQuest->vecQuestDropItem[i].MinCount, pQuest->vecQuestDropItem[i].MaxCount
					,pQuest->vecQuestDropItem[i].Probability);
				m_ctrlListCtrlCReqItem.InsertItem(i, szTemp);
			}

			if(pQuest->TargetMeetObjectIndex != 0 && pQuest->TargetMeetObjectMapArea.MapIndex)
			{
				m_valueEditMeetObjectIndex.Format("%d", pQuest->TargetMeetObjectIndex);
				m_valueEditMeetObjectMapIndex.Format("%d", pQuest->TargetMeetObjectMapArea.MapIndex);
			}

			// 2005-10-26 by cmkwon, 몬스터 카운트 추가
			for (i = 0; i < pQuest->TargetMonsterVector.size(); i++)
			{
				szTemp.Format("%d(%d)"
					,pQuest->TargetMonsterVector[i].MonsterUniqueNumber, pQuest->TargetMonsterVector[i].Count);
				m_ctrlList_reqMonsterList.InsertItem(i, szTemp);
			}

			// Completion Action
			if(pQuest->EndPayBonusStat != 0)
			m_valueEditPayBonusStat.Format("%d", pQuest->EndPayBonusStat);
// 2005-11-15 by cmkwon, 삭제함
//			if(pQuest->EndPayBonusSkillPoint != 0)
//			m_valueEditPayBonusSkillPoint.Format("%d", pQuest->EndPayBonusSkillPoint);
			if(pQuest->EndPayFame != 0)
			m_valueEditPayFame.Format("%d", pQuest->EndPayFame);
			if(pQuest->EndPayStatus != 0)
			m_valueEditPayStatus.Format("%d", pQuest->EndPayStatus);
			if(pQuest->EndPayLevel != 0)
			m_valueEditPayLevel.Format("%d", pQuest->EndPayLevel);
			if(pQuest->EndPayExperience != 0)
			m_valueEditPayExperience.Format("%d", pQuest->EndPayExperience);
			if(pQuest->EndPayWarpMapIndex != 0)
			m_valueEditPayWarpMapIndex.Format("%d", pQuest->EndPayWarpMapIndex);

			for (i = 0; i < pQuest->EndDeleteItemVector.size(); i++)
			{
				if(pQuest->EndDeleteItemVector[i].Count == 0x7fffffff)
					szTemp.Format("%d(ALL)",pQuest->EndDeleteItemVector[i].ItemNum);
				else
					szTemp.Format("%d(%d)",pQuest->EndDeleteItemVector[i].ItemNum, pQuest->EndDeleteItemVector[i].Count);
				m_ctrlListCtrlCADelItem.InsertItem(i, szTemp);
			}
			for (i = 0; i < pQuest->EndPayItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->EndPayItemVector[i].ItemNum, pQuest->EndPayItemVector[i].Count);
				m_ctrlListCtrlCAPayItem.InsertItem(i, szTemp);
			}
			for (i = 0; i < pQuest->EndMissionMasterPayItemVector.size(); i++)
			{// 2008-12-02 by dhjin, 미션마스터
				szTemp.Format("%d(%d)",pQuest->EndMissionMasterPayItemVector[i].ItemNum, pQuest->EndMissionMasterPayItemVector[i].Count);
				m_ctrlListCtrlCAPayItem.InsertItem(i, szTemp);
			}
			if(&pQuest->EndRandomPayItemVector[0] != 0)
			{
				for (i = 0; i < pQuest->EndRandomPayItemVector[0].NumOfItems; i++)
				{
					szTemp.Format("%d(%d,%d)",pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].ItemNum, pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].Count,pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].Prob100);
					m_ctrlListCtrlCARandomPayItem.InsertItem(i, szTemp);
				}
			}
			for (i = 0; i < pQuest->EndPayMixItemVector.size(); i++)
			{
				for(int j= 0 ; j<pQuest->EndPayMixItemVector[i].NumOfSourceItems;j++)
				{
					szTemp.Format("%d(%d)",pQuest->EndPayMixItemVector[i].SourceItem[j].ItemNum, pQuest->EndPayMixItemVector[i].SourceItem[j].Count);
					m_ctrlListCtrlCAMixItem.InsertItem(i, szTemp);
				}
				m_valueEditCAMixItemTargetItem.Format("%d(%d)", pQuest->EndPayMixItemVector[i].TargetItemNum, pQuest->EndPayMixItemVector[i].MixingProbability);
			}

			if(pQuest->EndPayPropensity != 0)
			m_valueEditCAPayPropensity.Format("%d", pQuest->EndPayPropensity);
			if(pQuest->EndCinemaNum != 0)
			m_valueEditCACinemaNum.Format("%d", pQuest->EndCinemaNum);

			tempBit = pQuest->EndInfluenceType; 
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_NORMAL) == TRUE)
				m_valueCheckCANormal = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_VCN) == TRUE)
				m_valueCheckCAVCN = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_ANI) == TRUE)
				m_valueCheckCAANI = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_RRP) == TRUE)
				m_valueCheckCARRP = TRUE;
			if((USHORT)tempBit == (BYTE)0x00FF)
				m_valueCheckCAALL = TRUE;
			break;
		}
		itr++;
	}
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnButtonInsertReqMonster() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(0 == m_editnMonsterUnikind || 0 == m_editnMonsterCount)
		return;

	CString tempString;
	tempString.Format("%d(%d)", m_editnMonsterUnikind, m_editnMonsterCount);
	m_ctrlList_reqMonsterList.InsertItem(0,tempString);
	UpdateData(FALSE);
}

void CQuestEditorDlg::OnDblclkListReqMonster(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_ctrlList_reqMonsterList.GetFirstSelectedItemPosition();
	int nItem = m_ctrlList_reqMonsterList.GetNextSelectedItem(pos);

	m_ctrlList_reqMonsterList.DeleteItem(nItem);
	*pResult = 0;
}

BOOL CQuestEditorDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	INT uTemp;
    uTemp = zDelta;
	uTemp = GetScrollPos(SB_VERT);
	
	int vsize;

	RECT rt;
	GetWindowRect(&rt);
	vsize = (nHeightOfMyDlg-rt.bottom+rt.top+50)/10;

	TRACE("PAGEDOWN %d\n",vsize/10*10);
    if(zDelta <0)
	{
		if(uTemp <=90)
		{
			uTemp +=10;
			ScrollWindow(0,-vsize/10*10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else 
		{
			ScrollWindow(0,uTemp-100, NULL, NULL);
			uTemp = 100;
			SetScrollPos(SB_VERT, uTemp*vsize/10, TRUE);
		}
	}
	else
	{
		if(uTemp >=10) 
		{
			uTemp -=10;
			ScrollWindow(0,vsize/10*10, NULL, NULL);
			SetScrollPos(SB_VERT, uTemp, TRUE);
		}
        else 
		{
			ScrollWindow(0,uTemp*vsize/10, NULL, NULL);
			SetScrollPos(SB_VERT, 0, TRUE);
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
