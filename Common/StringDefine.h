#ifndef __STRING_DEFINE_H__
#define __STRING_DEFINE_H__


#define STR_Q_GUILD_CREATE							"창설할 비행여단의 이름을 입력하세요."
#define STR_GUILD_CREATE_OK							"비행여단 %s가 창설되었습니다."		// GuildName
#define STR_Q_GUILD_LEAVE							"비행여단에서 탈퇴하시겠습니까?"
#define STR_Q_GUILD_INVITE							"초대할 유닛을 입력하세요."
#define STR_Q_GUILD_DISBAND							"비행여단을 해체 하시겠습니까?"
#define STR_Q_GUILD_CANCEL							"비행여단 해체를 취소하시겠습니까?"
#define STR_ERR_TOO_LONG_GUILD_NAME_LENGTH			"비행여단의 이름이 너무 깁니다."
#define STR_Q_GUILD_INVITE_QUESTION					"'%s'님의 비행여단 '%s'에 가입하시겠습니까?" // CharacterName, GuildName
#define STR_GUILD_ACCEPT_INVITE_OK					"'%s'님이 비행여단에 가입하였습니다." // CharacterName
#define STR_GUILD_REJECT_INVITE_OK					"'%s'님이 비행여단 가입을 거절하였습니다." // CharacterName
#define STR_GUILD_BAN_MEMBER_OK						"'%s'님을 비행여단에서 추방하였습니다." // CharacterName
#define STR_GUILD_LEAVE_OK							"'%s'님이 비행여단에서 탈퇴하였습니다." // CharacterName
#define STR_GUILD_DISMEMBER_OK						"비행여단이 해체되었습니다."
#define STR_GUILD_CANCEL_DISMEMBER_OK				"비행여단가 해체가 취소되었습니다."
#define STR_Q_GUILD_BANMEMBER_QUESTION				"'%s'님을 비행여단에서 추방하시겠습니까?" // CharacterName
#define STR_GUILD_STATE_DISMEMBER_READY				"현재 소속된 비행여단이 해체 대기상태입니다."
#define STR_GUILD_GET_DISMEMBER_DATE_OK				"비행여단 해산 날짜는 %s입니다." // year, month, day, hour, minute, second
#define STR_GUILD_CHANGE_GUILD_NAME_OK				"비행여단 이름이 '%s'로 변경되었습니다." // guild name
#define STR_SET_GUILD_MARK							"게임디렉토리에 있는 여단 문장의 파일이름을 넣으세요.(BMP,크기:24 x 12)"
#define STR_SET_GUILD_MARK_OK						"비행여단의 문장이 변경되었습니다."
#define STR_GUILD_SET_RANK_OK						"'%s'의 여단계급이 '%s'로 변경되었습니다." // CharacterName, GuildRank
#define STR_ERROR_INVALID_FILENAME					"파일이름이 잘못되었습니다."
#define STR_ERROR_INVALID_IMAGE						"이미지의 형식이 잘못되었습니다."
#define STR_CANNOT_SET_GUILD_RANK_COMMANDER			"연대장으로 승격은 할 수 없습니다."

#define STR_Q_VOIP_OK								"%s님과 음성 교신을 하시겠습니까?"
#define STR_VOIP_ERR_NO_SUCH_CHARACTER				"잘못된 캐릭터 이름입니다."
#define STR_VOIP_ERR_NO_SUCH_ACCOUNTUNIQUENUMBER	"잘못된 유저입니다."
#define STR_VOIP_ERR_CANNOT_CALL					"상대방이 통화중입니다."
#define STR_VOIP_ERR_REJECT							"상대방이 통화를 거절하였습니다."
#define STR_VOIP_CALL_START							"통화가 시작되었습니다."
#define STR_VOIP_ERR_CALLING_STATE_FOR_PARTY		"통화중이라 편대간 통화를 할 수 없습니다."
#define STR_VOIP_ACCEPT_PARTY_CALL					"편대간 통화에 참여하시겠습니까?"
#define STR_VOIP_END_PARTY_CALL						"편대간 통화를 종료합니다."
#define STR_VOIP_ERR_CALLING_STATE_FOR_GUILD		"통화중이라 여단 통화를 할 수 없습니다."
#define STR_VOIP_ACCEPT_GUILD_CALL					"여단 통화에 참여하시겠습니까?"
#define STR_VOIP_END_GUILD_CALL						"여단 통화를 종료합니다."

#define STR_ERR_CANNOT_ITEM_USE_ENCHANT				"인챈트를 할 수 없는 구성입니다."
#define STR_ERR_CANNOT_ITEM_USE_MIX					"아이템 조합을 할 수 없는 아이템 구성입니다."
#define STR_ERR_FAIL_MIX_ITEMS_RESULT				"아이템 조합에 실패하여 아이템이 파괴 되었습니다."
#define STR_ERROR_RESOURCE							"리소스 파일 에러입니다. 프로그램을 종료하고, 다시 인스톨하세요."
#endif