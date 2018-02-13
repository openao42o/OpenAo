# Microsoft Developer Studio Generated NMAKE File, Based on FieldServer.dsp
!IF "$(CFG)" == ""
CFG=FieldServer - Win32 Debug_MGAME
!MESSAGE No configuration specified. Defaulting to FieldServer - Win32 Debug_MGAME.
!ENDIF 

!IF "$(CFG)" != "FieldServer - Win32 Release" && "$(CFG)" != "FieldServer - Win32 Debug" && "$(CFG)" != "FieldServer - Win32 Debug_MGAME"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FieldServer.mak" CFG="FieldServer - Win32 Debug_MGAME"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FieldServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FieldServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FieldServer - Win32 Debug_MGAME" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "FieldServer - Win32 Release"

OUTDIR=.\..\..\output\FieldServer\Release
INTDIR=.\..\..\output\FieldServer\Release
# Begin Custom Macros
OutDir=.\..\..\output\FieldServer\Release
# End Custom Macros

ALL : "..\..\Bin\Release\FieldServer.exe" "$(OUTDIR)\FieldServer.bsc"


CLEAN :
	-@erase "$(INTDIR)\AtumDBManager.obj"
	-@erase "$(INTDIR)\AtumDBManager.sbr"
	-@erase "$(INTDIR)\AtumFieldDBManager.obj"
	-@erase "$(INTDIR)\AtumFieldDBManager.sbr"
	-@erase "$(INTDIR)\AtumParamProtocol.obj"
	-@erase "$(INTDIR)\AtumParamProtocol.sbr"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.obj"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.sbr"
	-@erase "$(INTDIR)\AtumSJ.obj"
	-@erase "$(INTDIR)\AtumSJ.sbr"
	-@erase "$(INTDIR)\AtumTimer.obj"
	-@erase "$(INTDIR)\AtumTimer.sbr"
	-@erase "$(INTDIR)\Config.obj"
	-@erase "$(INTDIR)\Config.sbr"
	-@erase "$(INTDIR)\FieldGlobal.obj"
	-@erase "$(INTDIR)\FieldGlobal.sbr"
	-@erase "$(INTDIR)\FieldIOCP.obj"
	-@erase "$(INTDIR)\FieldIOCP.sbr"
	-@erase "$(INTDIR)\FieldIOCPSocket.obj"
	-@erase "$(INTDIR)\FieldIOCPSocket.sbr"
	-@erase "$(INTDIR)\FieldMapChannel.obj"
	-@erase "$(INTDIR)\FieldMapChannel.sbr"
	-@erase "$(INTDIR)\FieldMapProject.obj"
	-@erase "$(INTDIR)\FieldMapProject.sbr"
	-@erase "$(INTDIR)\FieldMapWorkspace.obj"
	-@erase "$(INTDIR)\FieldMapWorkspace.sbr"
	-@erase "$(INTDIR)\FieldMonster.obj"
	-@erase "$(INTDIR)\FieldMonster.sbr"
	-@erase "$(INTDIR)\FieldServer.obj"
	-@erase "$(INTDIR)\FieldServer.pch"
	-@erase "$(INTDIR)\FieldServer.res"
	-@erase "$(INTDIR)\FieldServer.sbr"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\Global.sbr"
	-@erase "$(INTDIR)\GlobalGameServer.obj"
	-@erase "$(INTDIR)\GlobalGameServer.sbr"
	-@erase "$(INTDIR)\IMWinSocket.obj"
	-@erase "$(INTDIR)\IMWinSocket.sbr"
	-@erase "$(INTDIR)\IOCP.obj"
	-@erase "$(INTDIR)\IOCP.sbr"
	-@erase "$(INTDIR)\IOCPSocket.obj"
	-@erase "$(INTDIR)\IOCPSocket.sbr"
	-@erase "$(INTDIR)\LogWinSocket.obj"
	-@erase "$(INTDIR)\LogWinSocket.sbr"
	-@erase "$(INTDIR)\MapBlock.obj"
	-@erase "$(INTDIR)\MapBlock.sbr"
	-@erase "$(INTDIR)\MapChannel.obj"
	-@erase "$(INTDIR)\MapChannel.sbr"
	-@erase "$(INTDIR)\MapProject.obj"
	-@erase "$(INTDIR)\MapProject.sbr"
	-@erase "$(INTDIR)\MapWorkspace.obj"
	-@erase "$(INTDIR)\MapWorkspace.sbr"
	-@erase "$(INTDIR)\Monster.obj"
	-@erase "$(INTDIR)\Monster.sbr"
	-@erase "$(INTDIR)\MonsterDBAccess.obj"
	-@erase "$(INTDIR)\MonsterDBAccess.sbr"
	-@erase "$(INTDIR)\NPCScripts.obj"
	-@erase "$(INTDIR)\NPCScripts.sbr"
	-@erase "$(INTDIR)\Overlapped.obj"
	-@erase "$(INTDIR)\Overlapped.sbr"
	-@erase "$(INTDIR)\PartyGuild.obj"
	-@erase "$(INTDIR)\PartyGuild.sbr"
	-@erase "$(INTDIR)\PreWinSocket.obj"
	-@erase "$(INTDIR)\PreWinSocket.sbr"
	-@erase "$(INTDIR)\Quest.obj"
	-@erase "$(INTDIR)\Quest.sbr"
	-@erase "$(INTDIR)\SocketHeader.obj"
	-@erase "$(INTDIR)\SocketHeader.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SystemLogManager.obj"
	-@erase "$(INTDIR)\SystemLogManager.sbr"
	-@erase "$(INTDIR)\ThreadCheck.obj"
	-@erase "$(INTDIR)\ThreadCheck.sbr"
	-@erase "$(INTDIR)\UserVariable.obj"
	-@erase "$(INTDIR)\UserVariable.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VMem.obj"
	-@erase "$(INTDIR)\VMem.sbr"
	-@erase "$(INTDIR)\VMemPool.obj"
	-@erase "$(INTDIR)\VMemPool.sbr"
	-@erase "$(INTDIR)\WinPacket.obj"
	-@erase "$(INTDIR)\WinPacket.sbr"
	-@erase "$(INTDIR)\WinSocket.obj"
	-@erase "$(INTDIR)\WinSocket.sbr"
	-@erase "$(OUTDIR)\FieldServer.bsc"
	-@erase "..\..\Bin\Release\FieldServer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "NDEBUG" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "WIN32" /D "_WINDOWS" /D "_ATUM_SERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\FieldServer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FieldServer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AtumDBManager.sbr" \
	"$(INTDIR)\AtumFieldDBManager.sbr" \
	"$(INTDIR)\AtumParamProtocol.sbr" \
	"$(INTDIR)\AtumParamProtocolFunctions.sbr" \
	"$(INTDIR)\AtumSJ.sbr" \
	"$(INTDIR)\AtumTimer.sbr" \
	"$(INTDIR)\Config.sbr" \
	"$(INTDIR)\FieldGlobal.sbr" \
	"$(INTDIR)\FieldIOCP.sbr" \
	"$(INTDIR)\FieldIOCPSocket.sbr" \
	"$(INTDIR)\FieldMapChannel.sbr" \
	"$(INTDIR)\FieldMapProject.sbr" \
	"$(INTDIR)\FieldMapWorkspace.sbr" \
	"$(INTDIR)\FieldMonster.sbr" \
	"$(INTDIR)\FieldServer.sbr" \
	"$(INTDIR)\Global.sbr" \
	"$(INTDIR)\GlobalGameServer.sbr" \
	"$(INTDIR)\IMWinSocket.sbr" \
	"$(INTDIR)\IOCP.sbr" \
	"$(INTDIR)\IOCPSocket.sbr" \
	"$(INTDIR)\LogWinSocket.sbr" \
	"$(INTDIR)\MapBlock.sbr" \
	"$(INTDIR)\MapChannel.sbr" \
	"$(INTDIR)\MapProject.sbr" \
	"$(INTDIR)\MapWorkspace.sbr" \
	"$(INTDIR)\Monster.sbr" \
	"$(INTDIR)\MonsterDBAccess.sbr" \
	"$(INTDIR)\NPCScripts.sbr" \
	"$(INTDIR)\Overlapped.sbr" \
	"$(INTDIR)\PartyGuild.sbr" \
	"$(INTDIR)\PreWinSocket.sbr" \
	"$(INTDIR)\Quest.sbr" \
	"$(INTDIR)\SocketHeader.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SystemLogManager.sbr" \
	"$(INTDIR)\ThreadCheck.sbr" \
	"$(INTDIR)\UserVariable.sbr" \
	"$(INTDIR)\VMem.sbr" \
	"$(INTDIR)\VMemPool.sbr" \
	"$(INTDIR)\WinPacket.sbr" \
	"$(INTDIR)\WinSocket.sbr"

