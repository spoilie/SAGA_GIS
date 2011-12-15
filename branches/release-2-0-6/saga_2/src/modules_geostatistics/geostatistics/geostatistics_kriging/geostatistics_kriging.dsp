# Microsoft Developer Studio Project File - Name="geostatistics_kriging" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=geostatistics_kriging - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "geostatistics_kriging.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "geostatistics_kriging.mak" CFG="geostatistics_kriging - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "geostatistics_kriging - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "geostatistics_kriging - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "geostatistics_kriging - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "geostatistics_kriging - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "geostatistics_kriging - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\..\..\..\..\bin\saga_vc\modules"
# PROP Intermediate_Dir ".\..\..\..\..\bin\tmp\saga_vc\geostatistics_kriging"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXWIN)/lib/vc6_dll/msw" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "NDEBUG" /D "_USRDLL" /D "_MBCS" /D "_SAGA_VC" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "geostatistics_kriging_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlib.lib wxjpeg.lib wxpng.lib wxtiff.lib wxbase28.lib wxmsw28_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"$(SAGA)/bin/saga_vc" /libpath:"$(WXWIN)/lib/vc6_dll"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy      *.lng.txt      ..\..\..\..\bin\saga_vc\modules\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "geostatistics_kriging - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\..\..\..\..\bin\saga_vc_dbg\modules"
# PROP Intermediate_Dir ".\..\..\..\..\bin\tmp\saga_vc_dbg\geostatistics_kriging"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(WXWIN)/lib/vc6_dll/mswd" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "_USRDLL" /D "_MBCS" /D "_SAGA_VC" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "WXUSINGDLL" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "geostatistics_kriging_EXPORTS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlibd.lib wxjpegd.lib wxpngd.lib wxtiffd.lib wxbase28d.lib wxmsw28d_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(SAGA)/bin/saga_vc_dbg" /libpath:"$(WXWIN)/lib/vc6_dll"

!ELSEIF  "$(CFG)" == "geostatistics_kriging - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "geostatistics_kriging___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "geostatistics_kriging___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./../../../../bin/saga_vc_u_dbg/modules"
# PROP Intermediate_Dir "./../../../../bin/tmp/saga_vc_u_dbg/geostatistics_kriging"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(WXWIN)/lib/vc_lib/msw" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "__WXDEBUG__" /D "_DEBUG" /D "WIN32" /D "_USRDLL" /D "_MBCS" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "_SAGA_VC" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(WXWIN)/lib/vc6_dll/mswud" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "_USRDLL" /D "_MBCS" /D "_SAGA_VC" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_DEBUG" /D "WIN32" /D "_UNICODE" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "_SAGA_UNICODE" /D "geostatistics_kriging_EXPORTS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlibd.lib wxjpegd.lib wxpngd.lib wxtiffd.lib wxbase28d.lib wxmsw28d_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)/lib/vc_lib" /libpath:"$(SAGA)/bin/saga_vc_dbg"
# ADD LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlibd.lib wxjpegd.lib wxpngd.lib wxtiffd.lib wxbase28ud.lib wxmsw28ud_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(SAGA)/bin/saga_vc_u_dbg" /libpath:"$(WXWIN)/lib/vc6_dll"

!ELSEIF  "$(CFG)" == "geostatistics_kriging - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "geostatistics_kriging___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "geostatistics_kriging___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./../../../../bin/saga_vc_u/modules"
# PROP Intermediate_Dir "./../../../../bin/tmp/saga_vc_u/geostatistics_kriging"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(WXWIN)/lib/vc_lib/msw" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "NDEBUG" /D "WIN32" /D "_USRDLL" /D "_MBCS" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "_SAGA_VC" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(WXWIN)/lib/vc6_dll/mswu" /I "$(WXWIN)/include" /I "$(SAGA)/src/saga_core" /D "NDEBUG" /D "_USRDLL" /D "_MBCS" /D "_SAGA_VC" /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "_TYPEDEF_BYTE" /D "_TYPEDEF_WORD" /D "_SAGA_MSW" /D "_SAGA_UNICODE" /D "geostatistics_kriging_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlib.lib wxjpeg.lib wxpng.lib wxtiff.lib wxbase28.lib wxmsw28_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"$(WXWIN)/lib/vc_lib" /libpath:"$(SAGA)/bin/saga_vc"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 comctl32.lib rpcrt4.lib wsock32.lib wxzlib.lib wxjpeg.lib wxpng.lib wxtiff.lib wxbase28u.lib wxmsw28u_core.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib saga_api.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"$(SAGA)/bin/saga_vc_u" /libpath:"$(WXWIN)/lib/vc6_dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "geostatistics_kriging - Win32 Release"
# Name "geostatistics_kriging - Win32 Debug"
# Name "geostatistics_kriging - Win32 Unicode Debug"
# Name "geostatistics_kriging - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_kriging_base.cpp
# End Source File
# Begin Source File

SOURCE=.\_kriging_ordinary.cpp
# End Source File
# Begin Source File

SOURCE=.\_kriging_ordinary_global.cpp
# End Source File
# Begin Source File

SOURCE=.\_kriging_universal.cpp
# End Source File
# Begin Source File

SOURCE=.\_kriging_universal_global.cpp
# End Source File
# Begin Source File

SOURCE=.\kriging_base.cpp
# End Source File
# Begin Source File

SOURCE=.\kriging_ordinary.cpp
# End Source File
# Begin Source File

SOURCE=.\kriging_ordinary_global.cpp
# End Source File
# Begin Source File

SOURCE=.\kriging_universal.cpp
# End Source File
# Begin Source File

SOURCE=.\kriging_universal_global.cpp
# End Source File
# Begin Source File

SOURCE=.\MLB_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\semivariogram.cpp
# End Source File
# Begin Source File

SOURCE=.\variogram_dialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_kriging_base.h
# End Source File
# Begin Source File

SOURCE=.\_kriging_ordinary.h
# End Source File
# Begin Source File

SOURCE=.\_kriging_ordinary_global.h
# End Source File
# Begin Source File

SOURCE=.\_kriging_universal.h
# End Source File
# Begin Source File

SOURCE=.\_kriging_universal_global.h
# End Source File
# Begin Source File

SOURCE=.\kriging_base.h
# End Source File
# Begin Source File

SOURCE=.\kriging_ordinary.h
# End Source File
# Begin Source File

SOURCE=.\kriging_ordinary_global.h
# End Source File
# Begin Source File

SOURCE=.\kriging_universal.h
# End Source File
# Begin Source File

SOURCE=.\kriging_universal_global.h
# End Source File
# Begin Source File

SOURCE=.\MLB_Interface.h
# End Source File
# Begin Source File

SOURCE=.\semivariogram.h
# End Source File
# Begin Source File

SOURCE=.\variogram_dialog.h
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\api_core.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\dataobject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\doc_html.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\doc_pdf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\doc_svg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\geo_tools.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\grid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\mat_tools.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\module.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\module_library.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\parameters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\saga_api.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\shapes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\table.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\table_value.h
# End Source File
# Begin Source File

SOURCE=..\..\..\saga_core\saga_api\tin.h
# End Source File
# End Group
# End Target
# End Project