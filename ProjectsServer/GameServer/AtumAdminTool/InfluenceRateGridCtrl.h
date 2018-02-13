// InfluenceRateGridCtrl.h: interface for the CInfluenceRateGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLUENCERATEGRIDCTRL_H__4499D11F_B16A_402B_AD80_ABAC05C89871__INCLUDED_)
#define AFX_INFLUENCERATEGRIDCTRL_H__4499D11F_B16A_402B_AD80_ABAC05C89871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCGridHelper.h"

class CInfluenceRateGridCtrl : public CSCGridCtrl  
{
public:
	CInfluenceRateGridCtrl();
	virtual ~CInfluenceRateGridCtrl();

	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 비율 숫자만 입력가능하게 처리 하기 위해
    virtual void  OnEndEditCell(int nRow, int nCol, CString str);
};

#endif // !defined(AFX_INFLUENCERATEGRIDCTRL_H__4499D11F_B16A_402B_AD80_ABAC05C89871__INCLUDED_)
