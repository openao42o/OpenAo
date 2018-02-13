#pragma once


enum EnumQueryType
{
	QT_CreateCharacter = 0,
	QT_DeleteCharacter,
	QT_GetCharacterInfoByName,
	QT_GetAccountUniqueNumber,
	QT_ConnectGameStart,
	QT_GetAllCharacterInfoByID,
	QT_SaveCharacterCriticalData,
	QT_ChangeUnitKind,
	QT_ChangeStat,
	QT_ChangeBonusStatPoint,			// 2005-11-15 by cmkwon, 추가함
	QT_ChangeGuild,
	QT_ChangeFriendGroup,
	QT_ChangeExp,
	QT_ChangeLevel,
	QT_ChangeBodyCondition,
	QT_ChangePropensity,

	QT_ChangeInfluenceType,
	QT_ChangeStatus,
	QT_ChangePKPoint,
	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 사용하지 않으므로 제거
	//	QT_ChangeRacingPoint,
	QT_UpdateTotalPlayTime,
	QT_UpdateLastStartedTime,
	QT_ChangeHPDPSPEP,
	QT_ChangeCurrentHPDPSPEP,
	QT_ChangeMapChannel,
	QT_ChangePetInfo,
	QT_ChangeDockingItem,
	QT_ChangePosition,
	QT_EventWarpConnect,
	// Item 관련 query들
	QT_GetStoreItem,
	QT_ReloadAllEnchant,
	QT_DeleteStoreItem,
	QT_InsertStoreItem,
	QT_InsertStoreItemSkill,
	QT_UpdateStoreItemSkillUseTime,		// 2006-11-17 by dhjin
	QT_UpdateItemCount,
	QT_UpdateItemNum,					// 2006-06-14 by cmkwon
	QT_UpdateEndurance,
	QT_StoreUpdateColorCode,			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - EffectItemNum 의 의미로 사용, // 2005-11-21 by cmkwon, 추가함
	QT_UpdateShapeItemNum,				// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 

	QT_UpdateItemUsingTimeStamp,
	QT_UpdateItemRareFix,
	QT_InsertDefaultItems,
	QT_ChangeItemPosition,
	QT_UpdateWindowItemList,
	QT_UpdateItemPossess,
	QT_UpdateItemStorage,		// 2005-12-07 by cmkwon
	QT_LoadOneItem,			// 하나의 아이템 로딩 및 전송, 인챈트 정보 포함
	QT_INSERTLOGINITEMEVENT,	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
	QT_CheckEventItem,		// 2006-08-25 by dhjin, 이벤트 아이템 관련
	QT_InsertEventItem,		// 2006-08-25 by dhjin, 이벤트 아이템 관련
	QT_UpdateEventItemFixedPeriod,					// 2013-02-28 by bckim, 복귀유져 버프추가
	QT_CheckCouponEvent,	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
	QT_GetGuildStoreItem,	// 2006-09-20 by dhjin, 여단 창고 관련
	QT_UpdateGuildStoreItem,	// 2006-09-20 by dhjin, 여단 창고 관련
	QT_InsertGuildStoreItem,	// 2006-09-20 by dhjin, 여단 창고 관련
	QT_DeleteGuildStoreItem,	// 2006-09-20 by dhjin, 여단 창고 관련
	QT_GetLogGuildStoreItem,	// 2006-09-27 by dhjin, 여단 창고 로그 관련
	QT_AllDeleteGuildStoreItem, // 2006-09-29 by dhjin, 여단 창고 관련
	QT_Insert2WarpableUserList,	// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 추가
	QT_DeleteWarpableUser,		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 삭제
	//QT_UPDATE_ConnectingServerGroupID,		// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - 추가 함
	QT_GiveStoreItem,						// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	QT_GetLetter,					// 2008-04-24 by dhjin, EP3 편지 시스템 - DB에서 편지 가져오기
	QT_ReadLetter,					// 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 읽기
	QT_DeleteLetter,				// 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 삭제
	QT_SendLetter,					// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 보내기
	QT_GetAllLetter,				// 2008-05-09 by dhjin, EP3 편지 시스템 - DB에서 전체 편지 가져오기
	QT_SendAllLetter,				// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 보내기
	QT_ReadAllLetter,				// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기
	QT_DeleteAllLetter,				// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 삭제
	QT_DeleteOldAllLetter,			// 2008-05-09 by dhjin, EP3 편지 시스템 - 오래된 전체 편지 삭제

