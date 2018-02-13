// SelectItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelectItemDlg.h"
#include "AtumSJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectItemDlg dialog


CSelectItemDlg::CSelectItemDlg(ez_map<INT, ITEM> *i_pMapItemInfo, ez_map<string, ITEM> *i_pMapItemInfoName, BOOL i_nEnableCount, CWnd* pParent /* = NULL */)
	: CDialog(CSelectItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectItemDlg)
	m_nItemCount = 0;
	//}}AFX_DATA_INIT
	m_pMapItemInfo = i_pMapItemInfo;
	m_pMapItemInfoName = i_pMapItemInfoName;	
	m_nSelectedItemNum = 0;
	m_bEnableCount = i_nEnableCount;
}


void CSelectItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectItemDlg)
	DDX_Text(pDX, IDC_EDIT_ITEM_COUNT, m_nItemCount);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_ctl_comboItemNameList);
	DDX_Control(pDX, IDC_COMBO_ITEM_KIND, m_ctl_comboItemKindList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectItemDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectItemDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_KIND, OnSelchangeComboItemKind)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, OnSelchangeComboItem)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CBN_DROPDOWN(IDC_COMBO_ITEM, OnDropdownComboItem)
	ON_CBN_EDITCHANGE(IDC_COMBO_ITEM, OnEditchangeComboItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectItemDlg message handlers


#define COMBO_ITEMKIND_OFFSET			3
#define COMBO_ITEMKIND_ALL				2

BOOL CSelectItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 종류 리스트 생성
	CComboBox *pItemKindCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM_KIND);
	pItemKindCombo->AddString("Item Kind");
	pItemKindCombo->AddString("-----------------");
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) -  
	//pItemKindCombo->AddString("ALL");
	pItemKindCombo->AddString(CAtumSJ::GetItemKindName(ITEMKIND_ALL_ITEM));		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	for (int i = 0; g_arrItemKindString[i].byItemKind0 != ITEMKIND_UNKNOWN; i++)
	{
		if(IS_GENERAL_ITEM(g_arrItemKindString[i].byItemKind0))
		{
			pItemKindCombo->AddString(g_arrItemKindString[i].szItemKindName);
		}
	}
	pItemKindCombo->SetCurSel(0);

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 아래와 같이 수정
// 	// 아이템 리스트 생성
// 	CComboBox *pItemCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM);
// 	pItemCombo->AddString("All Item");
// 	pItemCombo->AddString("-----------------");
// 	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
// 	while (m_pMapItemInfoName->end() != itr)
// 	{
// 		ITEM *pItem = &itr->second;
// 
// 		if (IS_GENERAL_ITEM(pItem->Kind))
// 		{
// 			pItemCombo->AddString(pItem->ItemName);
// 		}
// 
// 		itr++;
// 	}
// 	pItemCombo->SetCurSel(0);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	this->OnSelchangeComboItemKind();

	// 개수 (비)활성화
	GetDlgItem(IDC_EDIT_ITEM_COUNT)->EnableWindow(m_bEnableCount);
	GetDlgItem(IDC_STATIC_ITEM_COUNT)->EnableWindow(m_bEnableCount);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectItemDlg::OnSelchangeComboItemKind() 
{
	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
	BYTE byItemKind = this->GetSelectedItemKindFromItemKindComboBox();
	this->ResetItemInfoList(byItemKind);

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 위와 같이 수정 함
// 	CComboBox *pItemKindCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM_KIND);
// 
// 	int nComboIdx = pItemKindCombo->GetCurSel();
// 	if (nComboIdx < COMBO_ITEMKIND_OFFSET && nComboIdx != COMBO_ITEMKIND_ALL)
// 	{
// 		return;
// 	}
// 	
// 	CString csItemKindName;	
// 	pItemKindCombo->GetWindowText(csItemKindName);
// 	int nItemKind = CAtumSJ::GetItemKindByItemKindName((LPSTR)(LPCSTR)csItemKindName);
// 
// 	// 아이템 리스트 생성
// 	CComboBox *pItemCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM);
// 	pItemCombo->ResetContent();
// 	CString tmpStr;
// 	tmpStr.Format(STRMSG_S_SCADMINTOOL_0043, csItemKindName);
// 	pItemCombo->AddString((LPCSTR)tmpStr);
// 	pItemCombo->AddString("-----------------");
// 	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
// 	while (m_pMapItemInfoName->end() != itr)
// 	{
// 		ITEM *pItem = &itr->second;
// 
// 		if (IS_GENERAL_ITEM(pItem->Kind)
// 			&& (nComboIdx == COMBO_ITEMKIND_ALL || pItem->Kind == nItemKind)
// 			)
// 		{
// 			pItemCombo->AddString(pItem->ItemName);
// 		}
// 		itr++;
// 	}
// 	pItemCombo->SetCurSel(0);	
}

