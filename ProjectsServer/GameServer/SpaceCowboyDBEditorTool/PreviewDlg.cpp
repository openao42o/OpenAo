// PreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "PreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg dialog


CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreviewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_pQuestEditorDlg = (CQuestEditorDlg*)pParent;
}


void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewDlg)
	DDX_Control(pDX, IDC_RICHEDIT_PREVIEW, m_ctrlRichEditPreview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CPreviewDlg)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_FILESAVE, OnButtonFilesave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewDlg message handlers

BOOL CPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InsertQuest() == false)
	{
		EndDialog(0);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CPreviewDlg::InsertQuest()
{
	m_pQuestEditorDlg->UpdateData();

	if(CheckValidation() == false)
		return false;
	CString szQuest, sztempString;
	szQuest = "quest (" + m_pQuestEditorDlg->m_valueEditQuestIndex + ")\r\n";
	szQuest += "{\r\n";
					//BASIC_INFO
	szQuest +=		"\tBASIC_INFO\r\n";
	szQuest +=		"\t{\r\n";
	szQuest +=		"\t	QUEST_INDEX	\t\t= " + m_pQuestEditorDlg->m_valueEditQuestIndex +";\r\n";
	szQuest +=		"\t	QUEST_NAME	\t\t= '" + m_pQuestEditorDlg->m_valueEditQuestName +"';\r\n";
	m_pQuestEditorDlg->m_ctrlComboQuestKind.GetWindowText(sztempString);
	szQuest +=		"\t\tQUEST_KIND	= " + sztempString +";\r\n";
	m_pQuestEditorDlg->m_ctrlComboQuestType.GetWindowText(sztempString);
	szQuest +=		"\t\tQUEST_TYPE	= " + sztempString + ";\r\n";
	m_pQuestEditorDlg->m_ctrlComboQuestSType.GetWindowText(sztempString);
	if(sztempString != "")
	szQuest +=		"\t\tQUEST_START_TYPE	= " + sztempString +";\r\n";
	m_pQuestEditorDlg->m_ctrlComboQuestEType.GetWindowText(sztempString);
	if(sztempString != "")
	szQuest +=		"\t\tQUEST_END_TYPE	= " + sztempString +";\r\n";
	szQuest +=		"\t\tQUEST_NPC_INFO\r\n";
	szQuest +=		"\t\t{\r\n";
	szQuest +=		"\t\t\tNPC_INDEX	= " + m_pQuestEditorDlg->m_valueEditNPCIndex +";\r\n";
	szQuest +=		"\t\t\tPRE_TALK	= '" + m_pQuestEditorDlg->m_valueEditPreTalk +"';\r\n";
	szQuest +=		"\t\t\tMID_TALK	= '" + m_pQuestEditorDlg->m_valueEditMidTalk +"';\r\n";
	szQuest +=		"\t\t\tSUCCESS_TALK	= '" + m_pQuestEditorDlg->m_valueEditSuccessTalk +"';\r\n";
	szQuest +=		"\t\t\tFAIL_TALK	= '" + m_pQuestEditorDlg->m_valueEditFailTalk +"';\r\n";
	szQuest +=		"\t\t};\r\n";
	szQuest +=		"\t\tDESCRIPTION	= '" + m_pQuestEditorDlg->m_valueEditDescription +"';\r\n";
	if(m_pQuestEditorDlg->m_valueEditEventMap != "")
	szQuest +=		"\t\tEVENT_MAP=	" + m_pQuestEditorDlg->m_valueEditEventMap +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditNextQuestIndex != "")
	szQuest +=		"\t\tNEXT_QUEST_INDEX=	" + m_pQuestEditorDlg->m_valueEditNextQuestIndex +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPartyQuestMemberCount != "")
	szQuest +=		"\t\tPARTY_QUEST_MEMBER_COUNT	= " + m_pQuestEditorDlg->m_valueEditPartyQuestMemberCount +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditDropItemToAllCharacter != "")
	szQuest +=		"\t\tDROP_ITEM_TO_ALL_ATTACK_CHARACTER	= " + m_pQuestEditorDlg->m_valueEditDropItemToAllCharacter +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditTimeLimitInMinites != "")
	szQuest +=		"\t\tTIME_LIMIT_IN_MINUTES	= " + m_pQuestEditorDlg->m_valueEditTimeLimitInMinites +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditDiscardableQuest != "")
	szQuest +=		"\t\tDISCARDABLE_QUEST	= " + m_pQuestEditorDlg->m_valueEditDiscardableQuest +";\r\n";
	if(m_pQuestEditorDlg->m_valueCheckBIQuestAttrisCitiwar == TRUE)
	szQuest +=		"\t\tBI_QUEST_ATTRI_IS_CITYWAR	= TRUE;\r\n";
	if(m_pQuestEditorDlg->m_valueCheckBIQuestAttriNeverCompletionByUser == TRUE)
	szQuest +=		"\t\tBI_QUEST_ATTRI_NEVER_COMPLETION_BY_USER	= TRUE;\r\n";
	szQuest +=		"\t};\r\n";
					//START_REQ
	szQuest +=		"\tSTART_REQ\r\n";
	szQuest +=		"\t{\r\n";
	int tempbit = 0;
	if(m_pQuestEditorDlg->m_valueCheckBGEAR == TRUE)
		tempbit += UNITKIND_BGEAR_MASK;
	if(m_pQuestEditorDlg->m_valueCheckIGEAR == TRUE)
		tempbit += UNITKIND_IGEAR_MASK;
	if(m_pQuestEditorDlg->m_valueCheckMGEAR == TRUE)
		tempbit += UNITKIND_MGEAR_MASK;
	if(m_pQuestEditorDlg->m_valueCheckAGEAR == TRUE)
		tempbit += UNITKIND_AGEAR_MASK;
	if(m_pQuestEditorDlg->m_valueCheckALL == TRUE)
		tempbit = UNITKIND_ALL_MASK;
	sztempString.Format("%d", tempbit);
	if(sztempString != "0")
	szQuest +=		"\t\tS_REQ_UNIT_KIND	= " + sztempString +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqLevelMin != "")
	szQuest +=		"\t\tS_REQ_LEVEL	= (" + m_pQuestEditorDlg->m_valueEditReqLevelMin + "~" + m_pQuestEditorDlg->m_valueEditReqLevelMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqAttackMin != "")
	szQuest +=		"\t\tS_REQ_ATTACK_PART	= (" + m_pQuestEditorDlg->m_valueEditReqAttackMin + "~" + m_pQuestEditorDlg->m_valueEditReqAttackMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqDefenseMin != "")
	szQuest +=		"\t\tS_REQ_DEFENSE_PART	= (" + m_pQuestEditorDlg->m_valueEditReqDefenseMin + "~" + m_pQuestEditorDlg->m_valueEditReqDefenseMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqFuelMin != "")
	szQuest +=		"\t\tS_REQ_FUEL_PART	= (" + m_pQuestEditorDlg->m_valueEditReqFuelMin + "~" + m_pQuestEditorDlg->m_valueEditReqFuelMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqSoulMin != "")
	szQuest +=		"\t\tS_REQ_SOUL_PART	= (" + m_pQuestEditorDlg->m_valueEditReqSoulMin + "~" + m_pQuestEditorDlg->m_valueEditReqSoulMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqShieldMin != "")
	szQuest +=		"\t\tS_REQ_SHIELD_PART	= (" + m_pQuestEditorDlg->m_valueEditReqShieldMin + "~" + m_pQuestEditorDlg->m_valueEditReqShieldMax +");\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqDodgeMin != "")
	szQuest +=		"\t\tS_REQ_DODGE_PART	= (" + m_pQuestEditorDlg->m_valueEditReqDodgeMin + "~" + m_pQuestEditorDlg->m_valueEditReqDodgeMax +");\r\n";
	
	int i;
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlReqItem.GetItemCount(); i++)
	szQuest +=		"\t\tS_REQ_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlReqItem.GetItemText(i,0)+";\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlReqIndex.GetItemCount(); i++)
	szQuest +=		"\t\tS_REQ_QUEST_INDEX	=" + m_pQuestEditorDlg->m_ctrlListCtrlReqIndex.GetItemText(i,0)+";\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlRecvReqIndex.GetItemCount(); i++)
	szQuest +=		"\t\tS_REQ_RECV_QUEST_INDEX	=" + m_pQuestEditorDlg->m_ctrlListCtrlRecvReqIndex.GetItemText(i,0)+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqGuild != "")
	szQuest +=		"\t\tS_REQ_GUILD	= " + m_pQuestEditorDlg->m_valueEditReqGuild +";\r\n";

	tempbit = 0;
	if(m_pQuestEditorDlg->m_valueCheckNormal == TRUE)
		tempbit += INFLUENCE_TYPE_NORMAL;
	if(m_pQuestEditorDlg->m_valueCheckVCN == TRUE)
		tempbit += INFLUENCE_TYPE_VCN;
	if(m_pQuestEditorDlg->m_valueCheckANI == TRUE)
		tempbit += INFLUENCE_TYPE_ANI;
	if(m_pQuestEditorDlg->m_valueCheckRRP == TRUE)
		tempbit += INFLUENCE_TYPE_RRP;
	if(m_pQuestEditorDlg->m_valueCheckALL == TRUE)
		tempbit = INFLUENCE_TYPE_ALL_MASK;
	sztempString.Format("%d", tempbit);
	if(sztempString != "0")
	szQuest +=		"\t\tS_REQ_INFLUENCE_TYPE	= " + sztempString +";\r\n";
	if(m_pQuestEditorDlg->m_valueEditReqMapNum != "" || m_pQuestEditorDlg->m_valueEditReqMapX != "" || m_pQuestEditorDlg->m_valueEditReqMapY != "" || m_pQuestEditorDlg->m_valueEditReqMapR != "")
	szQuest +=		"\t\tS_REQ_START_AREA	= AREA" + m_pQuestEditorDlg->m_valueEditReqMapNum +" (X "+ m_pQuestEditorDlg->m_valueEditReqMapX + ", Y " + m_pQuestEditorDlg->m_valueEditReqMapY + ", R " + m_pQuestEditorDlg->m_valueEditReqMapR +");\r\n";
	szQuest +=		"\t};\r\n";

						//START_ACTION
	szQuest +=		"\tSTART_ACTION\r\n";
	szQuest +=		"\t{\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlPayItem.GetItemCount(); i++)
	szQuest +=		"\t\tS_PAY_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlPayItem.GetItemText(i,0)+";\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlSDelItem.GetItemCount(); i++)
	szQuest +=		"\t\tS_PAY_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlSDelItem.GetItemText(i,0)+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditStartMapIndex != "")
	szQuest +=		"\t\tS_START_MAP_INDEX	= " + m_pQuestEditorDlg->m_valueEditStartMapIndex +";\r\n";
	szQuest +=		"\t};\r\n";

						// COMPLETION_REQ
	szQuest +=		"\tCOMPLETION_REQ\r\n";
	szQuest +=		"\t{\r\n";
	szQuest +=		"\t\tC_REQ_MEET_NPC_INFO\r\n";
	szQuest +=		"\t\t{\r\n";
	if(m_pQuestEditorDlg->m_valueEditCReqNPCIndex != "")
	szQuest +=		"\t\t\tNPC_INDEX	= " + m_pQuestEditorDlg->m_valueEditCReqNPCIndex +";\r\n";
	szQuest +=		"\t\t};\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlCReqItem.GetItemCount(); i++)
	szQuest +=		"\t\tC_REQ_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlCReqItem.GetItemText(i,0)+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditMeetObjectIndex != "" || m_pQuestEditorDlg->m_valueEditMeetObjectMapIndex != "")
	szQuest +=		"\t\tC_REQ_MEET_OBJ_INDEX	= " + m_pQuestEditorDlg->m_valueEditMeetObjectIndex + " AREA "+m_pQuestEditorDlg->m_valueEditMeetObjectMapIndex+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditCReqMeetArea != "" || m_pQuestEditorDlg->m_valueEditCReqMeetAreaX != "" || m_pQuestEditorDlg->m_valueEditCReqMeetAreaY != "" || m_pQuestEditorDlg->m_valueEditCReqMeetAreaR != "")
	szQuest +=		"\t\tC_REQ_MEET_AREA	= AREA" + m_pQuestEditorDlg->m_valueEditCReqMeetArea +" (X "+ m_pQuestEditorDlg->m_valueEditCReqMeetAreaX + ", Y " + m_pQuestEditorDlg->m_valueEditCReqMeetAreaY + ", R " + m_pQuestEditorDlg->m_valueEditCReqMeetAreaR +");\r\n";
	for(i=0; i < m_pQuestEditorDlg->m_ctrlList_reqMonsterList.GetItemCount(); i++)
	{
		szQuest +=	"\t\tC_REQ_MONSTER\t\t\t=" + m_pQuestEditorDlg->m_ctrlList_reqMonsterList.GetItemText(i,0)+";\r\n";
	}
	szQuest +=		"\t};\r\n";				// end_COMPLETION_REQ
						// COMPLETION_ACTION
	szQuest +=		"\tCOMPLETION_ACTION\r\n";
	szQuest +=		"\t{\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayBonusStat != "")
	szQuest +=		"\t\tC_PAY_BONUS_STAT	= " + m_pQuestEditorDlg->m_valueEditPayBonusStat+";\r\n";
