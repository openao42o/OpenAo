
// 2005-04-28 by cmkwon
// #include "StringDefineServer.h"
#ifndef _STRING_DEFINE_SERVER_H_
#define _STRING_DEFINE_SERVER_H_




///////////////////////////////////////////////////////////////////////////////
// 1 Atum
	// 1-1 
	#define STRERR_S_ATUMEXE_0001 "서버가 실행되어 있지 않습니다."
	#define STRERR_S_ATUMEXE_0002 "Socket Closed by Pre Server!"
	#define STRERR_S_ATUMEXE_0003 "자동 업데이트에 실패하였습니다.\r\n게임을 재설치해주시기 바랍니다.\r\n"
	#define STRERR_S_ATUMEXE_0004 "ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRERR_S_ATUMEXE_0005 "Unknown Error: %s(%#04x)"
	#define STRERR_S_ATUMEXE_0006 "다운로드 서버에 접속할 수 없습니다."
	#define STRERR_S_ATUMEXE_0007 "다운로드 파일의 크기를 알 수 없습니다."
	#define STRERR_S_ATUMEXE_0008 "업데이트 파일을 다운로드 받을 수 없습니다."
	#define STRERR_S_ATUMEXE_0009 "The system is out of memory or resources."
	#define STRERR_S_ATUMEXE_0010 "The .exe file is invalid."
	#define STRERR_S_ATUMEXE_0011 "파일을 찾을 수 없습니다."
	#define STRERR_S_ATUMEXE_0012 "The specified path was not found. "
// 2006-04-20 by cmkwon, 	#define STRERR_S_ATUMEXE_0013 "[Error]Unknown Message Type: %d(0x%08X)\n"
// 1_end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 2 - AtumLauncher
	// 2-1 STRMSG
	#define STRMSG_S_ATUMLAUNCHER_0000 "update completed"
	#define STRMSG_S_ATUMLAUNCHER_0001 "서버를 선택하세요."
	#define STRMSG_S_ATUMLAUNCHER_0002 "Updating Delete File List v%s\r\n"
	#define STRMSG_S_ATUMLAUNCHER_0003 "deleting files"
	#define STRMSG_S_ATUMLAUNCHER_0004 "updating notice"
	#define STRMSG_S_ATUMLAUNCHER_0005 "update completed(%s -> %s)"
	#define STRMSG_S_ATUMLAUNCHER_0006 "실행 파일 선택"
	#define STRMSG_S_ATUMLAUNCHER_0007 "실행 경로 선택"
	#define STRMSG_S_ATUMLAUNCHER_0008 "Downloading is canceled"
	#define STRMSG_S_ATUMLAUNCHER_0009 "Download finished"
	#define STRMSG_S_ATUMLAUNCHER_0010 "updating to v%s - %s(%d/%d)"
	#define STRMSG_S_ATUMLAUNCHER_0011 "Unable to create file %s"
	#define STRMSG_S_ATUMLAUNCHER_0012 "updating to v%s - %s(%d/%d)"
	#define STRMSG_S_ATUMLAUNCHER_0013 "Getting File Information %s"
	
	// STRERR
	#define STRERR_S_ATUMLAUNCHER_0000 "[Error] Parameter Count Error, Count(%d)\n"
	#define STRERR_S_ATUMLAUNCHER_0001 "[Error] Mutex Error\n"
	#define STRERR_S_ATUMLAUNCHER_0002 "[Error] Excute Type Error, Type(%s)\n"
	#define STRERR_S_ATUMLAUNCHER_0003 "[Error] Decryption ID Error, DecryptedID(%s)\n"
	#define STRERR_S_ATUMLAUNCHER_0004 "인증 서버에 접속할 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0005 "서버 접속에 실패하였습니다."
	#define STRERR_S_ATUMLAUNCHER_0006 "Socket Closed by Pre Server!"
	#define STRERR_S_ATUMLAUNCHER_0007 "Notice File Error!"
//	#define STRERR_S_ATUMLAUNCHER_0008 "게임을 다시 다운로드받아주세요.\nURL: http://space.mgame.com/\n최신 버전: "
	#define STRERR_S_ATUMLAUNCHER_0009 "자동 업데이트에 실패하였습니다.\r\n게임을 재설치해주시기 바랍니다.\r\n"
	#define STRERR_S_ATUMLAUNCHER_0010 "모든 서버가 비활성 상태입니다."
	#define STRERR_S_ATUMLAUNCHER_0011 "%-16s%s점검중..."
	#define STRERR_S_ATUMLAUNCHER_0012 "모든 서버가 점검중입니다. 잠시후에 접속해 주십시요."
	#define STRERR_S_ATUMLAUNCHER_0013 "ERROR %s(%#04X) received from %s[%s]\r\n"
// 2006-08-24 by cmkwon, Change to STRMSG_060824_0000
//	#define STRERR_S_ATUMLAUNCHER_0014 "틀린 아이디, 비밀번호 오류\n\n* 현재는 베타 테스터로 인증된 분들만 로그인이 가능합니다"
	#define STRERR_S_ATUMLAUNCHER_0015 "로그인 과정 오류"
	#define STRERR_S_ATUMLAUNCHER_0016 "사용자 아이디 미입력"
	#define STRERR_S_ATUMLAUNCHER_0017 "이중 로그인"
	#define STRERR_S_ATUMLAUNCHER_0018 "F 서버가 실행되어 있지 않습니다."
	#define STRERR_S_ATUMLAUNCHER_0019 "I 서버가 실행되어 있지 않습니다."
	#define STRERR_S_ATUMLAUNCHER_0020 "서비스가 일시적으로 중단되어 있습니다.\n\n잠시 후에 다시 접속해주세요."
	#define STRERR_S_ATUMLAUNCHER_0021 "현재 사용자가 너무 많습니다.\n\n잠시 후에 다시 접속해주세요."
	#define STRERR_S_ATUMLAUNCHER_0022 "현재 계정 압류 상태입니다.\n 기간 : %s\n\n자세한 사항은 에이스온라인 고객센터로 문의바랍니다."
	#define STRERR_S_ATUMLAUNCHER_0023 "클라이언트 버전이 맞지 않습니다.\n\n게임을 다시 다운로드 받아 주세요."
	#define STRERR_S_ATUMLAUNCHER_0024 "ERROR: %s(%#04X)"
	#define STRERR_S_ATUMLAUNCHER_0025 "다운로드 서버에 접속할 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0026 "다운로드 파일(%s)의 크기를 알 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0027 "다운로드 파일의 크기를 알 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0028 "업데이트 파일을 다운로드 받을 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0029 "업데이트 파일이 존재하지 않습니다."
	#define STRERR_S_ATUMLAUNCHER_0030 "선택하신 서버는 점검중입니다. 잠시후에 접속해 주십시요."
	#define STRERR_S_ATUMLAUNCHER_0031 "The system is out of memory or resources."
	#define STRERR_S_ATUMLAUNCHER_0032 "The .exe file is invalid."
	#define STRERR_S_ATUMLAUNCHER_0033 "파일을 찾을 수 없습니다."
	#define STRERR_S_ATUMLAUNCHER_0034 "경로를 찾을 수 없습니다."
// 2006-04-20 by cmkwon	#define STRERR_S_ATUMLAUNCHER_0035 "[Error] Unhandled Message Type: %s(%#04X)\n"
// 2006-04-20 by cmkwon	#define STRERR_S_ATUMLAUNCHER_0036 "[Error] Unhandled Message Type!\n"

	#define STRMSG_S_050506		"\'%s\'님은 현재 계정 압류 상태입니다.\n  사유: %s\n  기간: %s~%s\n\n자세한 사항은 에이스온라인 고객센터로 문의바랍니다."
	#define STRMSG_S_050930		"게임을 다시 다운로드받아주세요.\nURL: %s\n최신 버전: "
// 2_end
///////////////////////////////////////////////////////////////////////////////	

///////////////////////////////////////////////////////////////////////////////
// 3 - AtumAdminTool
	// 3-1 STRMSG
//	#define STRMSG_S_SCADMINTOOL_0000 "남"
//	#define STRMSG_S_SCADMINTOOL_0001 "여"
//	#define STRMSG_S_SCADMINTOOL_0002 "%d년, %d세"
//	#define STRMSG_S_SCADMINTOOL_0003 "정말 계정 정보를 수정하시겠습니까?"
//	#define STRMSG_S_SCADMINTOOL_0004 "CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
//	#define STRMSG_S_SCADMINTOOL_0005 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 습득, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
//	#define STRMSG_S_SCADMINTOOL_0006 "'''' + l.PeerCharacterName + '''에게 ' + CAST(l.ChangeCount AS VARCHAR(10)) + '개 줌, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
//	#define STRMSG_S_SCADMINTOOL_0007 "'''' + l.PeerCharacterName + '''에게 ' + CAST(l.ChangeCount AS VARCHAR(10)) + '개 받음, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
//	#define STRMSG_S_SCADMINTOOL_0008 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 버림, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
//	#define STRMSG_S_SCADMINTOOL_0009 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 구매, 남은 Spi: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
//	#define STRMSG_S_SCADMINTOOL_0010 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 판매, 남은 Spi: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
//	#define STRMSG_S_SCADMINTOOL_0011 "CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0012 "'''' + l.PeerCharacterName + '''가 ' + CAST(l.ChangeCount AS VARCHAR(10)) + '개 추가함, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0013 "'''' + l.PeerCharacterName + '''가 ' + CAST(l.ChangeCount AS VARCHAR(10)) + '개 삭제함, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0014 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 추가, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0015 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 맡김, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0016 "CAST(l.ChangeCount AS VARCHAR(10)) + '개 찾음, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '개'"
	#define STRMSG_S_SCADMINTOOL_0017 "\'게임시간 \' + dbo.atum_GetHMSFromS(l.PlayTime) + \', 총 \' + dbo.atum_GetHMSFromS(l.TotalPlayTime)"
	#define STRMSG_S_SCADMINTOOL_0018 "CAST(l.Param1 AS VARCHAR(10)) + ' -> ' + CAST(l.Param2 AS VARCHAR(10)) + ', 게임시간: ' + dbo.atum_GetHMSFromS(l.Param3)"
