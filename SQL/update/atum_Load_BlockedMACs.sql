-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================
USE [atum2_db_account]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Guss
-- Create date: 01/04/17
-- Description:	load blocked mac on ace online server
-- =============================================
CREATE PROCEDURE atum_Load_BlockedMACs 
AS
BEGIN
    -- Insert statements for procedure here
	SELECT * FROM dbo.td_blockedMacs WITH (NOLOCK)
END
GO