"$(OUTDIR)\FieldServer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib D3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\FieldServer.pdb" /machine:I386 /out:"..\..\Bin\Release/FieldServer.exe" /libpath:"..\..\Lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\AtumDBManager.obj" \
	"$(INTDIR)\AtumFieldDBManager.obj" \
	"$(INTDIR)\AtumParamProtocol.obj" \
	"$(INTDIR)\AtumParamProtocolFunctions.obj" \
	"$(INTDIR)\AtumSJ.obj" \
	"$(INTDIR)\AtumTimer.obj" \
	"$(INTDIR)\Config.obj" \
	"$(INTDIR)\FieldGlobal.obj" \
	"$(INTDIR)\FieldIOCP.obj" \
	"$(INTDIR)\FieldIOCPSocket.obj" \
	"$(INTDIR)\FieldMapChannel.obj" \
	"$(INTDIR)\FieldMapProject.obj" \
	"$(INTDIR)\FieldMapWorkspace.obj" \
	"$(INTDIR)\FieldMonster.obj" \
	"$(INTDIR)\FieldServer.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\GlobalGameServer.obj" \
	"$(INTDIR)\IMWinSocket.obj" \
	"$(INTDIR)\IOCP.obj" \
	"$(INTDIR)\IOCPSocket.obj" \
	"$(INTDIR)\LogWinSocket.obj" \
	"$(INTDIR)\MapBlock.obj" \
	"$(INTDIR)\MapChannel.obj" \
	"$(INTDIR)\MapProject.obj" \
	"$(INTDIR)\MapWorkspace.obj" \
	"$(INTDIR)\Monster.obj" \
	"$(INTDIR)\MonsterDBAccess.obj" \
	"$(INTDIR)\NPCScripts.obj" \
	"$(INTDIR)\Overlapped.obj" \
	"$(INTDIR)\PartyGuild.obj" \
	"$(INTDIR)\PreWinSocket.obj" \
	"$(INTDIR)\Quest.obj" \
	"$(INTDIR)\SocketHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemLogManager.obj" \
	"$(INTDIR)\ThreadCheck.obj" \
	"$(INTDIR)\UserVariable.obj" \
	"$(INTDIR)\VMem.obj" \
	"$(INTDIR)\VMemPool.obj" \
	"$(INTDIR)\WinPacket.obj" \
	"$(INTDIR)\WinSocket.obj" \
	"$(INTDIR)\FieldServer.res"

