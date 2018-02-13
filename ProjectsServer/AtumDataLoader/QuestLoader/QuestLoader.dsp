# Microsoft Developer Studio Project File - Name="QuestLoader" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=QuestLoader - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QuestLoader.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QuestLoader.mak" CFG="QuestLoader - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QuestLoader - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "QuestLoader - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QuestLoader - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\output\QuestLoader\Release"
# PROP Intermediate_Dir "..\..\output\QuestLoader\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\..\Common" /I "..\..\GameServer\CommonGameServer" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "YY_NEVER_INTERACTIVE" /D "WIN32" /D "_MBCS" /D "_ATUM_DATA_LOADER" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 libflex.lib /nologo /machine:I386

!ELSEIF  "$(CFG)" == "QuestLoader - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\output\QuestLoader\Debug"
# PROP Intermediate_Dir "..\..\output\QuestLoader\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I "..\..\Common" /I "..\..\GameServer\CommonGameServer" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "YY_NEVER_INTERACTIVE" /D "WIN32" /D "_MBCS" /D "_ATUM_DATA_LOADER" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libflex.lib /nologo /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "QuestLoader - Win32 Release"
# Name "QuestLoader - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\Quest.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestLoader.l.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestLoaderParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\UserVariable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\AtumDataLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\Quest.h
# End Source File
# Begin Source File

SOURCE=.\QuestLoader.cpp.h
# End Source File
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\QuestLoaderParser.h
# End Source File
# Begin Source File

SOURCE=..\..\GameServer\CommonGameServer\UserVariable.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\QuestLoader.l

!IF  "$(CFG)" == "QuestLoader - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Building Lexer
InputPath=.\QuestLoader.l
InputName=QuestLoader

"$(InputName).l.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex.exe -PQuestLoader -t $(InputName).l > $(InputName).l.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "QuestLoader - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Building Lexer
InputPath=.\QuestLoader.l
InputName=QuestLoader

"$(InputName).l.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex.exe -PQuestLoader -t $(InputName).l > $(InputName).l.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QuestLoader.y

!IF  "$(CFG)" == "QuestLoader - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Building Parser
InputPath=.\QuestLoader.y
InputName=QuestLoader

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison.exe -pQuestLoader -o QuestLoader.cpp -d $(InputName).y

# End Custom Build

!ELSEIF  "$(CFG)" == "QuestLoader - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Building Parser
InputPath=.\QuestLoader.y
InputName=QuestLoader

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison.exe -v -pQuestLoader -o QuestLoader.cpp -d $(InputName).y

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