//	#define STRMSG_S_SCADMINTOOL_0019 "CAST(l.Param1 AS VARCHAR(15)) + \' 증감, 총 \' + CAST(l.Param2 AS VARCHAR(15))"
//	#define STRMSG_S_SCADMINTOOL_0020 "충돌"
//	#define STRMSG_S_SCADMINTOOL_0021 "몬스터"
//	#define STRMSG_S_SCADMINTOOL_0022 "기어"
//	#define STRMSG_S_SCADMINTOOL_0023 "원인불명"
//	#define STRMSG_S_SCADMINTOOL_0024 "%s, 남은 스탯: %s"
	#define STRMSG_S_SCADMINTOOL_0025 "(없음)"
//	#define STRMSG_S_SCADMINTOOL_0026 "나"
//	#define STRMSG_S_SCADMINTOOL_0027 "없음"
//	#define STRMSG_S_SCADMINTOOL_0028 "버그 사용 유저"
	#define STRMSG_S_SCADMINTOOL_0029 "계정 블럭"
	#define STRMSG_S_SCADMINTOOL_0030 "채팅 금지"
//	#define STRMSG_S_SCADMINTOOL_0031 "접속 로그"
//	#define STRMSG_S_SCADMINTOOL_0032 "유저 로그"
//	#define STRMSG_S_SCADMINTOOL_0033 "아이템 로그"
	#define STRMSG_S_SCADMINTOOL_0034 "%s - %s 서버"
	#define STRMSG_S_SCADMINTOOL_0035 "%s - %s 서버,%d(%d)"
	#define STRMSG_S_SCADMINTOOL_0036 " 계정 압류"
//	#define STRMSG_S_SCADMINTOOL_0037 "분류    "
//	#define STRMSG_S_SCADMINTOOL_0038 "값"
	#define STRMSG_S_SCADMINTOOL_0039 "현재 계정의 압류 상태를 해제하시겠습니까?"
	#define STRMSG_S_SCADMINTOOL_0040 "%s(%d초)"
	#define STRMSG_S_SCADMINTOOL_0041 "%d승 %d패"
	#define STRMSG_S_SCADMINTOOL_0042 "정말 계정 연결을 끊고 막으시겠습니까?"
	#define STRMSG_S_SCADMINTOOL_0043 "%s 아이템"
	#define STRMSG_S_SCADMINTOOL_0044 "[%s %15s] 공지사항 : %s\r\n"
	#define STRMSG_S_SCADMINTOOL_0045 "사용자수: %d\r\n"
	#define STRMSG_S_SCADMINTOOL_0046 "[%s %15s] 받은메시지 : %s\r\n"
	#define STRMSG_S_SCADMINTOOL_0047 "[%s %15s] 유저수 : %4d\r\n"
	#define STRMSG_S_SCADMINTOOL_0048 "[%s %15s] FieldServer state : %d\r\n"
	#define STRMSG_S_SCADMINTOOL_0049 "서버와의 연결이 종료되었습니다\nSocket Name: %s\nIP: %s"
	
	// 3-2 AtumAdminTool - STRERR
	#define STRERR_S_SCADMINTOOL_0000 "해당 계정 정보를 읽어올 수 없습니다."
	#define STRERR_S_SCADMINTOOL_0001 "계정 정보 수정에 실패하였습니다."
	#define STRERR_S_SCADMINTOOL_0002 "비밀번호를 입력하시요"
	#define STRERR_S_SCADMINTOOL_0003 "비밀번호가 틀립니다"
	#define STRERR_S_SCADMINTOOL_0004 "로그인 이름을 입력하세요"
	#define STRERR_S_SCADMINTOOL_0005 "Pre Server가 실행되어 있지 않습니다."
	#define STRERR_S_SCADMINTOOL_0006 "Can not connect to PreServer !!"
//	#define STRERR_S_SCADMINTOOL_0007 "인증 실패하였습니다"			// 2006-04-11 by cmkwon, 주석처리함
	#define STRERR_S_SCADMINTOOL_0008 "ERROR: 해당 프로토콜이 존재하지 않습니다."
	#define STRERR_S_SCADMINTOOL_0009 "사용자명을 입력하세요"
	#define STRERR_S_SCADMINTOOL_0010 "사유를 입력하세요"
	#define STRERR_S_SCADMINTOOL_0011 "경험치 설정 오류 : Level %2d ==> Exp(%.1I64f ~ %.1I64f)"
	#define STRERR_S_SCADMINTOOL_0012 "최대 로그 수가 초과되었습니다.\r\n\r\n최대 로그 수나 검색 조건을 수정 바랍니다."
	#define STRERR_S_SCADMINTOOL_0013 "Database에 연결할 수 없습니다."
	#define STRERR_S_SCADMINTOOL_0014 "연결안됨"
	#define STRERR_S_SCADMINTOOL_0015 "연결됨"
	#define STRERR_S_SCADMINTOOL_0016 "업데이트중"
	#define STRERR_S_SCADMINTOOL_0017 "로그인됨"
	#define STRERR_S_SCADMINTOOL_0018 "캐릭터선택중"
	#define STRERR_S_SCADMINTOOL_0019 "게임중"
	#define STRERR_S_SCADMINTOOL_0020 "알수없음"
	#define STRERR_S_SCADMINTOOL_0021 "Database %s(%s:%d)에 연결할 수 없습니다."
	#define STRERR_S_SCADMINTOOL_0022 "아이템 추가 실패"
	#define STRERR_S_SCADMINTOOL_0023 "압류 계정 찾기 실패하였습니다."
	#define STRERR_S_SCADMINTOOL_0024 "수정을 위해 먼저 계정 연결을 끊고 계정 압류해 주세요!"
	#define STRERR_S_SCADMINTOOL_0025 "SPI(머니)는 추가하지 못합니다."
	#define STRERR_S_SCADMINTOOL_0026 "선택한 아이템은 이미 존재하므로 개수를 수정하시기 바랍니다"
	#define STRERR_S_SCADMINTOOL_0027 "아이템 찾기 에러"
	#define STRERR_S_SCADMINTOOL_0028 "캐릭터가 소유한 SPI(머니)는 삭제하지 못합니다."
	#define STRERR_S_SCADMINTOOL_0029 "아이템을 삭제하시겠습니까?"
	#define STRERR_S_SCADMINTOOL_0030 "아이템 삭제가 실패되었습니다."
	#define STRERR_S_SCADMINTOOL_0031 "아이템 수정이 실패되었습니다."
	#define STRERR_S_SCADMINTOOL_0032 "계정을 입력하세요"
	#define STRERR_S_SCADMINTOOL_0033 "계정이 존재하지 않습니다.(계정블럭 설정은 가능함)"
	#define STRERR_S_SCADMINTOOL_0034 "계정 혹은 캐릭이 존재하지 않습니다."
	#define STRERR_S_SCADMINTOOL_0035 "캐릭터 정보 조회 에러입니다."
	#define STRERR_S_SCADMINTOOL_0036 "해당 캐릭터가 존재하지 않습니다"
	#define STRERR_S_SCADMINTOOL_0037 "캐릭터 업데이트 오류입니다"
	#define STRERR_S_SCADMINTOOL_0038 "캐릭터 정보가 성공적으로 업데이트되었습니다"
	#define STRERR_S_SCADMINTOOL_0039 "아이템을 선택하세요."
	#define STRERR_S_SCADMINTOOL_0040 "아이템 개수를 선택하세요."
	#define STRERR_S_SCADMINTOOL_0041 "해당 아이템은 5개 이하로만 생성 가능합니다."
	#define STRERR_S_SCADMINTOOL_0042 "메시지를 입력하세요."
	#define STRERR_S_SCADMINTOOL_0043 "공지를 전송할 수 없습니다."
	#define STRERR_S_SCADMINTOOL_0044 "%s: server state(%d)\r\n"
	#define STRERR_S_SCADMINTOOL_0045 "IM Server가 실행되어 있지 않습니다."
	#define STRERR_S_SCADMINTOOL_0046 "Field Server가 실행되어 있지 않습니다."
	#define STRERR_S_SCADMINTOOL_0047 "[%s %15s] IMServer state : %d\r\n"

	// 3-3 AtumLaAtumAdminTool -
//	#define STRMSG_S_SCAT_COLNAME_0000 "계정명"
//	#define STRMSG_S_SCAT_COLNAME_0001 "타입"
//	#define STRMSG_S_SCAT_COLNAME_0002 "전과"
//	#define STRMSG_S_SCAT_COLNAME_0003 "시작일"
//	#define STRMSG_S_SCAT_COLNAME_0004 "종료일"
//	#define STRMSG_S_SCAT_COLNAME_0005 "처리자"
//	#define STRMSG_S_SCAT_COLNAME_0006 "처리사유"
//	#define STRMSG_S_SCAT_COLNAME_0007 "날짜"
//	#define STRMSG_S_SCAT_COLNAME_0008 "로그타입"
//	#define STRMSG_S_SCAT_COLNAME_0009 "IPAddress"
//	#define STRMSG_S_SCAT_COLNAME_0010 "서버명"
//	#define STRMSG_S_SCAT_COLNAME_0011 "캐릭터명"
//	#define STRMSG_S_SCAT_COLNAME_0012 "위치"
//	#define STRMSG_S_SCAT_COLNAME_0013 "내용"
//	#define STRMSG_S_SCAT_COLNAME_0014 "아이템"
//	#define STRMSG_S_SCAT_COLNAME_0015 "UID"
//	#define STRMSG_S_SCAT_COLNAME_0016 "고유번호"
//	#define STRMSG_S_SCAT_COLNAME_0017 "이름"
//	#define STRMSG_S_SCAT_COLNAME_0018 "아이템번호"
//	#define STRMSG_S_SCAT_COLNAME_0019 "Prefix"
//	#define STRMSG_S_SCAT_COLNAME_0020 "Suffix"
	#define STRMSG_S_SCAT_COLNAME_0021 "장착"
//	#define STRMSG_S_SCAT_COLNAME_0022 "개수"
//	#define STRMSG_S_SCAT_COLNAME_0023 "내구도"
//	#define STRMSG_S_SCAT_COLNAME_0024 "생성시간"
	#define STRMSG_S_SCAT_COLNAME_0025 "미장착"