"..\..\Bin\Release\FieldServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FieldServer - Win32 Debug"

OUTDIR=.\..\..\output\FieldServer\Debug
INTDIR=.\..\..\output\FieldServer\Debug
# Begin Custom Macros
OutDir=.\..\..\output\FieldServer\Debug
# End Custom Macros

ALL : "..\..\Bin\Debug\FieldServer.exe" "$(OUTDIR)\FieldServer.pch" "$(OUTDIR)\FieldServer.bsc"


CLEAN :
	-@erase "$(INTDIR)\AtumDBManager.obj"
	-@erase "$(INTDIR)\AtumDBManager.sbr"
	-@erase "$(INTDIR)\AtumFieldDBManager.obj"
	-@erase "$(INTDIR)\AtumFieldDBManager.sbr"
	-@erase "$(INTDIR)\AtumParamProtocol.obj"
	-@erase "$(INTDIR)\AtumParamProtocol.sbr"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.obj"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.sbr"
	-@erase "$(INTDIR)\AtumSJ.obj"
	-@erase "$(INTDIR)\AtumSJ.sbr"
	-@erase "$(INTDIR)\AtumTimer.obj"
	-@erase "$(INTDIR)\AtumTimer.sbr"
	-@erase "$(INTDIR)\Config.obj"
	-@erase "$(INTDIR)\Config.sbr"
	-@erase "$(INTDIR)\FieldGlobal.obj"
	-@erase "$(INTDIR)\FieldGlobal.sbr"
	-@erase "$(INTDIR)\FieldIOCP.obj"
	-@erase "$(INTDIR)\FieldIOCP.sbr"
	-@erase "$(INTDIR)\FieldIOCPSocket.obj"
	-@erase "$(INTDIR)\FieldIOCPSocket.sbr"
	-@erase "$(INTDIR)\FieldMapChannel.obj"
	-@erase "$(INTDIR)\FieldMapChannel.sbr"
	-@erase "$(INTDIR)\FieldMapProject.obj"
	-@erase "$(INTDIR)\FieldMapProject.sbr"
	-@erase "$(INTDIR)\FieldMapWorkspace.obj"
	-@erase "$(INTDIR)\FieldMapWorkspace.sbr"
	-@erase "$(INTDIR)\FieldMonster.obj"
	-@erase "$(INTDIR)\FieldMonster.sbr"
	-@erase "$(INTDIR)\FieldServer.obj"
	-@erase "$(INTDIR)\FieldServer.pch"
	-@erase "$(INTDIR)\FieldServer.res"
	-@erase "$(INTDIR)\FieldServer.sbr"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\Global.sbr"
	-@erase "$(INTDIR)\GlobalGameServer.obj"
	-@erase "$(INTDIR)\GlobalGameServer.sbr"
	-@erase "$(INTDIR)\IMWinSocket.obj"
	-@erase "$(INTDIR)\IMWinSocket.sbr"
	-@erase "$(INTDIR)\IOCP.obj"
	-@erase "$(INTDIR)\IOCP.sbr"
	-@erase "$(INTDIR)\IOCPSocket.obj"
	-@erase "$(INTDIR)\IOCPSocket.sbr"
	-@erase "$(INTDIR)\LogWinSocket.obj"
	-@erase "$(INTDIR)\LogWinSocket.sbr"
	-@erase "$(INTDIR)\MapBlock.obj"
	-@erase "$(INTDIR)\MapBlock.sbr"
	-@erase "$(INTDIR)\MapChannel.obj"
	-@erase "$(INTDIR)\MapChannel.sbr"
	-@erase "$(INTDIR)\MapProject.obj"
	-@erase "$(INTDIR)\MapProject.sbr"
	-@erase "$(INTDIR)\MapWorkspace.obj"
	-@erase "$(INTDIR)\MapWorkspace.sbr"
	-@erase "$(INTDIR)\Monster.obj"
	-@erase "$(INTDIR)\Monster.sbr"
	-@erase "$(INTDIR)\MonsterDBAccess.obj"
	-@erase "$(INTDIR)\MonsterDBAccess.sbr"
	-@erase "$(INTDIR)\NPCScripts.obj"
	-@erase "$(INTDIR)\NPCScripts.sbr"
	-@erase "$(INTDIR)\Overlapped.obj"
	-@erase "$(INTDIR)\Overlapped.sbr"
	-@erase "$(INTDIR)\PartyGuild.obj"
	-@erase "$(INTDIR)\PartyGuild.sbr"
	-@erase "$(INTDIR)\PreWinSocket.obj"
	-@erase "$(INTDIR)\PreWinSocket.sbr"
	-@erase "$(INTDIR)\Quest.obj"
	-@erase "$(INTDIR)\Quest.sbr"
	-@erase "$(INTDIR)\SocketHeader.obj"
	-@erase "$(INTDIR)\SocketHeader.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SystemLogManager.obj"
	-@erase "$(INTDIR)\SystemLogManager.sbr"
	-@erase "$(INTDIR)\ThreadCheck.obj"
	-@erase "$(INTDIR)\ThreadCheck.sbr"
	-@erase "$(INTDIR)\UserVariable.obj"
	-@erase "$(INTDIR)\UserVariable.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VMem.obj"
	-@erase "$(INTDIR)\VMem.sbr"
	-@erase "$(INTDIR)\VMemPool.obj"
	-@erase "$(INTDIR)\VMemPool.sbr"
	-@erase "$(INTDIR)\WinPacket.obj"
	-@erase "$(INTDIR)\WinPacket.sbr"
	-@erase "$(INTDIR)\WinSocket.obj"
	-@erase "$(INTDIR)\WinSocket.sbr"
	-@erase "$(OUTDIR)\FieldServer.bsc"
	-@erase "$(OUTDIR)\FieldServer.pdb"
	-@erase "..\..\Bin\Debug\FieldServer.exe"
	-@erase "..\..\Bin\Debug\FieldServer.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "_ATUM_SERVER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /YX"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\FieldServer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FieldServer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AtumDBManager.sbr" \
	"$(INTDIR)\AtumFieldDBManager.sbr" \
	"$(INTDIR)\AtumParamProtocol.sbr" \
	"$(INTDIR)\AtumParamProtocolFunctions.sbr" \
	"$(INTDIR)\AtumSJ.sbr" \
	"$(INTDIR)\AtumTimer.sbr" \
	"$(INTDIR)\Config.sbr" \
	"$(INTDIR)\FieldGlobal.sbr" \
	"$(INTDIR)\FieldIOCP.sbr" \
	"$(INTDIR)\FieldIOCPSocket.sbr" \
	"$(INTDIR)\FieldMapChannel.sbr" \
	"$(INTDIR)\FieldMapProject.sbr" \
	"$(INTDIR)\FieldMapWorkspace.sbr" \
	"$(INTDIR)\FieldMonster.sbr" \
	"$(INTDIR)\FieldServer.sbr" \
	"$(INTDIR)\Global.sbr" \
	"$(INTDIR)\GlobalGameServer.sbr" \
	"$(INTDIR)\IMWinSocket.sbr" \
	"$(INTDIR)\IOCP.sbr" \
	"$(INTDIR)\IOCPSocket.sbr" \
	"$(INTDIR)\LogWinSocket.sbr" \
	"$(INTDIR)\MapBlock.sbr" \
	"$(INTDIR)\MapChannel.sbr" \
	"$(INTDIR)\MapProject.sbr" \
	"$(INTDIR)\MapWorkspace.sbr" \
	"$(INTDIR)\Monster.sbr" \
	"$(INTDIR)\MonsterDBAccess.sbr" \
	"$(INTDIR)\NPCScripts.sbr" \
	"$(INTDIR)\Overlapped.sbr" \
	"$(INTDIR)\PartyGuild.sbr" \
	"$(INTDIR)\PreWinSocket.sbr" \
	"$(INTDIR)\Quest.sbr" \
	"$(INTDIR)\SocketHeader.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SystemLogManager.sbr" \
	"$(INTDIR)\ThreadCheck.sbr" \
	"$(INTDIR)\UserVariable.sbr" \
	"$(INTDIR)\VMem.sbr" \
	"$(INTDIR)\VMemPool.sbr" \
	"$(INTDIR)\WinPacket.sbr" \
	"$(INTDIR)\WinSocket.sbr"

