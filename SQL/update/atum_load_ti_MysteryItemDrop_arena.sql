USE [atum2_db_arena]
GO
/****** Object:  StoredProcedure [dbo].[atum_load_ti_MysteryItemDrop]    Script Date: 31/03/2017 19:32:35 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

--------------------------------------------------------------------------------
-- TABLE NAME: dbo.atum_load_ti_MysteryItemDrop
-- DESC      : 2007-02-22 by dhjin,
--------------------------------------------------------------------------------
ALTER PROCEDURE [dbo].[atum_load_ti_MysteryItemDrop]
--	@i_ServerGroupID	INT
AS
	SELECT D.MysteryItemDropNum,D.ReqUnitKind,D.ReqMinLevel,D.ReqMaxLevel,D.DropItemNum
			,D.MinCount,D.MaxCount,D.Probability,D.PrefixProbability,D.SuffixProbability
			,D.Period,D.CountPerPeriod,C.DropCount,D.starttime		-- // 2008-11-04 by dhjin, Â·Â°Ã…Â°Â¸Ã“Â½Ã…
	FROM atum2_db_account.dbo.ti_MysteryItemDrop AS D WITH(NOLOCK) LEFT JOIN 
			atum2_db_account.dbo.td_MysteryItemDropCount AS C WITH(NOLOCK)
			ON D.MysteryItemDropNum = C.MysteryItemDropNum AND D.DropItemNum = C.DropItemNum --  AND C.ServerGroupID = @i_ServerGroupID

