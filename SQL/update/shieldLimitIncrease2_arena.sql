USE [atum2_db_1]
GO
/****** Object:  StoredProcedure [dbo].[atum_SaveCharacterCriticalData]    Script Date: 09/02/2017 15:08:23 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO


--------------------------------------------------------------
-- 9.30. update stored procedures
--------------------------------------------------------------

--!!!!
-- Name:
-- Desc:
--		2006-04-10 by cmkwon, Ã†Ã„Â¶Ã³Â¹ÃŒÃ…Ã ÃƒÃŸÂ°Â¡(@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon)
--		2006-11-15 by cmkwon, Ã€ÃŽÃ€Ãš 2Â°Â³ ÃƒÃŸÂ°Â¡
--						@i_GameContinueTimeInSecondOfToday	INT,			-- 2006-11-15 by cmkwon, Â¿Ã€Â´Ãƒ Ã‡ÃÂ·Ã§ Â°Ã”Ã€Ã“ ÃÃ¸Ã‡Ã  Â½ÃƒÂ°Â£
--						@i_LastGameEndDate					VARCHAR(30)		-- 2006-11-15 by cmkwon, Â¸Â¶ÃÃ¶Â¸Â· Â°Ã”Ã€Ã“ ÃÂ¾Â·Ã¡ Â³Â¯Ã‚Â¥Â½ÃƒÂ°Â£
--====
ALTER PROCEDURE [dbo].[atum_SaveCharacterCriticalData]
	@i_CharacterUID		INT,
	@i_Level			TINYINT,
	@i_CharacterMode	TINYINT,
	@i_Experience		FLOAT,
	@i_DownExperience	FLOAT,
	@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon
	@i_BodyCondition	BIGINT,
	@i_PetLevel			TINYINT,
	@i_PetExperience	FLOAT,
	@i_Position_X		FLOAT,
	@i_Position_Y		FLOAT,
	@i_Position_Z		FLOAT,
	@i_MapIndex			SMALLINT,
	@i_ChannelIndex		SMALLINT,
	@i_HP				INTEGER,
	@i_DP				INTEGER,
	@i_SP				SMALLINT,
	@i_EP				SMALLINT,
	@i_currentHP		FLOAT,
	@i_currentDP		FLOAT,
	@i_currentSP		SMALLINT,
	@i_currentEP		FLOAT,
	@i_totalPlayTime	BIGINT,
	@i_GameContinueTimeInSecondOfToday	INT,			-- 2006-11-15 by cmkwon, Â¿Ã€Â´Ãƒ Ã‡ÃÂ·Ã§ Â°Ã”Ã€Ã“ ÃÃ¸Ã‡Ã  Â½ÃƒÂ°Â£
	@i_LastGameEndDate					VARCHAR(30),	-- 2006-11-15 by cmkwon, Â¸Â¶ÃÃ¶Â¸Â· Â°Ã”Ã€Ã“ ÃÂ¾Â·Ã¡ Â³Â¯Ã‚Â¥Â½ÃƒÂ°Â£
	@i_PCBangtotalPlayTime	BIGINT,						-- 2007-06-07 by dhjin, PCÂ¹Ã¦ ÃƒÃ‘ Ã‡ÃƒÂ·Â¹Ã€ÃŒ Â½ÃƒÂ°Â£
	@i_SecretInfoOption		INT				-- // 2008-06-23 by dhjin, EP3 Ã€Â¯Ã€ÃºÃÂ¤ÂºÂ¸Â¿Ã‰Â¼Ã‡
AS
	UPDATE td_Character
	SET	Level			= @i_Level,
		CharacterMode	= @i_CharacterMode,
		Experience		= @i_Experience,
		DownExperience	= @i_DownExperience,
		DownSPIOnDeath	= @i_DownSPIOnDeath,		-- 2006-04-10 by cmkwon
		BodyCondition	= @i_BodyCondition,
		PetLevel		= @i_PetLevel,
		PetExperience	= @i_PetExperience,
		Position_X		= @i_Position_X,
		Position_Y		= @i_Position_Y,
		Position_Z		= @i_Position_Z,
		MapIndex		= @i_MapIndex,
		ChannelIndex	= @i_ChannelIndex,
		HP				= @i_HP,
		DP				= @i_DP,
		SP				= @i_SP,
		EP				= @i_EP,
		currentHP		= @i_currentHP,
		currentDP		= @i_currentDP,
		currentSP		= @i_currentSP,
		currentEP		= @i_currentEP,
		TotalPlayTime	= @i_totalPlayTime,
		PCBangTotalPlayTime = @i_PCBangtotalPlayTime,
		SecretInfoOption	= @i_SecretInfoOption				-- // 2008-06-23 by dhjin, EP3 Ã€Â¯Ã€ÃºÃÂ¤ÂºÂ¸Â¿Ã‰Â¼Ã‡
	WHERE UniqueNumber = @i_CharacterUID
	
	--------------------------------------------------------------------------------
	-- 2006-11-15 by cmkwon
	-- atum2_db_account.dbo.td_account Ã…Ã—Ã€ÃŒÂºÃ­Â¿Â¡ Â°Ã”Ã€Ã“ÃÃ¶Â¼Ã“Â½ÃƒÂ°Â£, Â¸Â¶ÃÃ¶Â¸Â· Â°Ã”Ã€Ã“ ÃÂ¾Â·Ã¡Â½ÃƒÂ°Â£Ã€Â» Â¾Ã·ÂµÂ¥Ã€ÃŒÃ†Â® Ã‡Ã‘Â´Ã™.
	UPDATE atum2_db_account.dbo.td_account
		SET GameContinueTimeInSecondOfToday = @i_GameContinueTimeInSecondOfToday,
			LastGameEndDate = @i_LastGameEndDate
		FROM atum2_db_account.dbo.td_account a, td_character c
		WHERE c.UniqueNumber = @i_CharacterUID AND a.AccountUniqueNumber = c.AccountUniqueNumber