"$(OUTDIR)\FieldServer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib D3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib QuestLoaderLib.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\FieldServer.pdb" /debug /machine:I386 /out:"..\..\Bin\Debug/FieldServer.exe" /libpath:"..\..\Lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\AtumDBManager.obj" \
	"$(INTDIR)\AtumFieldDBManager.obj" \
	"$(INTDIR)\AtumParamProtocol.obj" \
	"$(INTDIR)\AtumParamProtocolFunctions.obj" \
	"$(INTDIR)\AtumSJ.obj" \
	"$(INTDIR)\AtumTimer.obj" \
	"$(INTDIR)\Config.obj" \
	"$(INTDIR)\FieldGlobal.obj" \
	"$(INTDIR)\FieldIOCP.obj" \
	"$(INTDIR)\FieldIOCPSocket.obj" \
	"$(INTDIR)\FieldMapChannel.obj" \
	"$(INTDIR)\FieldMapProject.obj" \
	"$(INTDIR)\FieldMapWorkspace.obj" \
	"$(INTDIR)\FieldMonster.obj" \
	"$(INTDIR)\FieldServer.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\GlobalGameServer.obj" \
	"$(INTDIR)\IMWinSocket.obj" \
	"$(INTDIR)\IOCP.obj" \
	"$(INTDIR)\IOCPSocket.obj" \
	"$(INTDIR)\LogWinSocket.obj" \
	"$(INTDIR)\MapBlock.obj" \
	"$(INTDIR)\MapChannel.obj" \
	"$(INTDIR)\MapProject.obj" \
	"$(INTDIR)\MapWorkspace.obj" \
	"$(INTDIR)\Monster.obj" \
	"$(INTDIR)\MonsterDBAccess.obj" \
	"$(INTDIR)\NPCScripts.obj" \
	"$(INTDIR)\Overlapped.obj" \
	"$(INTDIR)\PartyGuild.obj" \
	"$(INTDIR)\PreWinSocket.obj" \
	"$(INTDIR)\Quest.obj" \
	"$(INTDIR)\SocketHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemLogManager.obj" \
	"$(INTDIR)\ThreadCheck.obj" \
	"$(INTDIR)\UserVariable.obj" \
	"$(INTDIR)\VMem.obj" \
	"$(INTDIR)\VMemPool.obj" \
	"$(INTDIR)\WinPacket.obj" \
	"$(INTDIR)\WinSocket.obj" \
	"$(INTDIR)\FieldServer.res"