//	#define STRMSG_S_SCAT_COLNAME_0026 "창고"
//	#define STRMSG_S_SCAT_COLNAME_0027 "경매"
//	#define STRMSG_S_SCAT_COLNAME_0028 "맵"
//	#define STRMSG_S_SCAT_COLNAME_0029 "좌표"
//	#define STRMSG_S_SCAT_COLNAME_0030 "성별"
//	#define STRMSG_S_SCAT_COLNAME_0031 "종족"
//	#define STRMSG_S_SCAT_COLNAME_0032 "권한"
//	#define STRMSG_S_SCAT_COLNAME_0033 "유닛종류"
//	#define STRMSG_S_SCAT_COLNAME_0034 "레벨"
//	#define STRMSG_S_SCAT_COLNAME_0035 "최고레벨"
//	#define STRMSG_S_SCAT_COLNAME_0036 "경험치"
//	#define STRMSG_S_SCAT_COLNAME_0037 "감소된경험치"
//	#define STRMSG_S_SCAT_COLNAME_0038 "자동분배스탯타입"
//	#define STRMSG_S_SCAT_COLNAME_0039 "공격파트"
//	#define STRMSG_S_SCAT_COLNAME_0040 "방어파트"
//	#define STRMSG_S_SCAT_COLNAME_0041 "연료파트"
//	#define STRMSG_S_SCAT_COLNAME_0042 "감응파트"
//	#define STRMSG_S_SCAT_COLNAME_0043 "쉴드파트"
//	#define STRMSG_S_SCAT_COLNAME_0044 "회피파트"
//	#define STRMSG_S_SCAT_COLNAME_0045 "소속여단"
//	#define STRMSG_S_SCAT_COLNAME_0046 "성향"
//	#define STRMSG_S_SCAT_COLNAME_0047 "신분"
//	#define STRMSG_S_SCAT_COLNAME_0048 "총접속시간"
//	#define STRMSG_S_SCAT_COLNAME_0049 "생성시간"
//	#define STRMSG_S_SCAT_COLNAME_0050 "최종접속시간"
//	#define STRMSG_S_SCAT_COLNAME_0051 "종류"
//	#define STRMSG_S_SCAT_COLNAME_0052 "전체"
	#define STRMSG_S_SCAT_COLNAME_0053 "오토매틱류(1-1형)"
	#define STRMSG_S_SCAT_COLNAME_0054 "발칸류(1-1형)"	
	#define STRMSG_S_SCAT_COLNAME_0055 "듀얼리스트류(1-1형)" // 2005-08-01 by hblee : 그러네이드 -> 듀얼리스트로 변경.
	#define STRMSG_S_SCAT_COLNAME_0056 "캐논류(1-1형)"
	#define STRMSG_S_SCAT_COLNAME_0057 "라이플류(1-2형)"
	#define STRMSG_S_SCAT_COLNAME_0058 "개틀링류(1-2형)"
	#define STRMSG_S_SCAT_COLNAME_0059 "런처류(1-2형)"
	#define STRMSG_S_SCAT_COLNAME_0060 "메스드라이브류(1-2형)"
	#define STRMSG_S_SCAT_COLNAME_0061 "로켓류(2-1형)"
	#define STRMSG_S_SCAT_COLNAME_0062 "미사일류(2-1형)"
	#define STRMSG_S_SCAT_COLNAME_0063 "번들류(2-1형)"
	#define STRMSG_S_SCAT_COLNAME_0064 "마인류(2-1형)"
	#define STRMSG_S_SCAT_COLNAME_0065 "쉴드류(2-2형)"
	#define STRMSG_S_SCAT_COLNAME_0066 "더미류(2-2형)"
	#define STRMSG_S_SCAT_COLNAME_0067 "픽서류(2-2형)"
	#define STRMSG_S_SCAT_COLNAME_0068 "디코이류(2-2형)"
	#define STRMSG_S_SCAT_COLNAME_0069 "방어류"
	#define STRMSG_S_SCAT_COLNAME_0070 "보조장비류"
	#define STRMSG_S_SCAT_COLNAME_0071 "에너지류"
	#define STRMSG_S_SCAT_COLNAME_0072 "금속류"
	#define STRMSG_S_SCAT_COLNAME_0073 "카드류"
	#define STRMSG_S_SCAT_COLNAME_0074 "인챈트류"
	#define STRMSG_S_SCAT_COLNAME_0075 "탱크류"
	#define STRMSG_S_SCAT_COLNAME_0076 "탄알류"
	#define STRMSG_S_SCAT_COLNAME_0077 "퀘스트용"
	#define STRMSG_S_SCAT_COLNAME_0078 "레이더류"
	#define STRMSG_S_SCAT_COLNAME_0079 "컴퓨터류"
	#define STRMSG_S_SCAT_COLNAME_0080 "갬블카드류"
	#define STRMSG_S_SCAT_COLNAME_0081 "인챈트파괴방지류"		// 2005-08-02 by cmkwon
	#define STRMSG_S_SCAT_COLNAME_0082 "블래스터류"				// 2005-08-02 by cmkwon
	#define STRMSG_S_SCAT_COLNAME_0083 "레일건류"				// 2005-08-02 by cmkwon
//	#define STRMSG_S_SCAT_COLNAME_0081 "전체 아이템"
//	#define STRMSG_S_SCAT_COLNAME_0082 "전체 서버"
//	#define STRMSG_S_SCAT_COLNAME_0083 "서버명"
//	#define STRMSG_S_SCAT_COLNAME_0084 "서버 종류"
//	#define STRMSG_S_SCAT_COLNAME_0085 "서버 IP, Port"
//	#define STRMSG_S_SCAT_COLNAME_0086 "현재 유저 수"
//	#define STRMSG_S_SCAT_COLNAME_0087 "서버 상태"
//	#define STRMSG_S_SCAT_COLNAME_0088 "그룹 서버"
//	#define STRMSG_S_SCAT_COLNAME_0089 "필드 서버"
//	#define STRMSG_S_SCAT_COLNAME_0090 "알 수 없음"
//	#define STRMSG_S_SCAT_COLNAME_0091 "실행안됨"
//	#define STRMSG_S_SCAT_COLNAME_0092 "정상"
//	#define STRMSG_S_SCAT_COLNAME_0093 "비정상"
	#define STRMSG_S_SCAT_COLNAME_0094 "운영자"

	#define STRMSG_S_SCADMINTOOL_050512_0000	"CAST(l.CurrentCount AS VARCHAR(10)) + '개, Param1:' + CAST(l.Param1 AS VARCHAR(10))"
// 3_end
///////////////////////////////////////////////////////////////////////////////
	
	
///////////////////////////////////////////////////////////////////////////////
// 4
	// 4-1 AtumMonitor - STRMSG
//	#define STRMSG_S_SCMONITOR_0000 "명령어 리스트\r\n"
//	#define STRMSG_S_SCMONITOR_0001 "----- 백업된 계정 리스트 ---------------------------------------\r\n"
//	#define STRMSG_S_SCMONITOR_0002 "  계정: \'%s\', 원래 암호: \'%s\', 임시 암호: \'%s\'\r\n"
//	#define STRMSG_S_SCMONITOR_0003 "  계정: \'%s\' \r\n"
//	#define STRMSG_S_SCMONITOR_0004 "업데이트용 Version들이 속해 있는 폴더를 선택하세요"
//	#define STRMSG_S_SCMONITOR_0005 "\r\nMaking New Zip File From %s To %s...\r\n"
//	#define STRMSG_S_SCMONITOR_0006 "Rename Server Group"
//	#define STRMSG_S_SCMONITOR_0007 "파일이 성공적으로 생성되었습니다.\r\n\r\nDB 정보: %s(%d), %s"
//	#define STRMSG_S_SCMONITOR_0008 "%04d(%2d -  활성) %3d/%3d"
//	#define STRMSG_S_SCMONITOR_0009 "%04d(%2d -비활성) %3d/%3d"
//	#define STRMSG_S_SCMONITOR_0010 "서버의 데이타가 성공적으로 리로딩되었습니다."
//	#define STRMSG_S_SCMONITOR_0011 "서비스 상태가 성공적으로 반영되었습니다."
//	#define STRMSG_S_SCMONITOR_0012 "%04d(%2d -  활성)"
//	#define STRMSG_S_SCMONITOR_0013 "%04d(%2d -비활성)"
//	#define STRMSG_S_SCMONITOR_0014 "Version Info List Reload DONE!"
//	#define STRMSG_S_SCMONITOR_0015 "Blocked Account List Reload DONE!"
//	#define STRMSG_S_SCMONITOR_0016 "프리 서버 서비스가 중지되었습니다."
//	#define STRMSG_S_SCMONITOR_0017 "프리 서버 서비스가 시작되었습니다."
//	#define STRMSG_S_SCMONITOR_0018 "필드 서버가 실행되어 있지 않습니다"
//	#define STRMSG_S_SCMONITOR_0019 "정말 Field Server를 종료하시겠습니까?"
//	#define STRMSG_S_SCMONITOR_0020 "업데이트 버전 리스트 정보(최대 1492 Bytes)\r\n\r\n    버전리스트개수[%3d], 데이타용량[%4dBytes]"
//	#define STRMSG_S_SCMONITOR_0021 "정말 Pre Server를 종료하시겠습니까?"
//	#define STRMSG_S_SCMONITOR_0022 "정말 IM Server를 종료하시겠습니까?"
//	#define STRMSG_S_SCMONITOR_0023 "정말 NPC Server를 종료하시겠습니까?"
//	#define STRMSG_S_SCMONITOR_0024 "%Y년 %m월 %d일 %H시 %M분 %S초"
//	#define STRMSG_S_SCMONITOR_0025 "이벤트 없음(%d)"
//	#define STRMSG_S_SCMONITOR_0026 "오픈베타 출석 이벤트(%d)"
//	#define STRMSG_S_SCMONITOR_0027 "이벤트 모름(%d)"
//	#define STRMSG_S_SCMONITOR_0028 "다음 점령전 설정 시간"
//	#define STRMSG_S_SCMONITOR_0029 "다음 점령전 기본 시간"
//	#define STRMSG_S_SCMONITOR_0030 "점령여단"

	// 4-2 AtumMonitor - STRERR
