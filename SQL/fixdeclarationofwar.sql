-- Copy/pasted from RZ because I have lost my own scripts.

USE atum2_db_1
GO

DROP TABLE dbo.td_DeclarationOfWar
GO

CREATE TABLE dbo.td_DeclarationOfWar (
	Influence		TINYINT,
	MSWarStep		TINYINT,
	NCP			INT,
	MSNum               	INT,
	MSAppearanceMap     	INT,
	MSWarStepStartTime  	datetime,	
	MSWarStepEndTime	datetime,
	MSWarStartTime		datetime,	
	MSWarEndTime		datetime,
	SelectCount		TINYINT,
	GiveUp			BIT,
	MSWarEndState		TINYINT
)
CREATE UNIQUE CLUSTERED INDEX idx_on_DeclarationOfWar ON td_DeclarationOfWar(Influence, MSWarStep)
GO

INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 1, 0, 0, 0, GetDate(), DateAdd(day, 7, GetDate()), DateAdd(day, 3, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 2, 0, 0, 0, DateAdd(day, 7, GetDate()), DateAdd(day, 14, GetDate()), DateAdd(day, 10, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 3, 0, 0, 0, DateAdd(day, 14, GetDate()), DateAdd(day, 21, GetDate()), DateAdd(day, 17, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 4, 0, 0, 0, DateAdd(day, 21, GetDate()), DateAdd(day, 28, GetDate()), DateAdd(day, 24, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 5, 0, 0, 0, DateAdd(day, 28, GetDate()), DateAdd(day, 35, GetDate()), DateAdd(day, 31, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (2, 99, 0, 0, 0, DateAdd(day, 35, GetDate()), DateAdd(day, 42, GetDate()), DateAdd(day, 38, GetDate()), NULL, 3, 0, 0)

INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 1, 0, 0, 0, GetDate(), DateAdd(day, 7, GetDate()), DateAdd(day, 3, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 2, 0, 0, 0, DateAdd(day, 7, GetDate()), DateAdd(day, 14, GetDate()), DateAdd(day, 10, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 3, 0, 0, 0, DateAdd(day, 14, GetDate()), DateAdd(day, 21, GetDate()), DateAdd(day, 17, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 4, 0, 0, 0, DateAdd(day, 21, GetDate()), DateAdd(day, 28, GetDate()), DateAdd(day, 24, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 5, 0, 0, 0, DateAdd(day, 28, GetDate()), DateAdd(day, 35, GetDate()), DateAdd(day, 31, GetDate()), NULL, 3, 0, 0)
INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
										, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
	VALUES (4, 99, 0, 0, 0, DateAdd(day, 35, GetDate()), DateAdd(day, 42, GetDate()), DateAdd(day, 38, GetDate()), NULL, 3, 0, 0)
GO