"..\..\Bin\Debug\FieldServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FieldServer - Win32 Debug_MGAME"

OUTDIR=.\..\..\output\FieldServer___Win32_Debug_MGAME
INTDIR=.\..\..\output\FieldServer___Win32_Debug_MGAME
# Begin Custom Macros
OutDir=.\..\..\output\FieldServer___Win32_Debug_MGAME
# End Custom Macros

ALL : "..\..\Bin\Debug_MGAME\FieldServer.exe" "$(OUTDIR)\FieldServer.bsc"


CLEAN :
	-@erase "$(INTDIR)\AtumDBManager.obj"
	-@erase "$(INTDIR)\AtumDBManager.sbr"
	-@erase "$(INTDIR)\AtumFieldDBManager.obj"
	-@erase "$(INTDIR)\AtumFieldDBManager.sbr"
	-@erase "$(INTDIR)\AtumParamProtocol.obj"
	-@erase "$(INTDIR)\AtumParamProtocol.sbr"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.obj"
	-@erase "$(INTDIR)\AtumParamProtocolFunctions.sbr"
	-@erase "$(INTDIR)\AtumSJ.obj"
	-@erase "$(INTDIR)\AtumSJ.sbr"
	-@erase "$(INTDIR)\AtumTimer.obj"
	-@erase "$(INTDIR)\AtumTimer.sbr"
	-@erase "$(INTDIR)\Config.obj"
	-@erase "$(INTDIR)\Config.sbr"
	-@erase "$(INTDIR)\FieldGlobal.obj"
	-@erase "$(INTDIR)\FieldGlobal.sbr"
	-@erase "$(INTDIR)\FieldIOCP.obj"
	-@erase "$(INTDIR)\FieldIOCP.sbr"
	-@erase "$(INTDIR)\FieldIOCPSocket.obj"
	-@erase "$(INTDIR)\FieldIOCPSocket.sbr"
	-@erase "$(INTDIR)\FieldMapChannel.obj"
	-@erase "$(INTDIR)\FieldMapChannel.sbr"
	-@erase "$(INTDIR)\FieldMapProject.obj"
	-@erase "$(INTDIR)\FieldMapProject.sbr"
	-@erase "$(INTDIR)\FieldMapWorkspace.obj"
	-@erase "$(INTDIR)\FieldMapWorkspace.sbr"
	-@erase "$(INTDIR)\FieldMonster.obj"
	-@erase "$(INTDIR)\FieldMonster.sbr"
	-@erase "$(INTDIR)\FieldServer.obj"
	-@erase "$(INTDIR)\FieldServer.pch"
	-@erase "$(INTDIR)\FieldServer.res"
	-@erase "$(INTDIR)\FieldServer.sbr"
	-@erase "$(INTDIR)\Global.obj"
	-@erase "$(INTDIR)\Global.sbr"
	-@erase "$(INTDIR)\GlobalGameServer.obj"
	-@erase "$(INTDIR)\GlobalGameServer.sbr"
	-@erase "$(INTDIR)\IMWinSocket.obj"
	-@erase "$(INTDIR)\IMWinSocket.sbr"
	-@erase "$(INTDIR)\IOCP.obj"
	-@erase "$(INTDIR)\IOCP.sbr"
	-@erase "$(INTDIR)\IOCPSocket.obj"
	-@erase "$(INTDIR)\IOCPSocket.sbr"
	-@erase "$(INTDIR)\LogWinSocket.obj"
	-@erase "$(INTDIR)\LogWinSocket.sbr"
	-@erase "$(INTDIR)\MapBlock.obj"
	-@erase "$(INTDIR)\MapBlock.sbr"
	-@erase "$(INTDIR)\MapChannel.obj"
	-@erase "$(INTDIR)\MapChannel.sbr"
	-@erase "$(INTDIR)\MapProject.obj"
	-@erase "$(INTDIR)\MapProject.sbr"
	-@erase "$(INTDIR)\MapWorkspace.obj"
	-@erase "$(INTDIR)\MapWorkspace.sbr"
	-@erase "$(INTDIR)\Monster.obj"
	-@erase "$(INTDIR)\Monster.sbr"
	-@erase "$(INTDIR)\MonsterDBAccess.obj"
	-@erase "$(INTDIR)\MonsterDBAccess.sbr"
	-@erase "$(INTDIR)\NPCScripts.obj"
	-@erase "$(INTDIR)\NPCScripts.sbr"
	-@erase "$(INTDIR)\Overlapped.obj"
	-@erase "$(INTDIR)\Overlapped.sbr"
	-@erase "$(INTDIR)\PartyGuild.obj"
	-@erase "$(INTDIR)\PartyGuild.sbr"
	-@erase "$(INTDIR)\PreWinSocket.obj"
	-@erase "$(INTDIR)\PreWinSocket.sbr"
	-@erase "$(INTDIR)\Quest.obj"
	-@erase "$(INTDIR)\Quest.sbr"
	-@erase "$(INTDIR)\SocketHeader.obj"
	-@erase "$(INTDIR)\SocketHeader.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SystemLogManager.obj"
	-@erase "$(INTDIR)\SystemLogManager.sbr"
	-@erase "$(INTDIR)\ThreadCheck.obj"
	-@erase "$(INTDIR)\ThreadCheck.sbr"
	-@erase "$(INTDIR)\UserVariable.obj"
	-@erase "$(INTDIR)\UserVariable.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VMem.obj"
	-@erase "$(INTDIR)\VMem.sbr"
	-@erase "$(INTDIR)\VMemPool.obj"
	-@erase "$(INTDIR)\VMemPool.sbr"
	-@erase "$(INTDIR)\WinPacket.obj"
	-@erase "$(INTDIR)\WinPacket.sbr"
	-@erase "$(INTDIR)\WinSocket.obj"
	-@erase "$(INTDIR)\WinSocket.sbr"
	-@erase "$(OUTDIR)\FieldServer.bsc"
	-@erase "$(OUTDIR)\FieldServer.pdb"
	-@erase "..\..\Bin\Debug_MGAME\FieldServer.exe"
	-@erase "..\..\Bin\Debug_MGAME\FieldServer.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "_ATUM_LOAD_TEST" /D "_ATUM_SERVER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MGAME" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x412 /fo"$(INTDIR)\FieldServer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\FieldServer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AtumDBManager.sbr" \
	"$(INTDIR)\AtumFieldDBManager.sbr" \
	"$(INTDIR)\AtumParamProtocol.sbr" \
	"$(INTDIR)\AtumParamProtocolFunctions.sbr" \
	"$(INTDIR)\AtumSJ.sbr" \
	"$(INTDIR)\AtumTimer.sbr" \
	"$(INTDIR)\Config.sbr" \
	"$(INTDIR)\FieldGlobal.sbr" \
	"$(INTDIR)\FieldIOCP.sbr" \
	"$(INTDIR)\FieldIOCPSocket.sbr" \
	"$(INTDIR)\FieldMapChannel.sbr" \
	"$(INTDIR)\FieldMapProject.sbr" \
	"$(INTDIR)\FieldMapWorkspace.sbr" \
	"$(INTDIR)\FieldMonster.sbr" \
	"$(INTDIR)\FieldServer.sbr" \
	"$(INTDIR)\Global.sbr" \
	"$(INTDIR)\GlobalGameServer.sbr" \
	"$(INTDIR)\IMWinSocket.sbr" \
	"$(INTDIR)\IOCP.sbr" \
	"$(INTDIR)\IOCPSocket.sbr" \
	"$(INTDIR)\LogWinSocket.sbr" \
	"$(INTDIR)\MapBlock.sbr" \
	"$(INTDIR)\MapChannel.sbr" \
	"$(INTDIR)\MapProject.sbr" \
	"$(INTDIR)\MapWorkspace.sbr" \
	"$(INTDIR)\Monster.sbr" \
	"$(INTDIR)\MonsterDBAccess.sbr" \
	"$(INTDIR)\NPCScripts.sbr" \
	"$(INTDIR)\Overlapped.sbr" \
	"$(INTDIR)\PartyGuild.sbr" \
	"$(INTDIR)\PreWinSocket.sbr" \
	"$(INTDIR)\Quest.sbr" \
	"$(INTDIR)\SocketHeader.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SystemLogManager.sbr" \
	"$(INTDIR)\ThreadCheck.sbr" \
	"$(INTDIR)\UserVariable.sbr" \
	"$(INTDIR)\VMem.sbr" \
	"$(INTDIR)\VMemPool.sbr" \
	"$(INTDIR)\WinPacket.sbr" \
	"$(INTDIR)\WinSocket.sbr"