//	#define STRERR_S_SCMONITOR_0000 "  ==> 명령어가 성공하였습니다.\r\n"
//	#define STRERR_S_SCMONITOR_0001 "  ==> 명령어가 실패하였습니다.\r\n"
//	#define STRERR_S_SCMONITOR_0002 "DB에 연결할 수 없습니다."
//	#define STRERR_S_SCMONITOR_0003 "해당 Version이 존재하지 않습니다"
//	#define STRERR_S_SCMONITOR_0004 "압축할 폴더 이름을 입력하세요"
//	#define STRERR_S_SCMONITOR_0005 "출력할 폴더 이름을 입력하세요"
//	#define STRERR_S_SCMONITOR_0006 "시작 버전을 입력하세요"
//	#define STRERR_S_SCMONITOR_0007 "끝 버전을 입력하세요"
//	#define STRERR_S_SCMONITOR_0008 "업데이트용 Zip 파일을 출력할 폴더를 선택하세요"
//	#define STRERR_S_SCMONITOR_0009 "파일을 연결할 수 없습니다"
//	#define STRERR_S_SCMONITOR_0010 "서버를 선택하시오!"
//	#define STRERR_S_SCMONITOR_0011 "DB에 연결할 수 없습니다"
//	#define STRERR_S_SCMONITOR_0012 "[Error]Unable to process Message Type: %s(%#04x) in CLeftView::OnSocketNotify()\n"
//	#define STRERR_S_SCMONITOR_0013 "업데이트 버전 리스트가 너무 많습니다.(버전리스트개수[%3d], 데이타용량[%4dBytes])\r\n\r\n    버전 리스트를 정리해야 합니다."
//	#define STRERR_S_SCMONITOR_0014 "제거된 기능입니다.\r\n관리 툴을 이용하세요."
//	#define STRERR_S_SCMONITOR_0015 "도시점령전맵이 아닙니다"
//	#define STRERR_S_SCMONITOR_0016 "EDIT 컨트롤을 만들수 없습니다."
//	#define STRERR_S_SCMONITOR_0017 "이미 존재하는 파일을 등록하였습니다."

// 4_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 5 - FieldServer
	// 5-1 Field<->Log
	#define STRMSG_S_F2LOGCONNECT_0000 "[Error] WndProc(), Can't connect to LogServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2LOGCONNECT_0001 "Log Server에 접속 하였습니다.\r\n"
	#define STRMSG_S_F2LOGCONNECT_0002 "Log Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"

	// 5-2 Field<->Pre
	#define STRMSG_S_F2PRECONNECT_0000 "[Error] WndProc(), Can't connect to PreServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2PRECONNECT_0001 "Pre Server에 접속 하였습니다.\r\n"
	#define STRMSG_S_F2PRECONNECT_0002 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRMSG_S_F2PRECONNECT_0003 "Unknown Error@WM_PRE_PACKET_NOTIFY: %s(%#04x)\r\n"
	#define STRMSG_S_F2PRECONNECT_0004 "Pre Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"

	// 5-3 Field<->IM
	#define STRMSG_S_F2IMCONNECT_0000 "[Error] WndProc(), Can't connect to  IMServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2IMCONNECT_0001 "IM Server에 접속 하였습니다.\r\n"
	#define STRMSG_S_F2IMCONNECT_0002 "IM Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"
	#define STRMSG_S_F2IMCONNECT_0003 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRMSG_S_F2IMCONNECT_0004 "Unknown Error@WM_IM_PACKET_NOTIFY: %s(%#04x)\n"

	// 5-3 Field - DB
	#define STRMSG_S_F2DBQUERY_0000 "No such DB query In Field Server Queries! %d\r\n"
	#define STRMSG_S_F2DBQUERY_0001 "해당 아이템이 존재하지 않습니다."
	#define STRMSG_S_F2DBQUERY_0002 "'%s'님께서는 출석 신청을 하셨기 때문에 약속한 아이템을"
	#define STRMSG_S_F2DBQUERY_0003 "지급합니다. F5를 눌러서 인벤토리를 확인해 보세요"
	#define STRMSG_S_F2DBQUERY_0004 "스킬 %s(%d)가 추가되었습니다"
	#define STRMSG_S_F2DBQUERY_0005 "해당 아이템이 존재하지 않습니다."
	#define STRMSG_S_F2DBQUERY_0006 "아이템 구매에 실패하였습니다."

	// 5-4 Field - CityWar
	#define STRMSG_S_F2CITYWAR_0000 "  도시점령전 시작 : %d(%10s) occGuildName(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0001 "		  참여 여단 : GuildUID(%4d) GuildName(%10s) GuildMaster(%d)\r\n"
	#define STRMSG_S_F2CITYWAR_0002 "  도시점령전 몬스터 폭발 : %d(%10s) occGuildName(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0003 "		  데미지합 ==> GuildName(%10s) SumOfDamage(%8.2f)\r\n"
	#define STRMSG_S_F2CITYWAR_0004 "  도시점령전 : %d(%10s) CityMapIndex(%d) QuestIndex(%d) OccGuildID(%d) OccGuildName(%s) OccGuildMasterUID(%d) 점령전시간(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0005 "[Error] SetCityWarState_ DBError, MapIndex(%d)\r\n"
	#define STRMSG_S_F2CITYWAR_0006 "%d분 후 \"%s\" 도시점령전이 시작 됩니다."
	#define STRMSG_S_F2CITYWAR_0007 "%d분 후 \"%s\" 도시점령전이 종료 됩니다."
	#define STRMSG_S_F2CITYWAR_0008 "도시점령전(%s) 몬스터 소환됨 : NPC 점령"
	#define STRMSG_S_F2CITYWAR_0009 "도시점령전(%s) 몬스터 소환됨 : %s 여단 점령"
	#define STRMSG_S_F2CITYWAR_0010 "\"NPC\"가 \"%s\"을 현재 점령 하였습니다."
	#define STRMSG_S_F2CITYWAR_0011 "\"%s\"여단이 \"%s\"을 현재 점령 하였습니다."

	// 5-4 Field - Quest
	#define STRMSG_S_F2QUEST_0000 "퀘스트 로딩 실패"
	#define STRMSG_S_F2QUEST_0001 "퀘스트가 로딩되지 않았습니다.\r\n"
