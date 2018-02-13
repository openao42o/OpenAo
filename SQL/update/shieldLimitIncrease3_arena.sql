USE [atum2_db_1]
GO
/****** Object:  StoredProcedure [dbo].[atum_ChangeHPDPSPEP]    Script Date: 09/02/2017 15:16:19 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--!!!!
-- Name: atum_ChangeHPDPSPEP
-- Desc: 
--====
ALTER PROCEDURE [dbo].[atum_ChangeHPDPSPEP]
	@i_CharacterUID		INT,
	@i_HP				INTEGER,
	@i_DP				INTEGER,
	@i_SP				SMALLINT,
	@i_EP				SMALLINT,
	@i_CurrentHP		FLOAT,
	@i_CurrentDP		FLOAT,
	@i_CurrentSP		SMALLINT,
	@i_CurrentEP		FLOAT
AS
	UPDATE td_Character
		SET HP = @i_HP, DP = @i_DP, SP = @i_SP, EP = @i_EP,
			CurrentHP = @i_CurrentHP, CurrentDP = @i_CurrentDP, CurrentSP = @i_CurrentSP, CurrentEP = @i_CurrentEP
		WHERE  UniqueNumber = @i_CharacterUID;