	// IM Server용
	QT_AuthChatLogin,
	QT_SaveLastPartyID,
	// Guild 관련
	QT_GuildCreate,
	QT_GuildAddMember,
	QT_GuildAddOffMember,			// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 오프유저 여단 가입 		
	QT_GuildLeaveMember,
	QT_GuildBanMember,
	QT_GuildUpdateMemberCapacity,
	QT_GuildLoadGuild,
	QT_GuildDismember,			// 길드의 해체 날짜와 상태만 세팅하기
	QT_GuildCancelDismember,
	QT_GuildChangeGuildName,
	QT_GuildSetGuildMark,
	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 사용하지 안는 함수임
	//	QT_GuildGetGuildMark,
	QT_GuildSetRank,
	QT_GuildDeleteGuild,		// 길드를 완전히 제거하기
	QT_GuildSaveGuildWarPoint,
	QT_GuildDeleteGuildUIDOfCharacter,
	QT_GuildAddGuildFame,				// 2005-12-27 by cmkwon, 여단명성 수정
	QT_GuildUpdateCommander,			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	QT_GuildNotice,						// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
	QT_GuildGetApplicant,				// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	QT_GuildGetIntroduction,			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	QT_GuildDeleteIntroduction,			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	QT_GetSelfIntroduction,				// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서
	QT_GuildSearchIntroduction,			// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
	QT_GuildUpdateIntroduction,			// 2008-05-27 by dhjin,	EP3 - 여단 수정 사항 - 여단 소개 작성
	QT_GuildUpdateSelfIntroduction,		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
	QT_GuildDeleteSelfIntroduction,		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
	QT_GuildDeleteSelfIntroductionOffUser,		// 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 

	// Trade
	QT_ExecuteTrade,
	QT_TradeMoveItem,					// 2006-05-11 by cmkwon, 거래시 아이템 이동
	// Enchant
	QT_InsertEnchant,
	QT_DeleteAllEnchant,
	// Quest
	QT_GetAllQuest,					// 사용자의 진행중이거나 완료된 모든 quest를 loading
	QT_InsertQuest,					// 사용자의 quest를 삽입
	QT_UpdateQuestState,			// Quest 진행 상태 업데이트
	QT_DeleteQuest,					// Quest를 지움
	QT_UpdateQuestMonsterCount,		// 몬스터 카운트 추가 혹은 수정
	QT_DeleteQuestMonsterCount,		// 몬스터 카운트 삭제

	// Auction
	QT_AuctionGetItemList,		// 경매 아이템 목록 요청
	QT_AuctionRegisterItem,		// 경매 아이템 등록
	QT_AuctionCancelRegister,	// 경매 아이템 등록 취소
	QT_AuctionBuyItem,			// 경매 아이템 구매
	// 일반 query 실행
	QT_GeneralExec,
	// Pre Server
	QT_PRE_Login,
	QT_BlockUser,
	QT_UnblockUser,
	//QT_MGameLogin,
	QT_LogTotalUser,
	//QT_InsertGlogAccount,		// 2010-06-01 by shcho, GLogDB 관련 -
	// IM Server - Friend List
	QT_FriendInsertFriend,
	QT_FriendDeleteFriend,
	QT_FriendLoadFriends,
	// 유료화 프리미엄 카드 
	QT_CashInsertPremiumCard,
	QT_CashUpdatePremiumCard,
	QT_CashDeletePremiumCard,
	// HappyHourEvent 관련
	QT_LoadHappyHourEvent,
	QT_UpdateHappyHourEventDate,
	// 2006-08-31 by dhjin, ItemEvent 관련
	QT_LoadItemEvent,

	// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 
	//	QT_UpdatePCBangList,	// 2007-01-22 by dhjin, PC방 

	// 2006-09-04 by dhjin, 퀵 슬롯 관련
	QT_Get_QuickSlot,
	QT_Delete_QuickSlot,
	QT_Update_QuickSlot,
	// 2007-02-28 by dhjin, 전략포인트 생성 주기 수정 관련.
	QT_LoadStrategyPointSummonInfo,
	QT_UpdateStrategyPointSummonInfoBySummon,
	QT_UpdateStrategyPointSummonInfoBySummonTime,
	QT_InsertStrategyPointSummonInfoBySummon,
	QT_DeleteStrategyPointSummonInfoBySummon,
	QT_UpdateStrategyPointSummonInfoByNewSummonRange,
	QT_LoadStrategyPointSummonRange,
	QT_LoadStrategyPointNotSummonTime,
	QT_LoadStrategyPointNotSummonTimeByAdminTool,

	QT_UpdateInfluenceWarData,			// 2006-04-14 by cmkwon
	QT_UpdateOwnerOfConflictArea,		// 2006-07-12 by cmkwon, 분쟁지역 소유자 

	QT_UpdateSubleader,					// 2007-02-13 by dhjin, 부지도자 설정

	//QT_JamboreeInit,					// 2007-04-10 by cmkwon, 대회서버군 DB(atum2_db_20) 초기화
	//QT_JamboreeEntrants,				// 2007-04-10 by cmkwon, 캐릭터를 대회서버군 DB(atum2_db_20)로 복사

	// 2007-04-25 by dhjin, WarPoint 관련
	QT_UpdateWarPoint,					// 2007-04-25 by dhjin
	// 2007-06-07 by dhjin, Arena결과 관련
	QT_UpdateArenaResult,					// 2007-06-07 by dhjin
	QT_UpdateArenaDisConnect,				// 2007-06-07 by dhjin

	// 2007-07-06 by dhjin, Tutorial
	QT_LoadTutorialComplete,
	QT_InsertTutorialComplete,

	// 2007-08-21 by dhjin, OutPost
	QT_SetNPCPossessionOutPost,
	QT_SetGuildPossessionOutPost,
	QT_SetOutPostNextWarTime,

	// 2007-08-22 by dhjin, CityLeader
	QT_InitExpediencyFund,
	QT_SetExpediencyFund,
	QT_SetExpediencyFundPayBack,
	QT_SetExpediencyFundRate,
	QT_DeleteCityLeaderInfo,
	QT_RegNotice,
	QT_ModifyNotice,

	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
	QT_UpdateSecondaryPassword,		// 2차패스워드 업데이트

	// 2007-10-29 by dhjin, poll관련
	QT_LoadVoterList,
	QT_LoadLeaderCandidate,
	QT_SelectLeaderCandidateInfoByRealTimeVariable,
	QT_InsertLeaderCandidate,
	QT_UpdateLeaderDeleteCandidate,
	QT_UpdateLeaderPollCount,
	QT_InsertVoterList,

	QT_CheckGiveTarget,			// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 추가
	QT_UpdatePilotFace,			// 2007-11-21 by cmkwon, PilotFace 변경 카드 구현 - 추가

	QT_InsertNotifyMsg,			// 2007-11-28 by cmkwon, 통지시스템 구현 -
	QT_GetNotifyMsg,			// 2007-11-28 by cmkwon, 통지시스템 구현 -
	QT_DeleteNotifyMsg,			// 2007-11-28 by cmkwon, 통지시스템 구현 -

	QT_GetGuildMark,			// 2007-12-07 by dhjin