"$(OUTDIR)\FieldServer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=winmm.lib D3dx8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib AtumDataLoader.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\FieldServer.pdb" /debug /machine:I386 /out:"..\..\Bin\Debug_MGAME/FieldServer.exe" /libpath:"..\..\Lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\AtumDBManager.obj" \
	"$(INTDIR)\AtumFieldDBManager.obj" \
	"$(INTDIR)\AtumParamProtocol.obj" \
	"$(INTDIR)\AtumParamProtocolFunctions.obj" \
	"$(INTDIR)\AtumSJ.obj" \
	"$(INTDIR)\AtumTimer.obj" \
	"$(INTDIR)\Config.obj" \
	"$(INTDIR)\FieldGlobal.obj" \
	"$(INTDIR)\FieldIOCP.obj" \
	"$(INTDIR)\FieldIOCPSocket.obj" \
	"$(INTDIR)\FieldMapChannel.obj" \
	"$(INTDIR)\FieldMapProject.obj" \
	"$(INTDIR)\FieldMapWorkspace.obj" \
	"$(INTDIR)\FieldMonster.obj" \
	"$(INTDIR)\FieldServer.obj" \
	"$(INTDIR)\Global.obj" \
	"$(INTDIR)\GlobalGameServer.obj" \
	"$(INTDIR)\IMWinSocket.obj" \
	"$(INTDIR)\IOCP.obj" \
	"$(INTDIR)\IOCPSocket.obj" \
	"$(INTDIR)\LogWinSocket.obj" \
	"$(INTDIR)\MapBlock.obj" \
	"$(INTDIR)\MapChannel.obj" \
	"$(INTDIR)\MapProject.obj" \
	"$(INTDIR)\MapWorkspace.obj" \
	"$(INTDIR)\Monster.obj" \
	"$(INTDIR)\MonsterDBAccess.obj" \
	"$(INTDIR)\NPCScripts.obj" \
	"$(INTDIR)\Overlapped.obj" \
	"$(INTDIR)\PartyGuild.obj" \
	"$(INTDIR)\PreWinSocket.obj" \
	"$(INTDIR)\Quest.obj" \
	"$(INTDIR)\SocketHeader.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SystemLogManager.obj" \
	"$(INTDIR)\ThreadCheck.obj" \
	"$(INTDIR)\UserVariable.obj" \
	"$(INTDIR)\VMem.obj" \
	"$(INTDIR)\VMemPool.obj" \
	"$(INTDIR)\WinPacket.obj" \
	"$(INTDIR)\WinSocket.obj" \
	"$(INTDIR)\FieldServer.res"