//	#define STRMSG_S_F2QUEST_0002 "퀘스트 \'%30s\' 번호 %d -> OK\r\n"

	// 5-4 Field - config
	#define STRMSG_S_F2CONFIG_0000 "테스트용 서버 세팅됨! \r\n\r\nLoadFieldServerDataDebug() 제거바람! "
	#define STRMSG_S_F2NOTIFY_0000 "splash %d: %d에게 %5.2f(%2.1f%%)"
	#define STRMSG_S_F2NOTIFY_0001 "monster splash %d: %s에게 %5.2f"
	#define STRMSG_S_F2NOTIFY_0002 "1형"
	#define STRMSG_S_F2NOTIFY_0003 "2형"
	#define STRMSG_S_F2NOTIFY_0004 "몬1(%s)"
	#define STRMSG_S_F2NOTIFY_0005 "몬2(%s)"
	#define STRMSG_S_F2NOTIFY_0006 "잘못된 무기 종류! 관리자에게 문의!"
	#define STRMSG_S_F2NOTIFY_0007 "1-1형: %4.1f vs %4.1f -> 공격확률 %2.2f%% 감소"
	#define STRMSG_S_F2NOTIFY_0008 "%s->%s 실패, 확률(%d>%5.2f)"
	#define STRMSG_S_F2NOTIFY_0009 "%s->%s 실패, 확률(%d>%5.2f) -%5.2f"
	#define STRMSG_S_F2NOTIFY_0010 "%s->%s 실패, 확률(%d>%5.2f)"
	#define STRMSG_S_F2NOTIFY_0011 "1-2형: %4.1f vs %4.1f -> 데미지%2.2f%%감소(%4.1f->%4.1f)"
	#define STRMSG_S_F2NOTIFY_0012 "%s->%s에게 %5.2f(%5.2f-%d/255) 줌(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0013 "%s->%s로부터 %5.2f(%5.2f-%d/255) 받음(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0014 "%s->%s(%d, HP:%5.2f)에게 %5.2f(%5.2f-%d/255) 줌(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0015 "%s->%s로부터 %5.2f(%5.2f-%d/255) 받음(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0016 "더미 실패: 확률 부족 > %d"
	#define STRMSG_S_F2NOTIFY_0017 "남은 더미[%#08x]: %5.2f(%5.2f-%5.2f)"
	#define STRMSG_S_F2NOTIFY_0018 "총알정보가 없습니다. 관리자에게 문의하세요."
	#define STRMSG_S_F2NOTIFY_0019 "총알의 구경 정보가 존재하지 않습니다. %s %d"
	#define STRMSG_S_F2NOTIFY_0020 "총알의 정보입니다. %s %d"
	#define STRMSG_S_F2NOTIFY_0021 "이미 이벤트 처리중이므로 워프가 불가능합니다"
	#define STRMSG_S_F2NOTIFY_0022 "죽었으므로 워프가 불가능합니다"
	#define STRMSG_S_F2NOTIFY_0023 "편대전 중이므로 워프가 불가능합니다"
	#define STRMSG_S_F2NOTIFY_0024 "맵 제작 오류! 관리자에게 문의!"
	#define STRMSG_S_F2NOTIFY_0025 "맵 제작 오류! 관리자에게 문의! %d에 %d번 위프 타겟 인덱스가 없음!\r\n"
	#define STRMSG_S_F2NOTIFY_0026 "파티전이 끝나지 않았으므로 워프할 수 없습니다"
	#define STRMSG_S_F2NOTIFY_0027 "  WARP(%04d) 처리 불가 at Process_FC_CHARACTER_DEAD_GAMESTART(), %s\r\n"
	#define STRMSG_S_F2NOTIFY_0028 "  1 -> 캐릭터 %10s에게 데미지 %5.2f을 받았음"
	#define STRMSG_S_F2NOTIFY_0029 "1 -> 캐릭터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0030 "1 -> 캐릭터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0031 "1 -> 캐릭터 %10s에게 더미 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0032 "  2 -> 캐릭터 %10s에게 데미지 %5.2f을 받았음"
	#define STRMSG_S_F2NOTIFY_0033 "2 -> 캐릭터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0034 "2 -> 몬스터 %3d에게 데미지 %5.2f(%d)을 주었음"
	#define STRMSG_S_F2NOTIFY_0035 "2 -> 캐릭터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0036 "2 -> 캐릭터 %10s에게 더미 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0037 "  2 -> 몬스터 %3d에게 데미지 %5.2f을 받았음"
	#define STRMSG_S_F2NOTIFY_0038 "  MINE -> 캐릭터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0039 "  MINE -> 캐릭터 %10s에게 더미 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0040 "  MINE -> 몬스터 %10s에게 데미지 %5.2f을 주었음"
	#define STRMSG_S_F2NOTIFY_0041 "스탯 초기화 성공하였습니다."
	#define STRMSG_S_F2NOTIFY_0042 "죽었을 때는 사용할 수 없습니다"
	#define STRMSG_S_F2NOTIFY_0043 "ENCHANT_INFO 없음"
	#define STRMSG_S_F2NOTIFY_0044 "인챈트 재련에 성공하였습니다"
	#define STRMSG_S_F2NOTIFY_0045 "인챈트 재련에 실패하였습니다"
	#define STRMSG_S_F2NOTIFY_0046 "여단에 소속되지 않았습니다."
	#define STRMSG_S_F2NOTIFY_0047 "이미 요청 대기중입니다."
	#define STRMSG_S_F2NOTIFY_0048 "잠시 후 재시도 바랍니다."
	#define STRMSG_S_F2NOTIFY_0049 "자신 혹은 상대가 여단장이 아닙니다."
	#define STRMSG_S_F2NOTIFY_0050 "이미 여단전투 중입니다."
	#define STRMSG_S_F2NOTIFY_0051 "두 여단장이 같은 맵에 존재하지 않습니다."
	#define STRMSG_S_F2NOTIFY_0052 "모든 파티원들이 같은 맵에 있지 않습니다"
	#define STRMSG_S_F2NOTIFY_0053 "모든 파티원들이 같은 맵에 있지 않습니다"
	#define STRMSG_S_F2NOTIFY_0054 "길드에 소속되지 않았습니다."
	#define STRMSG_S_F2NOTIFY_0055 "신청자 피신청자가 다릅니다."
	#define STRMSG_S_F2NOTIFY_0056 "자신 혹은 상대가 여단장이 아닙니다."
	#define STRMSG_S_F2NOTIFY_0057 "두 여단장이 같은 맵에 존재하지 않습니다."
	#define STRMSG_S_F2NOTIFY_0058 "길드에 소속되지 않았습니다."
	#define STRMSG_S_F2NOTIFY_0059 "신청자 피신청자가 다릅니다."
	#define STRMSG_S_F2NOTIFY_0060 "자신 혹은 상대가 여단장이 아닙니다."
	#define STRMSG_S_F2NOTIFY_0061 "도시점령전 방어에 성공하였습니다"
	#define STRMSG_S_F2NOTIFY_0062 "미션 설정 에러입니다. 관리지에게 문의하세요."
	#define STRMSG_S_F2NOTIFY_0063 "이 위치에는 해당 아이템(%s)을 장착할 수 없습니다"
	#define STRMSG_S_F2NOTIFY_0064 "엔진은 비워둘 수 없습니다."
	#define STRMSG_S_F2NOTIFY_0065 "아이템 이동: (%I64d, %d) -> (%I64d, %d)"
	#define STRMSG_S_F2NOTIFY_0066 "잘못된 아이템 이동: (%I64d, %d) -> (%I64d, %d)"
	#define STRMSG_S_F2NOTIFY_0067 "  EVENT(%d) 처리 불가 at Process_FC_EVENT_REQUEST_WARP(), %s\r\n"
	#define STRMSG_S_F2NOTIFY_0068 "맵 이벤트 정보 이상!!! 관리자에게 문의!!!"
	#define STRMSG_S_F2NOTIFY_0069 "해당 워프 타겟이 없음"
	#define STRMSG_S_F2NOTIFY_0070 "아이템 구입 에러입니다. 관리자에게 문의해주세요."
	#define STRMSG_S_F2NOTIFY_0071 "구입하려는 스킬 레벨이 기존 스킬의 레벨보다 낮거나 같습니다."
	#define STRMSG_S_F2NOTIFY_0072 "스킬 구매 필요 아이템 에러입니다."
	#define STRMSG_S_F2NOTIFY_0073 "아이템 팔기 에러입니다. 관리자에게 문의해주세요."
	#define STRMSG_S_F2NOTIFY_0074 "아이템 구입 에러입니다. 관리자에게 문의해주세요."
	#define STRMSG_S_F2NOTIFY_0075 "\'유료 아이템\'을 구매하였습니다."
	#define STRMSG_S_F2NOTIFY_0076 "  구매목록 : \'%s(%d개)\'"
	#define STRMSG_S_F2NOTIFY_0077 "파티에 이미 가입하였습니다."
	#define STRMSG_S_F2NOTIFY_0078 "당신이 파티장인데 어떻게 가입합니까?"
	#define STRMSG_S_F2NOTIFY_0079 "편대전이 진행중이므로 가입할 수 없습니다."
	#define STRMSG_S_F2NOTIFY_0080 "모든 파티원 정보 가져오기는 지원되지 않음!"
	#define STRMSG_S_F2NOTIFY_0081 "맵 이벤트 정보 이상!!! 관리자에게 문의!!!"
	#define STRMSG_S_F2NOTIFY_0082 "거래를 할 수 없는 계정입니다"
	#define STRMSG_S_F2NOTIFY_0083 "%s님의 적재량 초과입니다."
	#define STRMSG_S_F2NOTIFY_0084 "이동 가능 좌표: (5, 5) -> (%d, %d)"
	#define STRMSG_S_F2NOTIFY_0085 "사용자: "
	#define STRMSG_S_F2NOTIFY_0086 "최대 20명까지만 볼 수 있습니다."
	#define STRMSG_S_F2NOTIFY_0087 "해당 사용자(%s)가 없습니다"
	#define STRMSG_S_F2NOTIFY_0088 "파티에 소속되지 않았음"
	#define STRMSG_S_F2NOTIFY_0089 "현재 시간: %d:%d, 아툼 시간: %d:%d"
	#define STRMSG_S_F2NOTIFY_0090 "변경 시간: %d:%d, 아툼 시간: %d:%d"
	#define STRMSG_S_F2NOTIFY_0091 "NPC 서버와 연결이 되지 않았습니다"
	#define STRMSG_S_F2NOTIFY_0092 "해당 아이템(%d)이 존재하지 않습니다"
	#define STRMSG_S_F2NOTIFY_0093 "Countable Item이 아닌 경우 10개로 제한됩니다"
	#define STRMSG_S_F2NOTIFY_0094 "맵 \'%s\'의 전체 접속자: %d명"
	#define STRMSG_S_F2NOTIFY_0095 "맵 %s %s 동접: %d명 - '%s'(*)"
	#define STRMSG_S_F2NOTIFY_0096 "맵 %s %s 동접: %d명 - '%s'"
	#define STRMSG_S_F2NOTIFY_0097 "현재 맵채널: %s, %d(%d)"
	#define STRMSG_S_F2NOTIFY_0098 "총알정보가 없습니다. 관리자에게 문의하세요."
	#define STRMSG_S_F2NOTIFY_0099 "총알의 구경 정보가 존재하지 않습니다. %s %d"
	#define STRMSG_S_F2NOTIFY_0100 "총알정보가 없습니다. 관리자에게 문의하세요."
	#define STRMSG_S_F2NOTIFY_0101 "총알의 구경 정보가 존재하지 않습니다. %s %d"
	#define STRMSG_S_F2NOTIFY_0102 "해당 사용자(%s)가 없습니다"
	#define STRMSG_S_F2NOTIFY_0103 "해당 사용자(%s)가 죽은 상태입니다"
	#define STRMSG_S_F2NOTIFY_0104 "해당 사용자(%s)가 없습니다"
	#define STRMSG_S_F2NOTIFY_0105 "일반 계정으로 설정되었습니다"
	#define STRMSG_S_F2NOTIFY_0106 "일반 계정이 해제되었습니다"
	#define STRMSG_S_F2NOTIFY_0107 "무적(데미지를 입지 않는) 유닛으로 설정되었습니다"
	#define STRMSG_S_F2NOTIFY_0108 "무적(데미지를 입지 않는) 유닛 설정이 해제되었습니다"
	#define STRMSG_S_F2NOTIFY_0109 "무기가 %5.0f%% 사기성을 띄게 되었습니다"
	#define STRMSG_S_F2NOTIFY_0110 "아이템 재장전시 초기화됩니다"
	#define STRMSG_S_F2NOTIFY_0111 "아이템 재장전시 초기화됩니다"
	#define STRMSG_S_F2NOTIFY_0112 "해당 사용자(%s)가 없습니다"
	#define STRMSG_S_F2NOTIFY_0113 "투명 기능이 해제되었습니다"
	#define STRMSG_S_F2NOTIFY_0114 "투명 기능이 활성화되었습니다"
	#define STRMSG_S_F2NOTIFY_0115 "%s 이벤트가 진행 중이 아닙니다"
	#define STRMSG_S_F2NOTIFY_0116 "%s 이벤트가 시작됨(배율:%4.2f, 이벤트시간:%3d분)"
	#define STRMSG_S_F2NOTIFY_0117 "일반프리미엄 계정으로 설정되었습니다"
	#define STRMSG_S_F2NOTIFY_0118 "프리미엄 계정 설정에 실패하였습니다"
	#define STRMSG_S_F2NOTIFY_0119 "슈퍼프리미엄 계정으로 설정되었습니다"
	#define STRMSG_S_F2NOTIFY_0120 "도시점령전 맵이 아닙니다"
	#define STRMSG_S_F2NOTIFY_0121 "도시점령전을 시작 할 수 없습니다"
	#define STRMSG_S_F2NOTIFY_0122 "도시점령전을 종료 할 수 없습니다"
	#define STRMSG_S_F2NOTIFY_0123 "스텔스 상태가 초기화 되었습니다"
	#define STRMSG_S_F2NOTIFY_0124 "스텔스 상태가 되었습니다"
	#define STRMSG_S_F2NOTIFY_0125 "맵이 유효하지 않습니다."
	#define STRMSG_S_F2NOTIFY_0126 "해피아워이벤트 설정됨 [진행시간:%4d분)]"
	#define STRMSG_S_F2NOTIFY_0127 "해피아워이벤트 종료됨"
	#define STRMSG_S_F2NOTIFY_0128 "  1 -> 몬스터 %3d에게 데미지 %5.2f을 받았음"
	#define STRMSG_S_F2NOTIFY_0129 "  1형 -> 몬스터 %3d에게 데미지 %5.2f을 받았음(더미)"
	#define STRMSG_S_F2NOTIFY_0130 "%s 권한으로 접속하였습니다"
	#define STRMSG_S_F2NOTIFY_0131 "  종료 루틴을 처리하지 않음 %s: CS(%d), DBStore(%d)\r\n"
	#define STRMSG_S_F2NOTIFY_0132 "  ERROR@CharacterGameEndRoutine(): 파티원 제거 실패! %s\r\n"