	//////////////////////////////////////////////////////////////////////////
	// 2007-12-28 by dhjin, 아레나 통합 - 
	QT_ArenaUpdateCharacterInfo,		 // 2007-12-28 by dhjin, 아레나 통합 - 케릭터 정보 업데이트
	QT_ArenaGetCharacter,				// 2007-12-29 by dhjin, 아레나 통합 - 케릭터 정보 로딩
	QT_ArenaCopyDBInfo,					// 2008-01-08 by dhjin, 아레나 통합 - 아레나 DB로 데이타 복사
	QT_ArenaStartGetCharacter,			// 2008-01-09 by dhjin, 아레나 통합 - 아레나 시작을 위해 케릭터 정보 다시 로딩하여 클라이언트에게 전송

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin,	모선전, 거점전 정보창 기획안 -
	QT_GetLogMSWarInfo,
	QT_GetLogSPWarInfo,
	QT_UpdateMSWarOptionType,
	QT_InsertMSWarLog,	// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
	QT_InsertSPWarLog,	// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	QT_UpdateDBServerGroup,				// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	QT_CheckConnectableAccount,			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-23 by dhjin, EP3 유저정보옵션 -
	QT_GetUserInfo,						// 2008-06-23 by dhjin, EP3 유저정보옵션 - 다른 유저 정보 요청

	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - DQT_DailyJob 로 변경 함.
	//	//////////////////////////////////////////////////////////////////////////
	//	// 2008-08-19 by dhjin, MySQL포팅 문제로 MySQL에서 지원하지 않는 MSSQL에 Job관련 작업을 여기서 처리한다.
	//	QT_DailyJob,

	// 2008-11-04 by dhjin, 럭키머신
	QT_UpdateLuckyItemDropCount,
	QT_UpdateLuckyItemStarttime,

	QT_ReloadAdminAutoNotice,		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 


	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고
	QT_UpdateStartDeclarationOfWar,
	QT_UpdateEndDeclarationOfWar,
	QT_UpdateMSWarStartTime,

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	QT_UpdateNickName,
	QT_GetSelfRanking,

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 -
	QT_ChangeItemWithItemMatching,		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 해당 계정의 창고 아이템을 세력 변경 처리한다.

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	QT_ChangeStartCityMapIndex,			// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - QT_ 추가
	QT_ChangeAddedInventoryCount,		// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	
	QT_LoadInfinityImpute,					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 귀속 정보 가져오기
	QT_InsertInfinityImpute,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 귀속 정보 추가
	QT_UpdateInfinityImpute,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 완료 
	QT_ResetInfinityImpute,					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	해당 인피 리셋으로 정보 삭제
	QT_ArenaCopyInfinityDBInfo,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 아레나DB에 복사하기
	QT_CharacterSaveDataInfinityFin,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
	QT_InfinityFinUpdateItem,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
	QT_InfinityFinInsertItem,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가ㅣ
	QT_InfinityInsertLog,					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 로그
	QT_CharacterSaveDataInfinityFinByDisconnect,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
	QT_InfinityFinUpdateItemByDisconnect,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
	QT_InfinityFinInsertItemByDisconnect,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가ㅣ
	QT_ResetInfinityImputeByServerStart,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 서버 시작시 인피 리셋
	QT_InfinityComBackPostWork,				// 2012-01-16 by hskim, 통계 - 화패

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	//	QT_LoadTenderInfo,						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender

	QT_CashLoadPremiumCard,					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 프리미엄 정보 가져오기
	QT_LoadInfinityShopInfo,				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	QT_UpdateItemCoolingTimeStamp,			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	QT_InfinityUpdateUserMapInfo,			// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	QT_LoadBurningMap,						// 2010-08-05 by dhjin, 버닝맵 -
	QT_Log_UserGetTenderItem,				// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장			

	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

	QT_PetUpdateInfinityFin,				// 2011-08-22 by hskim, 펫 정보 업데이트 (인피니티 종료 시 업데이트)
	QT_PetSetName,							// 2011-08-22 by hskim, 펫 이름 설정
	QT_PetSetExpRatio,						// 2011-08-22 by hskim, 펫 경험치 획득 비율 설정
	QT_PetSetLevel,							// 2011-08-22 by hskim, 펫 레벨 설정
	QT_PetSetExp,							// 2011-08-22 by hskim, 펫 경험치 설정
	QT_PetSetSocket,						// 2011-08-30 by hskim, 펫 소켓 설정
	QT_PetSetKitSlot,						// 2011-08-30 by hskim, 펫 키트 슬롯 설정
	QT_PetSetAutoSkillSlot,					// 2011-08-30 by hskim, 펫 오토 스킬 슬롯 설정
	QT_PetChangeSocketOwner,				// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
	QT_StoreGetItemOne,						// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