// 2005-11-15 by cmkwon, »èÁ¦ÇÔ
//	if(m_pQuestEditorDlg->m_valueEditPayBonusSkillPoint != "")
//	szQuest +=		"\t\tC_PAY_BONUS_SKILL_POINT	= " + m_pQuestEditorDlg->m_valueEditPayBonusSkillPoint+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayFame != "")
	szQuest +=		"\t\tC_PAY_FAME	= " + m_pQuestEditorDlg->m_valueEditPayFame+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayStatus != "")
	szQuest +=		"\t\tC_PAY_STATUS	= " + m_pQuestEditorDlg->m_valueEditPayStatus+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayLevel != "")
	szQuest +=		"\t\tC_PAY_LEVEL	= " + m_pQuestEditorDlg->m_valueEditPayLevel+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayExperience != "")
	szQuest +=		"\t\tC_PAY_EXPERIENCE	= " + m_pQuestEditorDlg->m_valueEditPayExperience+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditPayWarpMapIndex != "")
	szQuest +=		"\t\tC_PAY_WARP_MAP_INDEX	= " + m_pQuestEditorDlg->m_valueEditPayWarpMapIndex+";\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlCADelItem.GetItemCount(); i++)
	szQuest +=		"\t\tC_DEL_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlCADelItem.GetItemText(i,0)+";\r\n";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlCAPayItem.GetItemCount(); i++)
	szQuest +=		"\t\tC_PAY_ITEM	=" + m_pQuestEditorDlg->m_ctrlListCtrlCAPayItem.GetItemText(i,0)+";\r\n";

	if(m_pQuestEditorDlg->m_ctrlListCtrlCARandomPayItem.GetItemCount() != 0)
	{
	szQuest +=		"\t\tC_RANDOM_PAY_ITEM	=";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlCARandomPayItem.GetItemCount(); i++)
	szQuest += m_pQuestEditorDlg->m_ctrlListCtrlCARandomPayItem.GetItemText(i,0);
	if(m_pQuestEditorDlg->m_valueEditCARandomPayPerItemNum != "")
	szQuest += " PER " + m_pQuestEditorDlg->m_valueEditCARandomPayPerItemNum + ";\r\n";
	}

	if(m_pQuestEditorDlg->m_ctrlListCtrlCAMixItem.GetItemCount() != 0)
	{
	szQuest +=		"\t\tC_MIX_ITEM	=";
	for(i = 0; i<m_pQuestEditorDlg->m_ctrlListCtrlCAMixItem.GetItemCount(); i++)
	szQuest += m_pQuestEditorDlg->m_ctrlListCtrlCAMixItem.GetItemText(i,0);
	if(m_pQuestEditorDlg->m_valueEditCAMixItemTargetItem != "")
	szQuest += " -> " + m_pQuestEditorDlg->m_valueEditCAMixItemTargetItem + " PROB_10K (";
	szQuest += m_pQuestEditorDlg->m_valueEditCAMixItemProb +");\r\n";
	}

	if(m_pQuestEditorDlg->m_valueEditCAPayPropensity != "")
	szQuest +=		"\t\tC_PAY_PROPENSITY	= " + m_pQuestEditorDlg->m_valueEditCAPayPropensity+";\r\n";
	if(m_pQuestEditorDlg->m_valueEditCACinemaNum != "")
	szQuest +=		"\t\tC_CINEMA_NUM	= " + m_pQuestEditorDlg->m_valueEditCACinemaNum+";\r\n";
	tempbit = 0;
	if(m_pQuestEditorDlg->m_valueCheckCANormal == TRUE)
		tempbit += INFLUENCE_TYPE_NORMAL;
	if(m_pQuestEditorDlg->m_valueCheckCAVCN == TRUE)
		tempbit += INFLUENCE_TYPE_VCN;
	if(m_pQuestEditorDlg->m_valueCheckCAANI == TRUE)
		tempbit += INFLUENCE_TYPE_ANI;
	if(m_pQuestEditorDlg->m_valueCheckCARRP == TRUE)
		tempbit += INFLUENCE_TYPE_RRP;
	if(m_pQuestEditorDlg->m_valueCheckCAALL == TRUE)
		tempbit = INFLUENCE_TYPE_ALL_MASK;
	sztempString.Format("%d", tempbit);
	if(sztempString!= "0")
	szQuest +=		"\t\tC_PAY_INFLUENCE_TYPE	= " + sztempString +";\r\n";
	szQuest +=		"\t};\r\n";
	szQuest +=		"};\r\n";
	m_ctrlRichEditPreview.ReplaceSel(szQuest);
	return true;
}

void CPreviewDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CPreviewDlg::OnButtonFilesave() 
{
	// TODO: Add your control notification handler code here
	CFileDialog pDlg(FALSE, "cfg", "*.cfg", OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"Quest File(*.cfg)|*.cfg|");
	int rt = pDlg.DoModal();
	if(rt ==IDOK)
	{
		UpdateData(TRUE);

		HANDLE handle = CreateFile(pDlg.GetPathName(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		CString szPreview;
		m_ctrlRichEditPreview.GetWindowText(szPreview);

		DWORD writtenBytes;
		WriteFile(handle, szPreview, szPreview.GetLength(), &writtenBytes, NULL); 
		CloseHandle(handle);
		return;
	}
	else if(rt ==IDCANCEL)
	{
		return;
	}
}

BOOL CPreviewDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreTranslateMessage(pMsg);
}

bool CPreviewDlg::CheckValidation()
{
	if(m_pQuestEditorDlg->m_valueEditQuestIndex == "")
	{
		AfxMessageBox("Please Insert Quest Index");
		return false;
	}

	if(m_pQuestEditorDlg->m_valueEditQuestName == "")
	{
		AfxMessageBox("Please Insert Quest Name");
		return false;
	}
	
	return true;
}
