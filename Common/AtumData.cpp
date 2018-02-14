//Copyright[2002] MasangSoft
// AtumData.cpp: implementation of the CAtumData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumData.h"
#include "dxutil.h"
// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
#include "ShuttleChild.h"
#include "PetManager.h"

extern CShuttleChild* g_pShuttleChild;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtumData::CAtumData()
{
    m_bAniColl = FALSE;
    m_bIsUnderWater = FALSE;
    m_bCheckBlend = FALSE;
    m_pCharacterInfo = NULL;
    m_fAniCollTime = 0.0f;
    m_bShadowIsRender = FALSE;
    m_bShielding = FALSE;
//    m_pWeapon1_1 = NULL;
    m_pWeapon1_2 = NULL;
//    m_pWeapon2_1 = NULL;
    m_pWeapon2_2 = NULL;

    m_pWeapon1_1_1 = NULL;
    m_pWeapon1_1_2 = NULL;
    m_pWeapon2_1_1 = NULL;
    m_pWeapon2_1_2 = NULL;

    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    m_pRobotAGearWeapon_1    = NULL;
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

    m_pEngine = NULL;
    m_pRadar = NULL;
//    m_pArmor = NULL;
    m_pContainer = NULL;
    m_pAccessories = NULL;
    m_pPartner    = NULL;                                    // 2010-06-08 by dgwoo, 펫시스템 추가.  
    m_pPartner1    = NULL;                                    // 2010-06-08 by dgwoo, 펫시스템 추가.  
    m_pDummyPartner    = NULL;                                // 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
    m_pDummyPartner1 = NULL;                            // 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
    m_pWingIn = NULL;
    m_bDegree = 0;
    m_bItemDegree = 0;
    m_vLWBackPos = D3DXVECTOR3(0,0,0);
    m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);                // Up Vector
    m_vVel = D3DXVECTOR3(1.0f,0.0f,1.0f);                // Velocity & Direct

    // 1형 무기의 연사력을 높여주기 위한 다음 데이타
//    m_bPrimaryAttackCount = 0;
//    memset(&m_PrimaryAttackData1,0x00,sizeof(MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY));
//    memset(&m_PrimaryAttackData2,0x00,sizeof(MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY));
//    m_bIsItemAttack = FALSE;
//    m_pPrimaryAttackTarget = NULL;
//    m_fCheckPrimaryAttackTime = 0.0f;
    memset(&m_PrimaryAttack,0x00,sizeof(ATTACK_DATA));
    memset(&m_SecondaryAttack,0x00,sizeof(ATTACK_DATA));

    m_vWeaponPos = D3DXVECTOR3(0,0,0);        // A-Gear 용 무기 위치(x->m_vSideVel,y->m_vUp,z->m_vVel)D3DXVECTOR3(0,10,0);    
    m_vWeaponVel = m_vVel;                    // A-Gear 용 무기 앞방향
    m_vWeaponUp = m_vUp;                    // A-Gear 용 무기 위방향
    D3DXMatrixLookAtRH(&m_mWeaponMatrix,&m_vWeaponPos, &(m_vWeaponPos+m_vWeaponVel),&m_vWeaponUp);
    D3DXMatrixInverse( &m_mWeaponMatrix, NULL, &m_mWeaponMatrix );

    // 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태
    m_vPetLeftPos    = D3DXVECTOR3(0,0,0);
    m_vPetLeftVel    = m_vVel;
    m_vPetLeftUp    = m_vUp;

    D3DXMatrixLookAtRH(&m_vPetLeftMatrix,&m_vPetLeftPos, &(m_vPetLeftPos + m_vPetLeftVel),&m_vPetLeftUp);
    D3DXMatrixInverse( &m_vPetLeftMatrix, NULL, &m_vPetLeftMatrix );

    m_vPetRightPos    = D3DXVECTOR3(0,0,0);
    m_vPetRightVel    = m_vVel;
    m_vPetRightUp    = m_vUp;

    D3DXMatrixLookAtRH(&m_vPetRightMatrix,&m_vPetRightPos, &(m_vPetRightPos + m_vPetRightVel),&m_vPetRightUp);
    D3DXMatrixInverse( &m_vPetRightMatrix, NULL, &m_vPetRightMatrix );
    // end 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태
    
    for(int i = 0; i < 4; i++)
        D3DXMatrixIdentity(&m_pMatWeaponPosition[i]);
}

