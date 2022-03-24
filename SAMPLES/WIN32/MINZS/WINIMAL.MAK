# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "WINIMAL.MAK" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : $(OUTDIR)/Winimal.exe $(OUTDIR)/WINIMAL.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /YX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D BASED_FIXED=1 /FR /c
CPP_PROJ=/nologo /W3 /GX /YX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 BASED_FIXED=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"WINIMAL.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"WINIMAL.bsc" 
BSC32_SBRS= \
	$(INTDIR)/MLIBCMNT.SBR \
	$(INTDIR)/MINIMAL.SBR

$(OUTDIR)/WINIMAL.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /PDB:"WinRel/Rdemo.pdb" /MACHINE:I386
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:no /PDB:"WinRel/Rdemo.pdb" /MACHINE:I386\
 /OUT:"WinRel/Winimal.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/MLIBCMNT.OBJ \
	.\DFWLIB.LIB \
	$(INTDIR)/MINIMAL.OBJ

$(OUTDIR)/Winimal.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : $(OUTDIR)/WINIMAL.exe $(OUTDIR)/WINIMAL.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D BASED_FIXED=1 /FR /c
CPP_PROJ=/nologo /W3 /GX /Zi /YX /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D\
 BASED_FIXED=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"WINIMAL.pch" /Fo$(INTDIR)/\
 /Fd$(OUTDIR)/"WINIMAL.pdb" /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"WINIMAL.bsc" 
BSC32_SBRS= \
	$(INTDIR)/MLIBCMNT.SBR \
	$(INTDIR)/MINIMAL.SBR

$(OUTDIR)/WINIMAL.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:yes /PDB:$(OUTDIR)/"WINIMAL.pdb" /DEBUG\
 /MACHINE:I386 /OUT:$(OUTDIR)/"WINIMAL.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/MLIBCMNT.OBJ \
	.\DFWLIB.LIB \
	$(INTDIR)/MINIMAL.OBJ

$(OUTDIR)/WINIMAL.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\MLIBCMNT.C
DEP_MLIBC=\
	\BRENDER\V1_2\TEST_A\INC\BRENDER.H\
	\BRENDER\V1_2\TEST_A\INC\COMPILER.H\
	\BRENDER\V1_2\TEST_A\INC\BRLIMITS.H\
	\BRENDER\V1_2\TEST_A\INC\BRFILE.H\
	\BRENDER\V1_2\TEST_A\INC\BRMEM.H\
	\BRENDER\V1_2\TEST_A\INC\BRDIAG.H\
	\BRENDER\V1_2\TEST_A\INC\BRERROR.H\
	\BRENDER\V1_2\TEST_A\INC\FIXED.H\
	\BRENDER\V1_2\TEST_A\INC\SCALAR.H\
	\BRENDER\V1_2\TEST_A\INC\COLOUR.H\
	\BRENDER\V1_2\TEST_A\INC\MATRIX.H\
	\BRENDER\V1_2\TEST_A\INC\VECTOR.H\
	\BRENDER\V1_2\TEST_A\INC\ANGLES.H\
	\BRENDER\V1_2\TEST_A\INC\QUAT.H\
	\BRENDER\V1_2\TEST_A\INC\TRANSFRM.H\
	\BRENDER\V1_2\TEST_A\INC\PIXELMAP.H\
	\BRENDER\V1_2\TEST_A\INC\BRFONT.H\
	\BRENDER\V1_2\TEST_A\INC\TOKEN.H\
	\BRENDER\V1_2\TEST_A\INC\RENDER.H\
	\BRENDER\V1_2\TEST_A\INC\MATERIAL.H\
	\BRENDER\V1_2\TEST_A\INC\MODEL.H\
	\BRENDER\V1_2\TEST_A\INC\ACTOR.H\
	\BRENDER\V1_2\TEST_A\INC\CAMERA.H\
	\BRENDER\V1_2\TEST_A\INC\LIGHT.H\
	\BRENDER\V1_2\TEST_A\INC\POOL.H\
	\BRENDER\V1_2\TEST_A\INC\CUSTSUPT.H\
	\BRENDER\V1_2\TEST_A\INC\ORDER.H\
	\BRENDER\V1_2\TEST_A\INC\FMT.H\
	\BRENDER\V1_2\TEST_A\INC\FW_P.H\
	\BRENDER\V1_2\TEST_A\INC\MATH_P.H\
	\BRENDER\V1_2\TEST_A\INC\V1DB_P.H\
	\BRENDER\V1_2\TEST_A\INC\PM_P.H\
	\BRENDER\V1_2\TEST_A\INC\COMPEND.H\
	\BRENDER\V1_2\TEST_A\INC\PRETOK.H\
	\BRENDER\V1_2\TEST_A\INC\FMT_P.H\
	\BRENDER\V1_2\TEST_A\INC\G386IFIX.H\
	\BRENDER\V1_2\TEST_A\INC\WTCIFIX.H\
	\BRENDER\V1_2\TEST_A\INC\MSCIFIX.H\
	\BRENDER\V1_2\TEST_A\INC\PROIFIX.H\
	\BRENDER\V1_2\TEST_A\INC\VECFNS.H\
	\BRENDER\V1_2\TEST_A\INC\MATH_X.H

$(INTDIR)/MLIBCMNT.OBJ :  $(SOURCE)  $(DEP_MLIBC) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DFWLIB.LIB
# End Source File
################################################################################
# Begin Source File

SOURCE=.\MINIMAL.C
DEP_MINIM=\
	C:\BRENDER\INC\BRENDER.H\
	.\BRWRAP.H\
	C:\BRENDER\INC\COMPILER.H\
	C:\BRENDER\INC\BRLIMITS.H\
	C:\BRENDER\INC\FIXED.H\
	C:\BRENDER\INC\SCALAR.H\
	C:\BRENDER\INC\COLOUR.H\
	C:\BRENDER\INC\MATRIX.H\
	C:\BRENDER\INC\vector.h\
	C:\BRENDER\INC\ANGLES.H\
	C:\BRENDER\INC\QUAT.H\
	C:\BRENDER\INC\TRANSFRM.H\
	C:\BRENDER\INC\ORDER.H\
	C:\BRENDER\INC\PIXELMAP.H\
	C:\BRENDER\INC\BRFONT.H\
	C:\BRENDER\INC\MATERIAL.H\
	C:\BRENDER\INC\MODEL.H\
	C:\BRENDER\INC\ACTOR.H\
	C:\BRENDER\INC\CAMERA.H\
	C:\BRENDER\INC\LIGHT.H\
	C:\BRENDER\INC\POOL.H\
	C:\BRENDER\INC\CUSTSUPT.H\
	C:\BRENDER\INC\BRFILE.H\
	C:\BRENDER\INC\BRDIAG.H\
	C:\BRENDER\INC\BRMEM.H\
	C:\BRENDER\INC\BRERR.H\
	C:\BRENDER\INC\FMT.H\
	C:\BRENDER\INC\FWPROTO.H\
	C:\BRENDER\INC\ZBPROTO.H\
	C:\BRENDER\INC\ZSPROTO.H\
	C:\BRENDER\INC\FWPEXTRA.H\
	C:\BRENDER\INC\VECFNS.H\
	C:\BRENDER\INC\COMPEND.H\
	C:\BRENDER\INC\FMTPROTO.H\
	C:\BRENDER\INC\G386IFIX.H\
	C:\BRENDER\INC\WTCIFIX.H\
	C:\BRENDER\INC\MSCIFIX.H

$(INTDIR)/MINIMAL.OBJ :  $(SOURCE)  $(DEP_MINIM) $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################