// 2005-11-24 by cmkwon, 
//	#define STRMSG_S_F2NOTIFY_0133 "Prefix \'%s\' 성공: %d <= %d <= %d\r\n"
//	#define STRMSG_S_F2NOTIFY_0134 "Suffix \'%s\' 성공: %d <= %d <= %d\r\n"
	#define STRMSG_S_F2NOTIFY_0135 "%s: 중지됨!"
	#define STRMSG_S_F2NOTIFY_0136 "FATAL ERROR: 관리자에 문의하세요! 상점 변수 할당 에러!"
	#define STRMSG_S_F2NOTIFY_0137 "%s가(이) 삭제되었습니다."
	#define STRMSG_S_F2NOTIFY_0138 "현재 계신 채널은 비활성화 되어 몬스터 및 기타 기능이"
	#define STRMSG_S_F2NOTIFY_0139 "정지됩니다. 다른 채널을 이용하여 주시기 바랍니다."
	#define STRMSG_S_F2NOTIFY_0140 "  Warp Obj.[%s,%2d개]: %04d[%1s%4d]"
	#define STRMSG_S_F2NOTIFY_0141 "타이머 동작 %s\r\n"
	#define STRMSG_S_F2NOTIFY_0142 "HP 자동 회복 중지합니다"
	#define STRMSG_S_F2NOTIFY_0143 "착륙 가능 지역이 아니므로 HP 자동 회복 중지합니다"
	#define STRMSG_S_F2NOTIFY_0144 "응급에 의한 HP 5.2f 회복"
	#define STRMSG_S_F2NOTIFY_0145 "Gradual HP UP 중지됨(남은 시간: %d)"
	#define STRMSG_S_F2NOTIFY_0146 "Gradual DP UP 중지됨(남은 시간: %d)"
	#define STRMSG_S_F2NOTIFY_0147 "Gradual EP UP 중지됨(남은 시간: %d)"
	#define STRMSG_S_F2NOTIFY_0148 "%s 이벤트가 종료 되었습니다."
	#define STRMSG_S_F2NOTIFY_0149 "%s 이벤트 진행 중 (배율:%4.2f, 남은시간:%3d분)"
	#define STRMSG_S_F2NOTIFY_0150 "명령어가 수행되지 않았습니다"


	// 5-5 Field - NOTIFY Error
	#define STRERR_S_F2NOTIFY_0000 "	경험치 분배 오류(%s, %s(%d)): fTotalDamage(%d) < 0.0f or Empty Vector: %d, Distance(%5.1f)\n"
	#define STRERR_S_F2NOTIFY_0001 "  유효하지 않은 Game Start 메세지 오류 ClientState[%d]\r\n"
	#define STRERR_S_F2NOTIFY_0002 "  유효하지 않은 Game Start 메세지 오류 ClientState[%d]\r\n"
	#define STRERR_S_F2NOTIFY_0003 "  WARP(%04d) 처리 불가 at ProcessQuestResult(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0004 "맵 이벤트 정보 이상!!! 관리자에게 문의!!! Current(%s, %s, %04d), Target(%04d, %d)\r\n"
	#define STRERR_S_F2NOTIFY_0005 "  EVENT(%d) 처리 불가 at T_FC_PARTY_REQUEST_PARTY_WARP(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0006 "맵 이벤트 정보 이상!!! 관리자에게 문의!!! Current(%s, %s, %04d), Target(%04d, %d)\r\n"
	#define STRERR_S_F2NOTIFY_0007 "  WARP 처리 불가 at T_FC_PARTY_REQUEST_PARTY_WARP(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0008 "  EVENT(%d) 처리 불가(파티장) at Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0009 "  WARP(%04d) 처리 불가 at Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0010 "  EVENT(%d) 처리 불가 at Process_FC_EVENT_REQUEST_WARP(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0011 "  WARP 처리 불가 at T_FC_PARTY_REQUEST_PARTY_WARP(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0012 "파티 워프 실패: %s -> 거리: %5.2f, bodycon: %d, %d"
	#define STRERR_S_F2NOTIFY_0013 "  WARP(%04d) 처리 불가 at Process_FC_CHARACTER_DEAD_GAMESTART(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0014 "  %s -> 중복된 OK 버튼 클릭!!!\r\n"
	#define STRERR_S_F2NOTIFY_0015 "퀘스트가 로딩되지 않았습니다.\r\n"
	#define STRERR_S_F2NOTIFY_0016 "  WARP(%s) 처리 불가 at HandleAdminCommands(), /이동, %s\r\n"
	#define STRERR_S_F2NOTIFY_0017 "  WARP(%s) 처리 불가 at HandleAdminCommands(), /보내기, %s\r\n"
	#define STRERR_S_F2NOTIFY_0018 "  FATAL ERROR: 멤버가 속하지 않은 필드 서버에는 이 메세지가 오면 안 된다. check it out\r\n"
	#define STRERR_S_F2NOTIFY_0019 "  WARP(%04d) 처리 불가 at T_FI_ADMIN_CALL_CHARACTER, %s\r\n"
	#define STRERR_S_F2NOTIFY_0020 "  WARP(%s) 처리 불가 at T_FI_ADMIN_MOVETO_CHARACTER, %s\r\n"

	// 5-6 Field - Event
	#define STRMSG_S_F2EVENTTYPE_0000 "경험치"
	#define STRMSG_S_F2EVENTTYPE_0001 "스피(SPI)"
	#define STRMSG_S_F2EVENTTYPE_0002 "경험치복구"
	#define STRMSG_S_F2EVENTTYPE_0003 "아이템드랍"
	#define STRMSG_S_F2EVENTTYPE_0004 "레어아이템드랍"
	#define STRMSG_S_F2EVENTTYPE_0005 "이벤트모름"
// 5_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 6 - IMServer
	// 6-1 IM<->Pre
	#define STRMSG_S_I2PRECONNECT_0000 "Pre Server에 접속 하였습니다.\r\n"
	#define STRMSG_S_I2PRECONNECT_0001 "Pre Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"

	// 6-2 IM Notify
	#define STRMSG_S_I2NOTIFY_0000 "\'%s\'는(은) 이미 존재하는 여단 이름입니다"
	#define STRMSG_S_I2NOTIFY_0001 "\'%s\'는(은) 이미 여단에 가입되어 있습니다."
	#define STRMSG_S_I2NOTIFY_0002 "\'%s\'는(은) 이미 존재하는 여단 이름입니다"
	#define STRMSG_S_I2NOTIFY_0003 "여단 로딩 실패 - 관리자에게 문의해주세요!"
	#define STRMSG_S_I2NOTIFY_0004 "친구 '%s'님이 접속하였습니다"
	#define STRMSG_S_I2NOTIFY_0005 "이전의 파티를 검사합니다"
	#define STRMSG_S_I2NOTIFY_0006 "기존의 파티가 존재하지 않습니다"
	#define STRMSG_S_I2NOTIFY_0007 "파티에 재가입되었습니다"
	#define STRMSG_S_I2NOTIFY_0008 "이미 가입한 파티가 있습니다."
	#define STRMSG_S_I2NOTIFY_0009 "  Fatal Error: 심각한 파티 에러! at T_IC_PARTY_GET_MEMBER"
	#define STRMSG_S_I2NOTIFY_0010 "파티원의 상태가 잘 못 되었음"
	#define STRMSG_S_I2NOTIFY_0011 "파티원이 이미 길드에 가입하였음"
	#define STRMSG_S_I2NOTIFY_0012 "대상이 이미 길드에 가입하였습니다"
	#define STRMSG_S_I2NOTIFY_0013 "자기 자신은 초대하지 못합니다"
	#define STRMSG_S_I2NOTIFY_0014 "길드 가입이 허용되지 않는 상태입니다"
	#define STRMSG_S_I2NOTIFY_0015 "가입 가능한 여단원 수를 초과하였습니다"
	#define STRMSG_S_I2NOTIFY_0016 "길드 가입이 허용되지 않는 상태입니다"
	#define STRMSG_S_I2NOTIFY_0017 "여단장은 탈퇴할 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0018 "길드전 중이므로 길드에서 탈퇴할 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0019 "길드전 중이므로 길드원을 추방할 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0020 "여단장은 추방될 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0021 "길드전 중이므로 길드를 해산할 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0022 "같은 이름으로는 변경하지 못합니다"
	#define STRMSG_S_I2NOTIFY_0023 "길드 이름을 변경할 수 없는 상태입니다"
	#define STRMSG_S_I2NOTIFY_0024 "길드 마크를 변경할 수 없는 상태입니다"
	#define STRMSG_S_I2NOTIFY_0025 "계급을 변경할 수 없는 상태입니다"
	#define STRMSG_S_I2NOTIFY_0026 "계급이 중복되었습니다"
	#define STRMSG_S_I2NOTIFY_0027 "여단장으로의 계급 변경은 불가능합니다."
	#define STRMSG_S_I2NOTIFY_0028 "여단장의 계급 변경은 불가능합니다."
	#define STRMSG_S_I2NOTIFY_0029 "전체 사용자 수: %d명 (%d명까지 출력)"
	#define STRMSG_S_I2NOTIFY_0030 "운영자 메세지 리스트에 추가되었습니다"
	#define STRMSG_S_I2NOTIFY_0031 "운영자 메세지 리스트에서 제거되었습니다"
	#define STRMSG_S_I2NOTIFY_0032 "서버 IP: %s"
	#define STRMSG_S_I2NOTIFY_0033 "서버 그룹 \'%s\'의 전체 접속자: %d명"
	#define STRMSG_S_I2NOTIFY_0034 "정말 서버(%s)를 종료하시겠어요? 숫자: %d"
	#define STRMSG_S_I2NOTIFY_0035 "\'%s\'는 게임을 하고 있지 않습니다 "
	#define STRMSG_S_I2NOTIFY_0036 "전체 사용자 수: %d명 (%d명까지 출력)"
	#define STRMSG_S_I2NOTIFY_0037 "/보내기 %s %s"
	#define STRMSG_S_I2NOTIFY_0038 "해당 여단이 없습니다."
	#define STRMSG_S_I2NOTIFY_0039 "귓속말 차단이 해제 되었습니다!"
	#define STRMSG_S_I2NOTIFY_0040 "귓속말이 차단 되었습니다!"
	#define STRMSG_S_I2NOTIFY_0041 "길드에 가입하지 않았습니다"
	#define STRMSG_S_I2NOTIFY_0042 "길드(%d)가 유효하지 않습니다"
	#define STRMSG_S_I2NOTIFY_0043 "해당 날씨(%s)가 존재하지 않습니다"
	#define STRMSG_S_I2NOTIFY_0044 "Field Server가 유효하지 않습니다"
	#define STRMSG_S_I2NOTIFY_0045 "해당 맵(%s)이 존재하지 않습니다"
	#define STRMSG_S_I2NOTIFY_0046 "당신은 %d분 동안 채팅금지 상태입니다!!"
	#define STRMSG_S_I2NOTIFY_0047 "채팅금지 설정 : '%10s', %3d분"
	#define STRMSG_S_I2NOTIFY_0048 "채팅금지 상태가 해제 되었습니다"
	#define STRMSG_S_I2NOTIFY_0049 "채팅금지 해제 : '%10s'"
	#define STRMSG_S_I2NOTIFY_0050 "%s(계정: %s, 맵: %d(%d), 레벨: %d) PLAYING"
	#define STRMSG_S_I2NOTIFY_0051 "자기 자신을 부를 수 없습니다"
	#define STRMSG_S_I2NOTIFY_0052 "해당 길드가 없습니다."

	#define STRMSG_S_IMSERVER_050607_0001	"해당 맵이 없습니다."