CAtumData::~CAtumData()
{
    util::del(m_pCharacterInfo);
//    if(m_pWeapon1_1)
//    {
//        m_pWeapon1_1->m_pParent = NULL;
//        m_pWeapon1_1->m_bUsing = FALSE;
//        m_pWeapon1_1 = NULL;
//    }
    if(m_pWeapon1_1_1)
    {
        m_pWeapon1_1_1->m_pParent = NULL;
        m_pWeapon1_1_1->m_bUsing = FALSE;
        m_pWeapon1_1_1 = NULL;
    }
    if(m_pWeapon1_1_2)
    {
        m_pWeapon1_1_2->m_pParent = NULL;
        m_pWeapon1_1_2->m_bUsing = FALSE;
        m_pWeapon1_1_2 = NULL;
    }
    if(m_pWeapon1_2)
    {
        m_pWeapon1_2->m_pParent = NULL;
        m_pWeapon1_2->m_bUsing = FALSE;
        m_pWeapon1_2 = NULL;
    }
//    if(m_pWeapon2_1)
//    {
//        m_pWeapon2_1->m_pParent = NULL;
//        m_pWeapon2_1->m_bUsing = FALSE;
//        m_pWeapon2_1 = NULL;
//    }
    if(m_pWeapon2_1_1)
    {
        m_pWeapon2_1_1->m_pParent = NULL;
        m_pWeapon2_1_1->m_bUsing = FALSE;
        m_pWeapon2_1_1 = NULL;
    }
    if(m_pWeapon2_1_2)
    {
        m_pWeapon2_1_2->m_pParent = NULL;
        m_pWeapon2_1_2->m_bUsing = FALSE;
        m_pWeapon2_1_2 = NULL;
    }
    if(m_pWeapon2_2)
    {
        m_pWeapon2_2->m_pParent = NULL;
        m_pWeapon2_2->m_bUsing = FALSE;
        m_pWeapon2_2 = NULL;
    }


    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    if( m_pRobotAGearWeapon_1 )
    {
        m_pRobotAGearWeapon_1->m_pParent    = NULL;
        m_pRobotAGearWeapon_1->m_bUsing        = FALSE;
        m_pRobotAGearWeapon_1                = NULL;
    }
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


    if(m_pEngine)
    {
        m_pEngine->m_pParent = NULL;
        m_pEngine->m_bUsing = FALSE;
        m_pEngine = NULL;
    }
    if(m_pRadar)
    {
        m_pRadar->m_pParent = NULL;
        m_pRadar->m_bUsing = FALSE;
        m_pRadar = NULL;
    }
//    if(m_pArmor)
//    {
//        m_pArmor->m_pParent = NULL;
//        m_pArmor->m_bUsing = FALSE;
//        m_pArmor = NULL;
//    }
    if(m_pContainer)
    {
        m_pContainer->m_pParent = NULL;
        m_pContainer->m_bUsing = FALSE;
        m_pContainer = NULL;
    }
    if(m_pAccessories)
    {
        m_pAccessories->m_pParent = NULL;
        m_pAccessories->m_bUsing = FALSE;
        m_pAccessories = NULL;
    }
    // 2010-06-08 by dgwoo, 펫시스템 추가. 
    if(m_pPartner)
    {
        m_pPartner->m_pParent = NULL;
        m_pPartner->m_bUsing = FALSE;
        m_pPartner = NULL;
    }
    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
    if(m_pPartner1)
    {
        m_pPartner1->m_pParent = NULL;
        m_pPartner1->m_bUsing = FALSE;
        m_pPartner1 = NULL;
    }

    if(m_pDummyPartner)
    {
        m_pDummyPartner->m_pParent = NULL;
        m_pDummyPartner->m_bUsing = FALSE;
        m_pDummyPartner = NULL;
    }    
    if(m_pDummyPartner1)
    {
        m_pDummyPartner1->m_pParent = NULL;
        m_pDummyPartner1->m_bUsing = FALSE;
        m_pDummyPartner1 = NULL;
    }
    // end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
    // 2010-06-08 by dgwoo, 펫시스템 추가. 
    if(m_pWingIn)
    {
        m_pWingIn->m_pParent = NULL;
        m_pWingIn->m_bUsing = FALSE;
        m_pWingIn = NULL;
    }
}
// 2010. 03. 18 by jskim 몬스터변신 카드
//void CAtumData::LoadCharacterEffect(char* strFileName)
void CAtumData::LoadCharacterEffect(char* strFileName, int nMonsterTransformer /* = 0 */, float nMonsterTransScale /* = 0.0f */)
//end 2010. 03. 18 by jskim 몬스터변신 카드
{
    if(m_pCharacterInfo)
    {
        m_pCharacterInfo->InvalidateDeviceObjects();
        m_pCharacterInfo->DeleteDeviceObjects();
        util::del(m_pCharacterInfo);
    }
    m_pCharacterInfo = new CCharacterInfo();
    // 2010. 03. 18 by jskim 몬스터변신 카드
    //if(m_pCharacterInfo->Load(strFileName))
    if(m_pCharacterInfo->Load(strFileName, nMonsterTransformer, nMonsterTransScale))
    //end 2010. 03. 18 by jskim 몬스터변신 카드
    {
        m_pCharacterInfo->InitDeviceObjects();
        m_pCharacterInfo->RestoreDeviceObjects();
    }
    else
    {
        util::del(m_pCharacterInfo);
        DBGOUT("WARNING!! [EFFECT ERROR] Can't Find Effect File : %s \n",strFileName);
    }

}
// 2010-06-15 by shcho&hslee 펫시스템 - 1형, 2형 포지션 매트릭스 함수 분리
///////////////////////////////////////////////////////////////////////////////
/// \fn            CAtumData::SetWeaponPosition()
/// \brief        
/// \author        ispark
/// \date        2006-01-03 ~ 2006-01-03
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
// void CAtumData::SetWeaponPosition()
// {
//     if(m_pCharacterInfo != NULL)
//     {
//         m_pMatWeaponPosition[0] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
//         m_pMatWeaponPosition[1] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
//         m_pMatWeaponPosition[2] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_RIGHT_POSITION);
//         m_pMatWeaponPosition[3] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_LEFT_POSITION);
// 
//         // 기존에는 무기에서 셋팅을 했지만 모듈화 시키면서 좌표를 아머에서 가져와야 한다. 하나만 셋팅하면 된다.
//         m_PrimaryAttack.vSidePos.x = m_pMatWeaponPosition[0]._41;
//         m_PrimaryAttack.vSidePos.y = m_pMatWeaponPosition[0]._42;
//         m_PrimaryAttack.vSidePos.z = m_pMatWeaponPosition[0]._43;
// 
//         m_SecondaryAttack.vSidePos.x = m_pMatWeaponPosition[2]._41;
//         m_SecondaryAttack.vSidePos.y = m_pMatWeaponPosition[2]._42;
//         m_SecondaryAttack.vSidePos.z = m_pMatWeaponPosition[2]._43;
//     }
// }
void CAtumData::SetPrimaryWeaponPosition()
{
    if(m_pCharacterInfo != NULL)
    {
        m_pMatWeaponPosition[0] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
        m_pMatWeaponPosition[1] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
        
        // 기존에는 무기에서 셋팅을 했지만 모듈화 시키면서 좌표를 아머에서 가져와야 한다. 하나만 셋팅하면 된다.
        m_PrimaryAttack.vSidePos.x = m_pMatWeaponPosition[0]._41;
        m_PrimaryAttack.vSidePos.y = m_pMatWeaponPosition[0]._42;
        m_PrimaryAttack.vSidePos.z = m_pMatWeaponPosition[0]._43;        
    }
}