"..\..\Bin\Debug_MGAME\FieldServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("FieldServer.dep")
!INCLUDE "FieldServer.dep"
!ELSE 
!MESSAGE Warning: cannot find "FieldServer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "FieldServer - Win32 Release" || "$(CFG)" == "FieldServer - Win32 Debug" || "$(CFG)" == "FieldServer - Win32 Debug_MGAME"
SOURCE=..\CommonGameServer\AtumDBManager.cpp

"$(INTDIR)\AtumDBManager.obj"	"$(INTDIR)\AtumDBManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\AtumFieldDBManager.cpp

"$(INTDIR)\AtumFieldDBManager.obj"	"$(INTDIR)\AtumFieldDBManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\AtumParamProtocol.cpp

"$(INTDIR)\AtumParamProtocol.obj"	"$(INTDIR)\AtumParamProtocol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\AtumParamProtocolFunctions.cpp

"$(INTDIR)\AtumParamProtocolFunctions.obj"	"$(INTDIR)\AtumParamProtocolFunctions.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\AtumSJ.cpp

"$(INTDIR)\AtumSJ.obj"	"$(INTDIR)\AtumSJ.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\AtumTimer.cpp

"$(INTDIR)\AtumTimer.obj"	"$(INTDIR)\AtumTimer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\Config.cpp