// 6_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 7 - NPCServer
	// 7-1 NPC<->Field
	#define STRMSG_S_N2FIELDCONNECT_0000 "Field Server에 접속 하였습니다.\n"
	#define STRMSG_S_N2FIELDCONNECT_0001 "Field Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"

	// 7-2 IM Notify
	#define STRMSG_S_N2NOTIFY_0000 "							몬스터가 오브젝트와 충돌 체크 없음\r\n"
	#define STRMSG_S_N2NOTIFY_0001 "도시점령전 몬스터(%10s) 소환됨\r\n\r\n"

	#define STRMSG_S_N2TESTMONNAME_0000 "철민호"
// 7_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 8 - PreServer
	// 8-1 Pre Notify
	#define STRMSG_S_P2PRENOTIFY_0000 "%s 계정(%s)으로 접속하였습니다. IP: %s\r\n"
	#define STRMSG_S_P2PRENOTIFY_0001 "성공"
	#define STRMSG_S_P2PRENOTIFY_0002 "실패"
	#define STRMSG_S_P2PRENOTIFY_0003 "[ERROR] 계정정보추가 오류, AccountName(%s)  privateIP(%15s)\n"


	#define STRMSG_SCAT_051115_0001		"안녕하세요. 에이스온라인 운영자입니다."
	#define STRMSG_SCAT_051115_0002		"금일 6시부터 10시까지 에이스온라인 정기 점검을 실시합니다."
	#define STRMSG_SCAT_051115_0003		"안전한 장소로 이동 후 접속을 종료해 주시길 바랍니다."
	#define STRMSG_SCAT_051115_0004		"먼저 NPC 서버부터 종료 하겠습니다."
	#define STRMSG_SCAT_051115_0005		"지금부터 에이스온라인 정기 점검을 시작하겠습니다."
	#define STRMSG_SCAT_051115_0006		"에이스온라인과 함께 즐거운 하루 보내시길 바랍니다."
	#define STRMSG_SCAT_051115_0007		"잠시 후 긴급 점검을 위한 서버다운이 있을 예정입니다."
	#define STRMSG_SCAT_051115_0008		"점검 시간은 60분입니다."


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	// 2006-05-09 by cmkwon
	#define STRMSG_060509_0000			"투명상태: 유저가 볼 수 없음, 모든 무기 사용 불가"
	#define STRMSG_060509_0001			"무적상태: 데미지를 받지 않음"
	#define STRMSG_060509_0002			"스텔스모드: 선공 몬스터로부터 먼저 공격을 받지 않음"

	// 2006-05-26 by cmkwon
	#define	STRMSG_060526_0000			"Your request has been sent to a GM. One of our GMs will contact you momentarily."
	#define	STRMSG_060526_0001			"자동 업데이트를 실행할 수 없습니다.\r\n\r\n홈페이지(%s) 다운로드 페이지에서 수동패치를 받아 설치한 후 다시 접속하시기 바랍니다.\r\n\r\n    Error: %s"

	// 2006-08-24 by cmkwon
	#define STRMSG_060824_0000			"현재 입력하신 아이디가 등록되어 있지 않거나,\r\n 아이디 또는 패스워드를 잘못 입력 하셨습니다."
	
	// 2006-09-27 by cmkwon
	#define STRMSG_060927_0000			"현재 서비스 점검 중입니다. 자세한 사항은 홈페이지를 참고하세요."

	// 2006-10-11 by cmkwon
	#define STRERR_061011_0000			"클라이언트 버전이 맞지 않습니다.\r\n  홈페이지에서 수동패치 파일을 받아 다시 설치하거나, 클라이언트를 다시 설치해 주세요."

	// 2006-11-07 by cmkwon
	#define STRMSG_061107_0000			"%s에 의해 격추당했습니다."			  

	// 2006-11-07 by cmkwon
	#define STRMSG_070410_0000			"대회서버 DB(atum2_db_20) 초기화는 대회서버군을 종료 후에 해야합니다."
	#define STRMSG_070410_0001			"정말로 대회서버 DB(atum2_db_20)를 초기화 하시겠습니까? [인증숫자:%d]"
	#define STRMSG_070410_0002			"대회서버 DB(atum2_db_20)가 초기화 되었습니다."
	#define STRMSG_070410_0003			"대회서버 DB(atum2_db_20) 초기화 실패 !!"
	#define STRMSG_070410_0004			"대회서버 DB(atum2_db_20)로 '%s'의 정보 복사 완료"
	#define STRMSG_070410_0005			"대회서버 DB(atum2_db_20)로 '%s'의 정보 복사 실패 - 캐릭터가 존재하지 안음 !!"
	#define STRMSG_070410_0006			"대회서버 DB(atum2_db_20)로 '%s'의 정보 복사 실패 - 대회서버에 해당 계정의 캐릭터가 있음 !!"
	#define STRMSG_070410_0007			"대회서버 DB(atum2_db_20)로 '%s'의 정보 복사 실패 - DB 추가 오류임 !!"
	#define STRMSG_070410_0008			"대회서버 DB(atum2_db_20)로 '%s'의 정보 복사 실패 - 알수 없음(%d)!!"

///////////////////////////////////////////////////////////////////////////////
// 2007-05-07 by cmkwon, 해상도 문자열 
	// 2007-07-24 by cmkwon, 런처에서 800*600 해상도 삭제 - 콤보박스 스트링 필요 없음
	//#define STRMSG_WINDOW_DEGREE_800x600_LOW			"800x600 (low)"
	//#define STRMSG_WINDOW_DEGREE_800x600_MEDIUM			"800x600 (medium)"
	//#define STRMSG_WINDOW_DEGREE_800x600_HIGH			"800x600 (high)"
#define STRMSG_WINDOW_DEGREE_1024x768_LOW			"1024x768 (low)"
#define STRMSG_WINDOW_DEGREE_1024x768_MEDIUM		"1024x768 (medium)"
#define STRMSG_WINDOW_DEGREE_1024x768_HIGH			"1024x768 (high)"
#define STRMSG_WINDOW_DEGREE_W1280x800_LOW			"1280x800 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x800_MEDIUM		"1280x800 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x800_HIGH			"1280x800 (high-wide)"
#define STRMSG_WINDOW_DEGREE_1280x960_LOW			"1280x960 (low)"
#define STRMSG_WINDOW_DEGREE_1280x960_MEDIUM		"1280x960 (medium)"
#define STRMSG_WINDOW_DEGREE_1280x960_HIGH			"1280x960 (high)"
#define STRMSG_WINDOW_DEGREE_1280x1024_LOW			"1280x1024 (low)"
#define STRMSG_WINDOW_DEGREE_1280x1024_MEDIUM		"1280x1024 (medium)"
#define STRMSG_WINDOW_DEGREE_1280x1024_HIGH			"1280x1024 (high)"
#define STRMSG_WINDOW_DEGREE_W1600x900_LOW			"1600x900 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1600x900_MEDIUM		"1600x900 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1600x900_HIGH			"1600x900 (high-wide)"
#define STRMSG_WINDOW_DEGREE_1600x1200_LOW			"1600x1200 (low)"
#define STRMSG_WINDOW_DEGREE_1600x1200_MEDIUM		"1600x1200 (medium)"
#define STRMSG_WINDOW_DEGREE_1600x1200_HIGH			"1600x1200 (high)"

// 2007-06-15 by dhjin, 관전 관련 스트링
#define STRMSG_070615_0000				"투명 상태가 아니어서 관전 시작을 할 수 없습니다."
#define STRMSG_070615_0001				"관전시작을 할 수 없습니다."
#define STRMSG_070620_0000				"일반 세력은 관전 시작을 할 수 없습니다."

// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
#define STRMSG_S_F2EVENTTYPE_0006		"전쟁포인트"

// 2007-06-28 by cmkwon, 중국 방심취관련(게임 시간 알림 구현) - 스트링 추가
#define STRMSG_070628_0000				"당신의 연속 게임시간은 이미 %d시간이 되었습니다."
#define STRMSG_070628_0001				"당신의 연속 게임시간은 이미 %d시간이 되었습니다. 게임종료하시고 휴식도 하시고 적합한 운동을 하세요."
#define STRMSG_070628_0002				"당신은 이미 과도한 게임시간에 들어섰습니다. 당신의 게임수익은 50%%으로 줄어듭니다. 당신의 건강을 위하여 게임 종료하시고 적당한 운동을 하시고 합리하게 학습생활을 안배하세요"
#define STRMSG_070628_0003				"당신은 이미 불건전한 게임시간에 들어섰습니다. 당신의 건강을 위하여 즉시로 게임 종료하세요. 그렇지 않으면 당신의 신체에 해로울 뿐더러 당신의 게임수익은 0%%으로 줄어듭니다. 게임 종료 후 5시간후에 다시 정상적 수익으로 회복됩니다."