	QT_GetEnchantCount,						// 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)
	QT_GetAddExpRestExperienceCount,		// 2012-10-07 by hskim, 휴식 경험치
	QT_SetAddExpRestExperienceCount,		// 2012-10-07 by hskim, 휴식 경험치

	//QT_GetAddKillRestKillCount,				// 2015-06-11 by Future, Kill Count System
	//QT_SetAddKillrestKillCount,				// 2015-06-11 by Future, Kill Count System

	QT_GetFixedTerm,						// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	QT_InsertFixedTerm,						// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	QT_DeleteFixedTerm,						// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

	// end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
	/////////////////////////////////////////////////////////////

	QT_UpgradePet,							// 2015-06-22 Future, upgrading of Pets with sockets

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-19 by cmkwon, Log Server
	QT_FL_LOG_LOGIN = 10000,
	QT_FL_LOG_LOGOUT,
	QT_FL_LOG_LEVEL,
	QT_FL_LOG_EXP,
	QT_FL_LOG_STAT,
	QT_FL_LOG_GAME_START,
	QT_FL_LOG_GAME_END,
	QT_FL_LOG_QUEST_COMPLETION,
	QT_FL_LOG_QUEST_COMPENSATION,
	QT_FL_LOG_PKWIN,
	QT_FL_LOG_PKLOSS,
	QT_FL_LOG_DEAD,
	QT_FL_LOG_WARP,
	QT_FL_LOG_SPEED_HACK,
	QT_FL_LOG_CREATE_CHARACTER,
	QT_FL_LOG_DELETE_CHARACTER,
	QT_FL_LOG_MEMORY_HACK,
	QT_FL_LOG_PvEWIN,
	QT_FL_LOG_PvELOSS,
	QT_FL_LOG_ITEM_CHARACTER_ITEM,
	QT_FL_LOG_ITEM_GET_ITEM,
	QT_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND,
	QT_FL_LOG_ITEM_INSERT_TO_STORE,
	QT_FL_LOG_ITEM_DRAW_FROM_STORE,
	QT_FL_LOG_ITEM_TRADE_SEND,
	QT_FL_LOG_ITEM_TRADE_RECV,
	QT_FL_LOG_GUILD_STORE,
	QT_FL_LOG_ITEM_THROWAWAY_ITEM,
	QT_FL_LOG_ITEM_BUY_ITEM,
	QT_FL_LOG_ITEM_SELL_ITEM,
	QT_FL_LOG_ITEM_USE_ITEM,
	QT_FL_LOG_ITEM_USE_ENERGY,
	QT_FL_LOG_ITEM_USE_ENCHANT,
	QT_FL_LOG_ITEM_STORE_ITEM,
	QT_FL_LOG_ITEM_USE_MIX,
	QT_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN,
	QT_FL_LOG_SERVER_INFO_MAP,
	QT_FL_LOG_SERVER_INFO_TOTAL,
	QT_FL_LOG_MONSTER_BOSS,
	QT_FL_LOG_START_FIELD_SERVER,
	QT_FL_LOG_MS_WAR,		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	QT_FL_LOG_SP_WAR,		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	QT_FL_LOG_STATISTICS_MONEY,		// 2012-01-16 by hskim, 통계 - 화패


	QT_NA,		// 2008-12-01 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - DQT_ <== Dynamic Query Type

	DQT_DailyJob,			// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	DQT_UpdatePCBangList,	// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - PCBang IP 리스트 리로드
	DQT_ReloadWRKServiceList,	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	DQT_ReloadWRKLevel,			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	DQT_ReloadWRKFame,			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	DQT_ReloadWRKPVP,			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	DQT_LoadInfluenceRate,				// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	DQT_NA,