void CAtumData::SetSecondaryWeaponPosition()
{
    if(m_pCharacterInfo != NULL)
    {
        m_pMatWeaponPosition[2] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_RIGHT_POSITION);
        m_pMatWeaponPosition[3] = m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_LEFT_POSITION);
        
        // 기존에는 무기에서 셋팅을 했지만 모듈화 시키면서 좌표를 아머에서 가져와야 한다. 하나만 셋팅하면 된다.
        m_SecondaryAttack.vSidePos.x = m_pMatWeaponPosition[2]._41;
        m_SecondaryAttack.vSidePos.y = m_pMatWeaponPosition[2]._42;
        m_SecondaryAttack.vSidePos.z = m_pMatWeaponPosition[2]._43;        
    }
}
// 2010-06-15 by shcho&hslee 펫시스템 - 1형, 2형 포지션 매트릭스 함수 분리

// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
void CAtumData::SetPetPosition()
{
    if(m_pCharacterInfo != NULL)
    {
        m_pMatPetPosition[0] = m_pCharacterInfo->GetEffectMatrix(BODYCON_PET_POSITION_MASK, RC_EFF_PET_RIGHT_POSITION);
        m_pMatPetPosition[1] = m_pCharacterInfo->GetEffectMatrix(BODYCON_PET_POSITION_MASK, RC_EFF_PET_LEFT_POSITION);

        D3DXMATRIX temp;
        D3DXMatrixIdentity( &temp );

        if(m_pMatPetPosition[0] == temp)
        {
            m_PetAttack.vSidePos.x = m_PetAttack.vSideBackPos.x = m_pMatPetPosition[0]._41 = PET_DEFULT_POS_X;
            m_PetAttack.vSidePos.y = m_PetAttack.vSideBackPos.y = m_pMatPetPosition[0]._42 = PET_DEFULT_POS_Y;
            m_PetAttack.vSidePos.z = m_PetAttack.vSideBackPos.z = m_pMatPetPosition[0]._43 = PET_DEFULT_POS_Z;
        }
        else
        {    
            m_PetAttack.vSidePos.x = m_PetAttack.vSideBackPos.x = m_pMatPetPosition[0]._41;
            m_PetAttack.vSidePos.y = m_PetAttack.vSideBackPos.y = m_pMatPetPosition[0]._42;
            m_PetAttack.vSidePos.z = m_PetAttack.vSideBackPos.z = m_pMatPetPosition[0]._43;

            m_pMatPetPosition[0]._11 = 1;
            m_pMatPetPosition[0]._33 = 1;

            m_pMatPetPosition[1]._11 = 1;
            m_pMatPetPosition[1]._33 = 1;            
        }
        
        if(m_pMatPetPosition[1] == temp)
        {
            m_pMatPetPosition[1]._41 = - PET_DEFULT_POS_X;
            m_pMatPetPosition[1]._42 =     PET_DEFULT_POS_Y;
            m_pMatPetPosition[1]._43 =   PET_DEFULT_POS_Z;
        }    
    }
}
//2011-07-20 by jhahn    인피3차 시네마플레이중 타캐릭터 안보이기
void  CAtumData::SetHideData()
{
    if (m_pWeapon1_2)
        m_pWeapon1_2->m_bRender = FALSE;
    if (m_pWeapon2_2)
        m_pWeapon2_2->m_bRender = FALSE;
    if (m_pEngine)
        m_pEngine->m_bRender = FALSE;    
    if (m_pRadar)
        m_pRadar->m_bRender = FALSE;    
    if (m_pContainer)
        m_pContainer->m_bRender = FALSE;
    if (m_pWingIn)
        m_pWingIn->m_bRender = FALSE;    

    if (m_pWeapon1_1_1)
        m_pWeapon1_1_1->m_bRender = FALSE;        
    if (m_pWeapon1_1_2)
        m_pWeapon1_1_2->m_bRender = FALSE;        
    if (m_pWeapon2_1_1)
        m_pWeapon2_1_1->m_bRender = FALSE;        
    if (m_pWeapon2_1_2)
        m_pWeapon2_1_2->m_bRender = FALSE;            
    if (m_pRobotAGearWeapon_1)
        m_pRobotAGearWeapon_1->m_bRender = FALSE;

    if (m_pPartner)
        m_pPartner->m_bRender = FALSE;            
    if (m_pPartner1)
        m_pPartner1->m_bRender = FALSE;        
    if (m_pDummyPartner)
        m_pDummyPartner->m_bRender = FALSE;    
    if (m_pDummyPartner1)
        m_pDummyPartner1->m_bRender = FALSE;    
}
void CAtumData::SetShowData()
{

    if (m_pWeapon1_2)
        m_pWeapon1_2->m_bRender = TRUE;
    if (m_pWeapon2_2)
        m_pWeapon2_2->m_bRender = TRUE;
    if (m_pEngine)
        m_pEngine->m_bRender = TRUE;    
    if (m_pRadar)
        m_pRadar->m_bRender = TRUE;    
    if (m_pContainer)
        m_pContainer->m_bRender = TRUE;
    if (m_pWingIn)
        m_pWingIn->m_bRender = TRUE;    
    
    if (m_pWeapon1_1_1)
        m_pWeapon1_1_1->m_bRender = TRUE;        
    if (m_pWeapon1_1_2)
        m_pWeapon1_1_2->m_bRender = TRUE;        
    if (m_pWeapon2_1_1)
        m_pWeapon2_1_1->m_bRender = TRUE;        
    if (m_pWeapon2_1_2)
        m_pWeapon2_1_2->m_bRender = TRUE;            
    if (m_pRobotAGearWeapon_1)
        m_pRobotAGearWeapon_1->m_bRender = TRUE;
    
    if (m_pPartner)
        m_pPartner->m_bRender = TRUE;            
    if (m_pPartner1)
        m_pPartner1->m_bRender = TRUE;        
    if (m_pDummyPartner)
        m_pDummyPartner->m_bRender = TRUE;    
    if (m_pDummyPartner1)
        m_pDummyPartner1->m_bRender = TRUE;    
}
//end 2011-07-20 by jhahn    인피3차 시네마플레이중 타캐릭터 안보이기

// 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
void CAtumData::SetMarkPosition()
{
    if(m_pCharacterInfo != NULL)
    {
        m_pMatMarkPosition = m_pCharacterInfo->GetEffectMatrix(BODYCON_MARK_POSITION_MASK, RC_EFF_MARK_POSITION);    
    }
    D3DXMATRIX temp;
    D3DXMatrixIdentity( &temp );
    
    if(m_pMatMarkPosition == temp)
    {
        m_pMatMarkPosition._41 = MARK_DEFULT_POS_X;
        m_pMatMarkPosition._42 = MARK_DEFULT_POS_Y;
        m_pMatMarkPosition._43 = MARK_DEFULT_POS_Z;
    }

}
//end 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
