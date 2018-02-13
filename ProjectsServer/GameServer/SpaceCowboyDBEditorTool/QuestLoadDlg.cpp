// QuestLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "QuestLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "QuestLoaderParser.h"
#include "Quest.h"
#include <assert.h>

/////////////////////////////////////////////////////////////////////////////
// CQuestLoadDlg dialog


CQuestLoadDlg::CQuestLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestLoadDlg)
	
	//}}AFX_DATA_INIT
	m_pQuestEditorDlg = (CQuestEditorDlg*)pParent; // 부모 클래스 포인터 저장
	AfxInitRichEdit();
}

CQuestLoadDlg::~CQuestLoadDlg()
{
	itr = QuestInfoMap.begin();
	while (itr != QuestInfoMap.end())
	{
		delete itr->second;
		itr++;
	}
}

void CQuestLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestLoadDlg)
	DDX_Control(pDX, IDC_COMBO_QUESTLIST, m_ctrlQuestLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CQuestLoadDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestLoadDlg message handlers

BOOL CQuestLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	char szQuestDefinedPath[MAX_PATH];
	util::zero(szQuestDefinedPath, MAX_PATH);
	vector<string> vecQuestFiles;

	if(FALSE == GetQuestFilesPath(szQuestDefinedPath, &vecQuestFiles))
	{
		AfxMessageBox("File Open Fail");
		EndDialog(0);
		return 0;
	}

	if (!QuestLoaderParser::ParseFiles(szQuestDefinedPath, &vecQuestFiles, &QuestInfoMap))
	{
		return FALSE;
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

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CQuestLoadDlg::GetQuestFilesPath(char *o_szQuestDefinedPath, vector<string> *o_pVecQuestFiles)
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

	if(0 == strcmp(o_szQuestDefinedPath, "")
		|| o_pVecQuestFiles->size() == 0)
	{
		return FALSE;
	}

	return TRUE;
}

void CQuestLoadDlg::ReflectQuestEditor()
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
			m_pQuestEditorDlg->m_valueEditQuestIndex.Format("%d", pQuest->QuestIndex);
			m_pQuestEditorDlg->m_valueEditQuestName = pQuest->QuestName;
			
			if(pQuest->QuestKind == QUEST_KIND_NORMAL)
				m_pQuestEditorDlg->m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_NORMAL");
			else if(pQuest->QuestKind == QUEST_KIND_SIDESTORY)					// 2006-03-15 by cmkwon, QUEST_KIND_COLLECTION를 변경함
				m_pQuestEditorDlg->m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_SIDESTORY");
			else if(pQuest->QuestKind == QUEST_KIND_SCENARIO)
				m_pQuestEditorDlg->m_ctrlComboQuestKind.SetWindowText("$QUEST_KIND_SCENARIO");

			if(pQuest->QuestType == QUEST_TYPE_SINGLE)
				m_pQuestEditorDlg->m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_SINGLE");
			else if(pQuest->QuestType == QUEST_TYPE_SERIES)
				m_pQuestEditorDlg->m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_SERIES");
			else if(pQuest->QuestType == QUEST_TYPE_REPEATABLE)
				m_pQuestEditorDlg->m_ctrlComboQuestType.SetWindowText("$QUEST_TYPE_REPEATABLE");

			if(pQuest->QuestStartType == QUEST_START_TYPE_LEVEL_CHANGE)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_LEVEL_CHANGE");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_EXP_CHANGE)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_EXP_CHANGE");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_MEET_NPC)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_MEET_NPC");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_ENTER_BUILDING)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_ENTER_BUILDING");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_ITEM_CLICK)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_ITEM_CLICK");
			else if(pQuest->QuestStartType == QUEST_START_TYPE_COUPON)
				m_pQuestEditorDlg->m_ctrlComboQuestSType.SetWindowText("$QUEST_START_TYPE_COUPON");

			if(pQuest->QuestEndType == QUEST_END_TYPE_IMMEDIATE)
				m_pQuestEditorDlg->m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_IMMEDIATE");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_REQESTED_BY_CLIENT)
				m_pQuestEditorDlg->m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_REQESTED_BY_CLIENT");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
				m_pQuestEditorDlg->m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_LEVEL_UP");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_TIME_OVER)
				m_pQuestEditorDlg->m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_TIME_OVER");
			else if(pQuest->QuestEndType == QUEST_END_TYPE_SELECT_INFLUENCE)
				m_pQuestEditorDlg->m_ctrlComboQuestEType.SetWindowText("$QUEST_END_TYPE_SELECT_INFLUENCE");

			m_pQuestEditorDlg->m_valueEditNPCIndex.Format("%d", pQuest->QuestNPCInfo.NPCIndex);
			m_pQuestEditorDlg->m_valueEditPreTalk = pQuest->QuestNPCInfo.PreTalk;
			m_pQuestEditorDlg->m_valueEditMidTalk = pQuest->QuestNPCInfo.MidTalk;
			m_pQuestEditorDlg->m_valueEditSuccessTalk = pQuest->QuestNPCInfo.SuccessTalk;
			m_pQuestEditorDlg->m_valueEditFailTalk = pQuest->QuestNPCInfo.FailTalk;
			m_pQuestEditorDlg->m_valueEditDescription = pQuest->QuestDescription;

			if(pQuest->EventMapIndex != 0)
			m_pQuestEditorDlg->m_valueEditEventMap.Format("%d", pQuest->EventMapIndex);
			if(pQuest->NextQuestIndex != 0)
			m_pQuestEditorDlg->m_valueEditNextQuestIndex.Format("%d", pQuest->NextQuestIndex);

			if(pQuest->PartyQuestMemberCount != 0)
			m_pQuestEditorDlg->m_valueEditPartyQuestMemberCount.Format("%d", pQuest->PartyQuestMemberCount);
			if(pQuest->DropItemToAllAttackCharacter != 0)
			m_pQuestEditorDlg->m_valueEditDropItemToAllCharacter.Format("%d", pQuest->DropItemToAllAttackCharacter);
			if(pQuest->TimeLimitInMinutes != 0)
			m_pQuestEditorDlg->m_valueEditTimeLimitInMinites.Format("%d", pQuest->TimeLimitInMinutes);
			if(pQuest->IsDiscardable != 0)
			m_pQuestEditorDlg->m_valueEditDiscardableQuest.Format("%d", pQuest->IsDiscardable);
			if(pQuest->IsCityWar == TRUE)
				m_pQuestEditorDlg->m_valueCheckBIQuestAttrisCitiwar = TRUE;
			if(pQuest->IsNeverCompletionByUser == TRUE)
			m_pQuestEditorDlg->m_valueCheckBIQuestAttriNeverCompletionByUser = TRUE;

			// Start Requirements
			int tempBit = pQuest->ReqUnitKind; //unitkind
			if(IS_BGEAR((USHORT)tempBit) == TRUE)
				m_pQuestEditorDlg->m_valueCheckBGEAR = TRUE;
			if(IS_MGEAR((USHORT)tempBit) == TRUE)
				m_pQuestEditorDlg->m_valueCheckMGEAR = TRUE;
			if(IS_AGEAR((USHORT)tempBit) == TRUE)
				m_pQuestEditorDlg->m_valueCheckAGEAR = TRUE;
			if(IS_IGEAR((USHORT)tempBit) == TRUE)
				m_pQuestEditorDlg->m_valueCheckIGEAR = TRUE;

			if(pQuest->ReqLevel.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqLevelMin.Format("%d", pQuest->ReqLevel.Min);
			if(pQuest->ReqLevel.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqLevelMax.Format("%d", pQuest->ReqLevel.Max);

			if(pQuest->ReqAttackPart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqAttackMin.Format("%d", pQuest->ReqAttackPart.Min);
			if(pQuest->ReqAttackPart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqAttackMax.Format("%d", pQuest->ReqAttackPart.Max);

			if(pQuest->ReqDefensePart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqDefenseMin.Format("%d", pQuest->ReqDefensePart.Min);
			if(pQuest->ReqDefensePart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqDefenseMax.Format("%d", pQuest->ReqDefensePart.Max);

			if(pQuest->ReqFuelPart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqFuelMin.Format("%d", pQuest->ReqFuelPart.Min);
			if(pQuest->ReqFuelPart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqFuelMax.Format("%d", pQuest->ReqFuelPart.Max);

			if(pQuest->ReqSoulPart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqSoulMin.Format("%d", pQuest->ReqSoulPart.Min);
			if(pQuest->ReqSoulPart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqSoulMax.Format("%d", pQuest->ReqSoulPart.Max);

			if(pQuest->ReqShieldPart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqShieldMin.Format("%d", pQuest->ReqShieldPart.Min);
			if(pQuest->ReqShieldPart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqShieldMax.Format("%d", pQuest->ReqShieldPart.Max);

			if(pQuest->ReqDodgePart.Min != 0)
				m_pQuestEditorDlg->m_valueEditReqDodgeMin.Format("%d", pQuest->ReqDodgePart.Min);
			if(pQuest->ReqDodgePart.Max != 0x7fffffff)
				m_pQuestEditorDlg->m_valueEditReqDodgeMax.Format("%d", pQuest->ReqDodgePart.Max);

			m_pQuestEditorDlg->m_valueEditReqGuild.Format("%d",pQuest->ReqGuild);

			int i;
			for (i = 0; i < pQuest->ReqItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->ReqItemVector[i].ItemNum, pQuest->ReqItemVector[i].Count);
				m_pQuestEditorDlg->m_ctrlListCtrlReqItem.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->ReqQuestIndexVector.size(); i++)
			{
				szTemp.Format("%d",pQuest->ReqQuestIndexVector[i]);
				m_pQuestEditorDlg->m_ctrlListCtrlReqIndex.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->ReqRecvQuestIndexVector.size(); i++)
			{
				szTemp.Format("%d",pQuest->ReqRecvQuestIndexVector[i]);
				m_pQuestEditorDlg->m_ctrlListCtrlRecvReqIndex.InsertItem(i, szTemp);
			}

			tempBit = pQuest->ReqInfluenceType; 
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_NORMAL) == TRUE)
				m_pQuestEditorDlg->m_valueCheckNormal = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_VCN) == TRUE)
				m_pQuestEditorDlg->m_valueCheckVCN = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_ANI) == TRUE)
				m_pQuestEditorDlg->m_valueCheckANI = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_RRP) == TRUE)
				m_pQuestEditorDlg->m_valueCheckRRP = TRUE;
			if((USHORT)tempBit == (BYTE)0x00FF)
				m_pQuestEditorDlg->m_valueCheckALL = TRUE;

			// Start Action
			for (i = 0; i < pQuest->StartPayItemVector.size(); i++)
			{
				szTemp.Format("%d(%d) PER %d TO %d"
					,pQuest->StartPayItemVector[i].ItemNum, pQuest->StartPayItemVector[i].Count
					,pQuest->StartPayItemVector[i].PerItem, pQuest->StartPayItemVector[i].ToUnitKind);
				m_pQuestEditorDlg->m_ctrlListCtrlPayItem.InsertItem(i, szTemp);
			}

			for (i = 0; i < pQuest->StartDelItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->StartDelItemVector[i].ItemNum, pQuest->StartDelItemVector[i].Count);
				m_pQuestEditorDlg->m_ctrlListCtrlSDelItem.InsertItem(i, szTemp);
			}
			m_pQuestEditorDlg->m_valueEditQuestIndex.Format("%d", pQuest->QuestIndex);

			// Completion Requirements
			m_pQuestEditorDlg->m_valueEditCReqNPCIndex.Format("%d", pQuest->TargetMeetNPCInfo.NPCIndex);

			for (i = 0; i < pQuest->TargetMonsterItemVector.size(); i++)
			{
				szTemp.Format("%d(%d) FROM MONSTER(%d,%d~%d,%d)"
					,pQuest->TargetMonsterItemVector[i].ItemWithCount.ItemNum, pQuest->TargetMonsterItemVector[i].ItemWithCount.Count
					,pQuest->TargetMonsterItemVector[i].MonsterUniqueNumber, pQuest->vecQuestDropItem[i].MinCount, pQuest->vecQuestDropItem[i].MaxCount
					,pQuest->vecQuestDropItem[i].Probability);
				m_pQuestEditorDlg->m_ctrlListCtrlCReqItem.InsertItem(i, szTemp);
			}

			if(pQuest->TargetMeetObjectIndex != 0 && pQuest->TargetMeetObjectMapArea.MapIndex)
			{
			m_pQuestEditorDlg->m_valueEditMeetObjectIndex.Format("%d", pQuest->TargetMeetObjectIndex);
			m_pQuestEditorDlg->m_valueEditMeetObjectMapIndex.Format("%d", pQuest->TargetMeetObjectMapArea.MapIndex);
			}
			// Completion Action
			if(pQuest->EndPayBonusStat != 0)
			m_pQuestEditorDlg->m_valueEditPayBonusStat.Format("%d", pQuest->EndPayBonusStat);
// 2005-11-15 by cmkwon, 삭제함
//			if(pQuest->EndPayBonusSkillPoint != 0)
//			m_pQuestEditorDlg->m_valueEditPayBonusSkillPoint.Format("%d", pQuest->EndPayBonusSkillPoint);
			if(pQuest->EndPayFame != 0)
			m_pQuestEditorDlg->m_valueEditPayFame.Format("%d", pQuest->EndPayFame);
			if(pQuest->EndPayStatus != 0)
			m_pQuestEditorDlg->m_valueEditPayStatus.Format("%d", pQuest->EndPayStatus);
			if(pQuest->EndPayLevel != 0)
			m_pQuestEditorDlg->m_valueEditPayLevel.Format("%d", pQuest->EndPayLevel);
			if(pQuest->EndPayExperience != 0)
			m_pQuestEditorDlg->m_valueEditPayExperience.Format("%d", pQuest->EndPayExperience);
			if(pQuest->EndPayWarpMapIndex != 0)
			m_pQuestEditorDlg->m_valueEditPayWarpMapIndex.Format("%d", pQuest->EndPayWarpMapIndex);

			for (i = 0; i < pQuest->EndDeleteItemVector.size(); i++)
			{
				if(pQuest->EndDeleteItemVector[i].Count == 0x7fffffff)
					szTemp.Format("%d(ALL)",pQuest->EndDeleteItemVector[i].ItemNum);
				else
					szTemp.Format("%d(%d)",pQuest->EndDeleteItemVector[i].ItemNum, pQuest->EndDeleteItemVector[i].Count);
				m_pQuestEditorDlg->m_ctrlListCtrlCADelItem.InsertItem(i, szTemp);
			}
			for (i = 0; i < pQuest->EndPayItemVector.size(); i++)
			{
				szTemp.Format("%d(%d)",pQuest->EndPayItemVector[i].ItemNum, pQuest->EndPayItemVector[i].Count);
				m_pQuestEditorDlg->m_ctrlListCtrlCAPayItem.InsertItem(i, szTemp);
			}
			for (i = 0; i < pQuest->EndMissionMasterPayItemVector.size(); i++)
			{// 2008-12-02 by dhjin, 미션마스터
				szTemp.Format("%d(%d)",pQuest->EndMissionMasterPayItemVector[i].ItemNum, pQuest->EndMissionMasterPayItemVector[i].Count);
				m_pQuestEditorDlg->m_ctrlListCtrlCAPayItem.InsertItem(i, szTemp);
			}
			if(&pQuest->EndRandomPayItemVector[0] != 0)
			{
				for (i = 0; i < pQuest->EndRandomPayItemVector[0].NumOfItems; i++)
				{
					szTemp.Format("%d(%d,%d)",pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].ItemNum, pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].Count,pQuest->EndRandomPayItemVector[0].ItemWithCountProb[i].Prob100);
					m_pQuestEditorDlg->m_ctrlListCtrlCARandomPayItem.InsertItem(i, szTemp);
				}
			}
			for (i = 0; i < pQuest->EndPayMixItemVector.size(); i++)
			{
				for(int j= 0 ; j<pQuest->EndPayMixItemVector[i].NumOfSourceItems;j++)
				{
					szTemp.Format("%d(%d)",pQuest->EndPayMixItemVector[i].SourceItem[j].ItemNum, pQuest->EndPayMixItemVector[i].SourceItem[j].Count);
					m_pQuestEditorDlg->m_ctrlListCtrlCAMixItem.InsertItem(i, szTemp);
				}
				m_pQuestEditorDlg->m_valueEditCAMixItemTargetItem.Format("%d(%d)", pQuest->EndPayMixItemVector[i].TargetItemNum, pQuest->EndPayMixItemVector[i].MixingProbability);
			}

			if(pQuest->EndPayPropensity != 0)
			m_pQuestEditorDlg->m_valueEditCAPayPropensity.Format("%d", pQuest->EndPayPropensity);
			if(pQuest->EndCinemaNum != 0)
			m_pQuestEditorDlg->m_valueEditCACinemaNum.Format("%d", pQuest->EndCinemaNum);

			tempBit = pQuest->EndInfluenceType; 
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_NORMAL) == TRUE)
				m_pQuestEditorDlg->m_valueCheckCANormal = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_VCN) == TRUE)
				m_pQuestEditorDlg->m_valueCheckCAVCN = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_ANI) == TRUE)
				m_pQuestEditorDlg->m_valueCheckCAANI = TRUE;
			if(COMPARE_INFLUENCE((USHORT)tempBit, INFLUENCE_TYPE_RRP) == TRUE)
				m_pQuestEditorDlg->m_valueCheckCARRP = TRUE;
			if((USHORT)tempBit == (BYTE)0x00FF)
				m_pQuestEditorDlg->m_valueCheckCAALL = TRUE;
			break;
		}
		itr++;
	}
	m_pQuestEditorDlg->UpdateData(FALSE);
	EndDialog(0);
}

void CQuestLoadDlg::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here
	ReflectQuestEditor();
}

void CQuestLoadDlg::InitQuestEditor()
{
	m_pQuestEditorDlg->InitQuestEditor();
	m_pQuestEditorDlg->UpdateData(FALSE);
}

BOOL CQuestLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreTranslateMessage(pMsg);
}