///////////////////////////////////////////////////////////////////////////////
// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - 추가된 스트링
#define STRMSG_070711_0000				"\'%s\'님은 아레나 이용 금지 상태가 아닙니다."
#define STRMSG_070711_0001				"\'%s\'님은 아레나 이용 금지 상태 입니다.(남은 시간:%d분)"
#define STRMSG_070711_0002				"당신은 %d분 동안 아레나 이용 금지 상태입니다!!"
#define STRMSG_070711_0003				"당신은 아레나 이용 금지가 해제 되었습니다."
#define STRMSG_070711_0004				"\'%s\'님의 아레나 이용 금지가 해제 되었습니다."

///////////////////////////////////////////////////////////////////////////////
// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 스트링 추가
#define STRMSG_WINDOW_DEGREE_W1280x720_LOW			"1280x720 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x720_MEDIUM		"1280x720 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x720_HIGH			"1280x720 (high-wide)"

// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 스트링 추가
#define STRMSG_070830_0001				"세력선택 후 사용 가능한 명령어 입니다."
#define STRMSG_070830_0002				"해당 세력의 회의룸맵(%d)이 유효하지 않음"
#define STRMSG_070830_0003				"현재 회의룸에 입장 가능한 인원수 : %d명"
#define STRMSG_070830_0004				"'%s'님은 이미 회의룸 입장 권한이 있습니다."
#define STRMSG_070830_0005				"'%s'님은 회의룸 입장 권한이 없습니다."
#define STRMSG_070830_0006				"'%s'은(는) 유효한 캐릭터가 아닙니다."
#define STRMSG_070830_0007				"'%s'을 회의룸 입장 권한 리스트에 추가 할 수 없습니다.(최대 %d명)"
#define STRMSG_070830_0008				"'%s'님에게 회의룸 입장 권한이 부여되었습니다."
#define STRMSG_070830_0009				"회의룸 입장 권한이 부여되었습니다."
#define STRMSG_070830_0010				"회의룸 입장 권한이 해제되었습니다."
#define STRMSG_070830_0011				"'%s'"

// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
#define STRMSG_071115_0001				"\\y%s님께 %s아이템을 선물 하였습니다."
#define STRMSG_071115_0002				"선물 목록 : \'%s(%d개)\'"
#define STRMSG_071115_0003				"\\y%s님이 %s님께 %s아이템을 선물 하였습니다. 창고를 확인하세요."

// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
#define STRMSG_071120_0001				"진정 시스템 이용시간이 아닙니다. 고객 센터로 문의 하세요."
#define STRMSG_071120_0002				"진정 시스템이 활성화 되었습니다."
#define STRMSG_071120_0003				"진정 시스템이 종료 되었습니다."
#define STRMSG_071120_0004				"%s 부터 %s 까지 진정 시스템을 시작 합니다."

// 2007-11-28 by cmkwon, 통지시스템 구현 - 
#define STRMSG_071128_0001				"%s님이 %s아이템 %d개를 선물 하였습니다. 창고에서 확인하세요."

// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 
#define STRMSG_071228_0001				"해상도 설정이 유효하지 않습니다. 다시 확인하세요."

// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
#define STRMSG_080201_0001				"'%s'님 블럭 설정에 오류가 있습니다. ErrorCode(%d)"
#define STRMSG_080201_0002				"'%s'계정을 블럭 처리하였습니다.[Block End Date: %s]"
#define STRMSG_080201_0003				"'%s'계정이 블럭 리스트에 없습니다. ErrorCode(%d)"
#define STRMSG_080201_0004				"'%s'계정 블럭 해제 처리에 오류가 있습니다. ErrorCode(%d)"
#define STRMSG_080201_0005				"'%s'계정을 블럭 해제 처리 하였습니다."

// 2008-02-11 by cmkwon, 해상도 추가(1440x900) - 
#define STRMSG_WINDOW_DEGREE_1440x900_LOW			"1440x900 (low-wide)"
#define STRMSG_WINDOW_DEGREE_1440x900_MEDIUM		"1440x900 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_1440x900_HIGH			"1440x900 (high-wide)"

// 2007-12-27 by dhjin, 아레나 통합 - 아레나 서버 연결 관련 오류
#define STRMSG_S_MF2AFCONNECT_0000 "[Error] WndProc(), Can't connect to  ArenaServer[%15s:%4d] Reconnect\r\n"
#define STRMSG_S_MF2AFCONNECT_0001 "Arena Server에 접속 하였습니다.\r\n"
#define STRMSG_S_MF2AFCONNECT_0002 "Arena Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n"
#define STRMSG_S_MF2AFCONNECT_0003 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
#define STRMSG_S_MF2AFCONNECT_0004 "Unknown Error@WM_FIELD_PACKET_NOTIFY: %s(%#04x)\n"
#define STRMSG_ARENAEVENT_080310_0001		"\\y%d번 아레나 대기방에 이벤트 속성을 부여 했습니다.\r\n"
#define STRMSG_ARENAEVENT_080310_0002		"\\y%d번 아레나 방에 대한 이벤트 속성을 해제 하였습니다.\r\n"
#define STRMSG_ARENAEVENT_080310_0003		"\\y아레나 이벤트 속성 부여에 실패 하였습니다.\r\n"
#define STRMSG_080428_0001					"%s님을 격추 하였습니다."		// 2008-04-28 by dhjin, 아레나 통합 - 상대방 격추 시 스트링 추가, 아레나 맵에서만

// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
#define STRMSG_080430_0001					"선택하신 서버는 신규 캐릭터 생성이 제한된 서버 입니다."

// 2008-06-13 by dhjin, EP3 여단 수정 사항 - 
#define STRMSG_080613_0001					"%s여단에 가입 신청이 거부되었습니다."

// 2008-09-04 by cmkwon, don't need translation, 서버용 SystemLog 이므로 번역 필요 없음
#define STRMSG_080904_0001					 "[DB Error] No such DB query process(QP_xxx) function !! QueryType(%d:%s)\r\n"


// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 
#define STRMSG_081230_0001					"\\y%s캐릭터 %d분 채팅 제한을 시작 합니다.\\y"
#define STRMSG_081230_0002					"\\y지도자에 의해 %d분 동안 채팅 제한 상태가 되었습니다.\\y"
#define STRMSG_081230_0003					"\\y지도자 채팅 제한이 해제 되었습니다.\\y"

///////////////////////////////////////////////////////////////////////////////
// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 
// 2009-09-02 by cmkwon, Gameforge4D 게임 가드 동의창 WebPage로 처리 - STRMSG_090831_0001는 웹페이지로 처리
//#define STRMSG_090831_0001					"AirRivals is now protected from cheaters with a hackshield.\r\nPlease install it to help us to make AirRivals even safer.\r\nYou can only continue gameplay once you have installed the hackshield.\r\nPlease read the privacy policy< http://agb.gameforge.de/mmog/index.php?lang=en&art=datenschutz_mmog&special=airrivals&&f_text=b1daf2&f_text_hover=ffffff&f_text_h=061229&f_text_hr=061229&f_text_hrbg=061229&f_text_hrborder=9EBDE4&f_text_font=arial%2C+arial%2C+arial%2C+sans-serif&f_bg=000000 > to find out more about the hackshield's function."
#define STRMSG_090831_0002					"Install hackshield"
#define STRMSG_090831_0003					"cancel"

///////////////////////////////////////////////////////////////////////////////
// 2009-09-02 by cmkwon, Gameforge4D 게임 가드 동의창 WebPage로 처리 - 
#define STRMSG_090902_0001					"http://www.airrivals.net/launcher/hackshield.html"

///////////////////////////////////////////////////////////////////////////////
// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
#define STRMSG_WINDOW_DEGREE_1680x1050_LOW			"1680x1050 (low)"
#define STRMSG_WINDOW_DEGREE_1680x1050_MEDIUM		"1680x1050 (medium)"
#define STRMSG_WINDOW_DEGREE_1680x1050_HIGH			"1680x1050 (high)"
#define STRMSG_WINDOW_DEGREE_1920x1080_LOW			"1920x1080 (low)"
#define STRMSG_WINDOW_DEGREE_1920x1080_MEDIUM		"1920x1080 (medium)"
#define STRMSG_WINDOW_DEGREE_1920x1080_HIGH			"1920x1080 (high)"
#define STRMSG_WINDOW_DEGREE_1920x1200_LOW			"1920x1200 (low)"
#define STRMSG_WINDOW_DEGREE_1920x1200_MEDIUM		"1920x1200 (medium)"
#define STRMSG_WINDOW_DEGREE_1920x1200_HIGH			"1920x1200 (high)"

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버의 접속 허용 상황
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_NOT_REGISTER			"등록되지 않은 서버입니다. (사설 서버일 가능성이 있습니다)"
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_DB_ERROR				"DB 연동시 에러가 발생하였습니다"
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_OK						"서버 인증에 성공하였습니다."
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_BLOCKED				"해당 서버 IP가 등록되어 있으나 인증 거부 처리되었습니다."
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_SHUTDOWN				"서버에 강제 종료 명령을 전달했습니다."

///////////////////////////////////////////////////////////////////////////////
// 2012-03-30 by hskim, EP4 트리거 시스템 이벤트 알림
#define STRMSG_120330_0001					"%s이 애쉬레인에서 숨겨진 공간 워프 게이트를 발동 하였습니다."
#define STRMSG_120330_0002					"%s이 애쉬레인에서 몬스터 경험치 100%% 증가 해피아워 이벤트(1시간)를 발동 하였습니다."
#define STRMSG_120330_0003					"%s이 애쉬레인에서 아이템 드롭율 100%% 증가 해피아워 이벤트(1시간)를 발동 하였습니다."
#define STRMSG_120330_0004					"%s이 애쉬레인에서 아이템 드롭율 100%% / 몬스터 경험치 100%% 증가 / SPI량 100%% 증가 해피아워 이벤트(1시간)를 발동 하였습니다."
#define STRMSG_120330_0005					"%s님이 애쉬레인에서 무기 상점을 발동 하였습니다."
#define STRMSG_120330_0006					"%s님이 애쉬레인에서 하이퍼 카드 상점을 발동 하였습니다."
#define STRMSG_120330_0007					"%s님이 애쉬레인에서 키트 상점을 발동 하였습니다."
#define STRMSG_120330_0008					"%s님이 애쉬레인에서 대형 몬스터를 소환하셨습니다."
#define STRMSG_120330_0009					"크리스탈이 소멸하였습니다."

#endif // end_#ifndef _STRING_DEFINE_SERVER_H_