void CSelectItemDlg::OnSelchangeComboItem() 
{
	CComboBox *pItemCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM);
	int nItem = pItemCombo->GetCurSel();

	if (nItem < 2)
	{
		return;
	}

// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - ItemKind 를 설정하지 안는다.
// 	CString itemName;
// 	pItemCombo->GetLBText(nItem, itemName);
// 
// 	CComboBox *pItemKindCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM_KIND);
// 	pItemKindCombo->SetCurSel(m_pMapItemInfoName->findEZ_ptr((LPCSTR)itemName)->Kind+COMBO_ITEMKIND_OFFSET);
}

void CSelectItemDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CComboBox *pItemCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM);
	int nItem = pItemCombo->GetCurSel();

	if (nItem < 2)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0039);
		return;
	}

	CString itemName;
	pItemCombo->GetLBText(nItem, itemName);

	CComboBox *pItemKindCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ITEM_KIND);
	ITEM *pItem = m_pMapItemInfoName->findEZ_ptr((LPCSTR)itemName);

	if (pItem == NULL)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0039);
		return;
	}

	// ItemNum 할당
	m_nSelectedItemNum = pItem->ItemNum;
	m_szSelectedItemName = pItem->ItemName;

	if (m_bEnableCount && m_nItemCount <= 0)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0040);
		return;
	}

	if (!IS_COUNTABLE_ITEM(pItem->Kind) && m_nItemCount > 5)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0041);
		return;
	}

	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CSelectItemDlg::GetSelectedItemKindFromItemKindComboBox(void)
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - CSelectItemDlg::GetSelectedItemKindFromItemKindComboBox() 추가
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CSelectItemDlg::GetSelectedItemKindFromItemKindComboBox(void)
{
	BYTE byItemKind		= ITEMKIND_ALL_ITEM;
	int nComboIdx		= m_ctl_comboItemKindList.GetCurSel();
	if (nComboIdx <= COMBO_ITEMKIND_OFFSET)
	{
		return byItemKind;
	}
	
	CString csItemKindName;	
	m_ctl_comboItemKindList.GetWindowText(csItemKindName);
	byItemKind = CAtumSJ::GetItemKindByItemKindName((LPSTR)(LPCSTR)csItemKindName);
	if(ITEMKIND_UNKNOWN == byItemKind)
	{
		byItemKind = ITEMKIND_ALL_ITEM;
	}
	
	return byItemKind;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSelectItemDlg::ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName/*=NULL*/)
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-05-30 ~ 2008-05-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSelectItemDlg::ResetItemInfoList(BYTE i_byItemKind, CString *i_pcsPartOfItemName/*=NULL*/)
{
	if(ITEMKIND_UNKNOWN == i_byItemKind)
	{
		return FALSE;
	}
	
	// 아이템 리스트 생성
	m_ctl_comboItemNameList.ResetContent();
	CString tmpStr;
	tmpStr.Format(STRMSG_S_SCADMINTOOL_0043, CAtumSJ::GetItemKindName(i_byItemKind));
	m_ctl_comboItemNameList.AddString((LPCSTR)tmpStr);
	m_ctl_comboItemNameList.AddString("-----------------");
	ez_map<string, ITEM>::iterator itr = m_pMapItemInfoName->begin();
	while (m_pMapItemInfoName->end() != itr)
	{
		ITEM *pItem = &itr->second;
		if (IS_GENERAL_ITEM(pItem->Kind)
			&& (ITEMKIND_ALL_ITEM == i_byItemKind || pItem->Kind == i_byItemKind))
		{
			if(NULL == i_pcsPartOfItemName
				|| strstr(pItem->ItemName, *i_pcsPartOfItemName))
			{
				m_ctl_comboItemNameList.AddString(pItem->ItemName);
			}
		}
		
		itr++;
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSelectItemDlg::OnDropdownComboItem() 
{
	// TODO: Add your control notification handler code here
	BYTE byItemKind = this->GetSelectedItemKindFromItemKindComboBox();
	
	CString csSearchName;
	if(FALSE == util::in_range(0, m_ctl_comboItemNameList.GetCurSel(), 1))
	{
		m_ctl_comboItemNameList.GetWindowText(csSearchName);
		if(csSearchName.IsEmpty())
		{
			this->ResetItemInfoList(byItemKind);
			return;
		}
	}
	
	this->ResetItemInfoList(byItemKind, &csSearchName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-03 by cmkwon, AdminTool, DBTool 사용시 아이템 검색시 콤보박스에서 검색 기능 추가(K0000143) - 
/// \author		cmkwon
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSelectItemDlg::OnEditchangeComboItem() 
{
	// TODO: Add your control notification handler code here
	m_ctl_comboItemNameList.ShowDropDown(FALSE);	// DropDown 리스트를 닫는다. 검색을 위해서
}