	// 2010-06-01 by shcho, GLogDB 관련 -
	QT_FL_GLOG_BUY_CASH_ITEM = 30000,
	QT_FL_GLOG_CHARACTER_PLAY_TIME,
	QT_FL_GLOG_CONNECT_TOTAL_USER_COUNT,
	QT_FL_GLOG_CONNECT_ZONE_USER_COUNT,
	QT_FL_GLOG_EVENT_PARTICIPATION_RATE,

	// 2011-12-12 by hskim, GLog 2차
	QT_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc,	// LogServer
	QT_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc,	// LogServer
	QT_FL_GLOG_TB_CONNECT_USER,								// LogServer
	QT_FL_GLOG_TB_ITEM_BUY,									// LogServer
	QT_FL_GLOG_TB_CONCURRENT_USER,							// LogServer
	QT_FL_GLOG_TB_USER_PLAYTIME,							// LogServer

	// 2011-01-26 by hskim, 인증 서버 구현
	QT_AUTH_CONFIRM,
	QT_AUTH_INSERT_CONFIRM_LOG,

	// 2012-01-13 by hskim, EP4 [동영상 1회 재생]
	QT_UpdateAccountLastGameEndDate,

	//////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, 아레나 추가개발 - 아레나 플레이 카운트 확인 및 정보 Ipdata 추가
	QT_ArenaPlayCount,
	QT_MF_Updata_CharacterArena,
	// end 2012-04-12 by jhseol, 아레나 추가개발 - 아레나 플레이 카운트 확인 및 정보 Ipdata 추가

	// start 2012-10-08 by khkim, GLog
	QT_FL_LOG_ACCOUNTCONNECT,
	QT_FL_LOG_CHARDATA,
	QT_FL_LOG_ITEM,
	QT_FL_LOG_ITEMSTATE,
	QT_FL_LOG_SERVER,
	// end 2012-10-08 by khkim, GLog
	QT_ArenaCharacterReset	// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.

	//////////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
	, QT_LoadRenewalStrategyPointSummonInfo
	, QT_UpdateStrategyPointSummonInfo
	// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

	, QT_ArenaCharacterReload	// 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.
	, QT_InsertStoreItemFromXMLRPC		// 2013-03-13 by hskim, 웹 캐시 상점
	, QT_DeleteStoreItemFromXMLRPC		// 2013-03-13 by hskim, 웹 캐시 상점
	, QT_GetCharacterInfoFromXMLRPC		// 2013-03-13 by hskim, 웹 캐시 상점

	, QT_LoadCashBuyDate			// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
	, QT_InsertCashBuyDate		// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

	, QT_GetShapeStatLevel		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - QT_GetShapeStatLevel 추가
	, QT_InsertShapeStatLevel	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - QT_InsertShapeStatLevel 추가

	, QT_LoadMonthlyArmorEvent		// 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 이벤트 로드
	, QT_UpdateInfluenceConsecutiveVictorites		// 2013-05-09 by hskim, 세력 포인트 개선

	, QT_GetTemporarySystemInfomation		// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	, QT_GetStoreExtension					// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	, QT_InsertStoreExtension				// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	, QT_CollectionArmorListLoad				// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 로드
	, QT_CollectionArmorUpdate				// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 저장

	, QT_FL_LOG_ITEM_TRY_ENCHANT_INFO		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
	, QT_AccountInflChange					// 2013-07-26 by jhseol, 타 계정 세력변경

	#ifdef S_IP_UNIQUE_CONNECTION
	, QT_GetCharacterMultipleIPStatus		// 2015-11-24 Future, Multi IP restriction
	#endif // S_IP_UNIQUE_CONNECTION

	, QT_NationTransfer						// 2015-12-17 Future, Nation Change Card

	, QT_LoadBlockedAccounts // 2016-01-09 Future, improved Account block loading

	// 2016-01-09 Future, MAC Blocking
	, QT_LoadBlockedMACs
	, QT_BlockMAC
	, QT_UnblockMAC
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	, QT_GetWarVictoryDiff
#endif
};