"$(INTDIR)\Config.obj"	"$(INTDIR)\Config.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Main\FieldGlobal.cpp

"$(INTDIR)\FieldGlobal.obj"	"$(INTDIR)\FieldGlobal.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\FieldIOCP.cpp

"$(INTDIR)\FieldIOCP.obj"	"$(INTDIR)\FieldIOCP.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\FieldIOCPSocket.cpp

"$(INTDIR)\FieldIOCPSocket.obj"	"$(INTDIR)\FieldIOCPSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Main\FieldMapChannel.cpp

"$(INTDIR)\FieldMapChannel.obj"	"$(INTDIR)\FieldMapChannel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\FieldMapProject.cpp

"$(INTDIR)\FieldMapProject.obj"	"$(INTDIR)\FieldMapProject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\FieldMapWorkspace.cpp

"$(INTDIR)\FieldMapWorkspace.obj"	"$(INTDIR)\FieldMapWorkspace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Main\FieldMonster.cpp

"$(INTDIR)\FieldMonster.obj"	"$(INTDIR)\FieldMonster.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\FieldServer.cpp

"$(INTDIR)\FieldServer.obj"	"$(INTDIR)\FieldServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"


SOURCE=.\FieldServer.rc

"$(INTDIR)\FieldServer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=..\..\Common\Global.cpp

"$(INTDIR)\Global.obj"	"$(INTDIR)\Global.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\GlobalGameServer.cpp

"$(INTDIR)\GlobalGameServer.obj"	"$(INTDIR)\GlobalGameServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\IMWinSocket.cpp

"$(INTDIR)\IMWinSocket.obj"	"$(INTDIR)\IMWinSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\IOCP.cpp

"$(INTDIR)\IOCP.obj"	"$(INTDIR)\IOCP.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\IOCPSocket.cpp

"$(INTDIR)\IOCPSocket.obj"	"$(INTDIR)\IOCPSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Main\LogWinSocket.cpp

"$(INTDIR)\LogWinSocket.obj"	"$(INTDIR)\LogWinSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\MapBlock.cpp

"$(INTDIR)\MapBlock.obj"	"$(INTDIR)\MapBlock.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\MapChannel.cpp

"$(INTDIR)\MapChannel.obj"	"$(INTDIR)\MapChannel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\MapProject.cpp

"$(INTDIR)\MapProject.obj"	"$(INTDIR)\MapProject.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\MapWorkspace.cpp

"$(INTDIR)\MapWorkspace.obj"	"$(INTDIR)\MapWorkspace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\Monster.cpp

"$(INTDIR)\Monster.obj"	"$(INTDIR)\Monster.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\MonsterDBAccess.cpp

"$(INTDIR)\MonsterDBAccess.obj"	"$(INTDIR)\MonsterDBAccess.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\NPCScripts.cpp

"$(INTDIR)\NPCScripts.obj"	"$(INTDIR)\NPCScripts.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\Overlapped.cpp

"$(INTDIR)\Overlapped.obj"	"$(INTDIR)\Overlapped.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\PartyGuild.cpp

"$(INTDIR)\PartyGuild.obj"	"$(INTDIR)\PartyGuild.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\main\PreWinSocket.cpp

"$(INTDIR)\PreWinSocket.obj"	"$(INTDIR)\PreWinSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\Quest.cpp

"$(INTDIR)\Quest.obj"	"$(INTDIR)\Quest.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\SocketHeader.cpp

"$(INTDIR)\SocketHeader.obj"	"$(INTDIR)\SocketHeader.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "FieldServer - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "NDEBUG" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "WIN32" /D "_WINDOWS" /D "_ATUM_SERVER" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FieldServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "FieldServer - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "_ATUM_SERVER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FieldServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "FieldServer - Win32 Debug_MGAME"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\Common" /I ".\Main" /I "..\CommonGameServer" /D "_MBCS" /D "_ATUM_FIELD_SERVER" /D "_ATUM_LOAD_TEST" /D "_ATUM_SERVER" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MGAME" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\FieldServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\FieldServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\..\Common\SystemLogManager.cpp

"$(INTDIR)\SystemLogManager.obj"	"$(INTDIR)\SystemLogManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\ThreadCheck.cpp

"$(INTDIR)\ThreadCheck.obj"	"$(INTDIR)\ThreadCheck.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\CommonGameServer\UserVariable.cpp

"$(INTDIR)\UserVariable.obj"	"$(INTDIR)\UserVariable.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\VMem.cpp

"$(INTDIR)\VMem.obj"	"$(INTDIR)\VMem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\VMemPool.cpp

"$(INTDIR)\VMemPool.obj"	"$(INTDIR)\VMemPool.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\WinPacket.cpp

"$(INTDIR)\WinPacket.obj"	"$(INTDIR)\WinPacket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\Common\WinSocket.cpp

"$(INTDIR)\WinSocket.obj"	"$(INTDIR)\WinSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\FieldServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

