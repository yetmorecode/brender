# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Win32 Fixed (Debug)
!MESSAGE No configuration specified.  Defaulting to Win32 Fixed (Debug).
!ENDIF 

!IF "$(CFG)" != "Win32 Fixed (Release)" && "$(CFG)" != "Win32 Fixed (Debug)" &&\
 "$(CFG)" != "Win32 Float (Release)" && "$(CFG)" != "Win32 Float (Debug)"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simpview.mak" CFG="Win32 Fixed (Debug)"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Fixed (Release)" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Fixed (Debug)" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Float (Release)" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Float (Debug)" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Fixed (Debug)"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Fixed (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_XR"
# PROP Intermediate_Dir "Win32_XR"
OUTDIR=.\Win32_XR
INTDIR=.\Win32_XR

ALL : $(OUTDIR)/Simpview.exe $(OUTDIR)/Simpview.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D BASED_FIXED=1 /FR /c
CPP_PROJ=/nologo /Gz /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D BASED_FIXED=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Simpview.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\Win32_XR/
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo$(INTDIR)/"APP.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Simpview.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Mlibcmnt.sbr \
	$(INTDIR)/BUFFER.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/World.sbr \
	$(INTDIR)/App.sbr \
	$(INTDIR)/DISPATCH.SBR

$(OUTDIR)/Simpview.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:no\
 /PDB:$(OUTDIR)/"Simpview.pdb" /MACHINE:I386 /OUT:$(OUTDIR)/"Simpview.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Mlibcmnt.obj \
	$(INTDIR)/APP.res \
	$(INTDIR)/BUFFER.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/World.obj \
	$(INTDIR)/App.obj \
	$(INTDIR)/DISPATCH.OBJ

$(OUTDIR)/Simpview.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_XD"
# PROP Intermediate_Dir "Win32_XD"
OUTDIR=.\Win32_XD
INTDIR=.\Win32_XD

ALL : $(OUTDIR)/Simpview.exe $(OUTDIR)/Simpview.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D BASED_FIXED=1 /FR /c
CPP_PROJ=/nologo /Gz /Zp4 /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D BASED_FIXED=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Simpview.pch"\
 /Fo$(INTDIR)/ /Fd$(OUTDIR)/"Simpview.pdb" /c 
CPP_OBJS=.\Win32_XD/
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo$(INTDIR)/"APP.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Simpview.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Mlibcmnt.sbr \
	$(INTDIR)/BUFFER.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/World.sbr \
	$(INTDIR)/App.sbr \
	$(INTDIR)/DISPATCH.SBR

$(OUTDIR)/Simpview.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /DEBUGTYPE:both /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"Simpview.pdb" /DEBUG /DEBUGTYPE:both /MACHINE:I386\
 /OUT:$(OUTDIR)/"Simpview.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Mlibcmnt.obj \
	$(INTDIR)/APP.res \
	$(INTDIR)/BUFFER.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/World.obj \
	$(INTDIR)/App.obj \
	$(INTDIR)/DISPATCH.OBJ

$(OUTDIR)/Simpview.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Win32_Fl"
# PROP BASE Intermediate_Dir "Win32_Fl"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32_FR"
# PROP Intermediate_Dir "Win32_FR"
OUTDIR=.\Win32_FR
INTDIR=.\Win32_FR

ALL : $(OUTDIR)/Simpview.exe $(OUTDIR)/Simpview.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D BASED_FLOAT=1 /FR /c
CPP_PROJ=/nologo /Gz /Zp4 /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D BASED_FLOAT=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Simpview.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\Win32_FR/
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo$(INTDIR)/"APP.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Simpview.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Mlibcmnt.sbr \
	$(INTDIR)/BUFFER.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/World.sbr \
	$(INTDIR)/App.sbr \
	$(INTDIR)/DISPATCH.SBR

$(OUTDIR)/Simpview.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:no\
 /PDB:$(OUTDIR)/"Simpview.pdb" /MACHINE:I386 /OUT:$(OUTDIR)/"Simpview.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Mlibcmnt.obj \
	$(INTDIR)/APP.res \
	$(INTDIR)/BUFFER.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/World.obj \
	$(INTDIR)/App.obj \
	$(INTDIR)/DISPATCH.OBJ

$(OUTDIR)/Simpview.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32_F0"
# PROP BASE Intermediate_Dir "Win32_F0"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Win32_FD"
# PROP Intermediate_Dir "Win32_FD"
OUTDIR=.\Win32_FD
INTDIR=.\Win32_FD

ALL : $(OUTDIR)/Simpview.exe $(OUTDIR)/Simpview.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D BASED_FLOAT=1 /FR /c
CPP_PROJ=/nologo /Gz /Zp4 /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D BASED_FLOAT=1 /FR$(INTDIR)/ /Fp$(OUTDIR)/"Simpview.pch"\
 /Fo$(INTDIR)/ /Fd$(OUTDIR)/"Simpview.pdb" /c 
CPP_OBJS=.\Win32_FD/
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo$(INTDIR)/"APP.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"Simpview.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Mlibcmnt.sbr \
	$(INTDIR)/BUFFER.SBR \
	$(INTDIR)/MAIN.SBR \
	$(INTDIR)/World.sbr \
	$(INTDIR)/App.sbr \
	$(INTDIR)/DISPATCH.SBR

$(OUTDIR)/Simpview.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /DEBUGTYPE:both /MACHINE:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib /NOLOGO /SUBSYSTEM:windows /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"Simpview.pdb" /DEBUG /DEBUGTYPE:both /MACHINE:I386\
 /OUT:$(OUTDIR)/"Simpview.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Mlibcmnt.obj \
	$(INTDIR)/APP.res \
	$(INTDIR)/BUFFER.OBJ \
	$(INTDIR)/MAIN.OBJ \
	$(INTDIR)/World.obj \
	$(INTDIR)/App.obj \
	$(INTDIR)/DISPATCH.OBJ

$(OUTDIR)/Simpview.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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

SOURCE=.\Mlibcmnt.c
DEP_MLIBC=\
	h:\sam\brender\inc\brender.h\
	h:\sam\brender\inc\compiler.h\
	h:\sam\brender\inc\brlimits.h\
	h:\sam\brender\inc\brfile.h\
	h:\sam\brender\inc\brmem.h\
	h:\sam\brender\inc\brdiag.h\
	h:\sam\brender\inc\brerror.h\
	h:\sam\brender\inc\fixed.h\
	h:\sam\brender\inc\scalar.h\
	h:\sam\brender\inc\colour.h\
	h:\sam\brender\inc\matrix.h\
	h:\sam\brender\inc\vector.h\
	h:\sam\brender\inc\angles.h\
	h:\sam\brender\inc\quat.h\
	h:\sam\brender\inc\transfrm.h\
	h:\sam\brender\inc\pixelmap.h\
	h:\sam\brender\inc\brfont.h\
	h:\sam\brender\inc\token.h\
	h:\sam\brender\inc\render.h\
	h:\sam\brender\inc\pool.h\
	h:\sam\brender\inc\custsupt.h\
	h:\sam\brender\inc\order.h\
	h:\sam\brender\inc\fw_p.h\
	h:\sam\brender\inc\math_p.h\
	h:\sam\brender\inc\pm_p.h\
	h:\sam\brender\inc\brv1db.h\
	h:\sam\brender\inc\compend.h\
	h:\sam\brender\inc\pretok.h\
	h:\sam\brender\inc\g386ifix.h\
	h:\sam\brender\inc\wtcifix.h\
	h:\sam\brender\inc\mscifix.h\
	h:\sam\brender\inc\proifix.h\
	h:\sam\brender\inc\vecfns.h\
	h:\sam\brender\inc\math_x.h\
	h:\sam\brender\inc\material.h\
	h:\sam\brender\inc\model.h\
	h:\sam\brender\inc\actor.h\
	h:\sam\brender\inc\camera.h\
	h:\sam\brender\inc\light.h\
	h:\sam\brender\inc\fmt.h\
	h:\sam\brender\inc\v1db_p.h\
	h:\sam\brender\inc\fmt_p.h

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/Mlibcmnt.obj :  $(SOURCE)  $(DEP_MLIBC) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/Mlibcmnt.obj :  $(SOURCE)  $(DEP_MLIBC) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/Mlibcmnt.obj :  $(SOURCE)  $(DEP_MLIBC) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/Mlibcmnt.obj :  $(SOURCE)  $(DEP_MLIBC) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\APP.RC
DEP_APP_R=\
	.\ARGONAUT.ICO\
	.\APP.H\
	.\BUFFER.H\
	.\WORLD.H

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/APP.res :  $(SOURCE)  $(DEP_APP_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/APP.res :  $(SOURCE)  $(DEP_APP_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/APP.res :  $(SOURCE)  $(DEP_APP_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/APP.res :  $(SOURCE)  $(DEP_APP_R) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BUFFER.C
DEP_BUFFE=\
	.\BUFFER.H\
	.\APP.H

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/BUFFER.OBJ :  $(SOURCE)  $(DEP_BUFFE) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/BUFFER.OBJ :  $(SOURCE)  $(DEP_BUFFE) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/BUFFER.OBJ :  $(SOURCE)  $(DEP_BUFFE) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/BUFFER.OBJ :  $(SOURCE)  $(DEP_BUFFE) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MAIN.C
DEP_MAIN_=\
	.\APP.H\
	.\WORLD.H\
	.\BUFFER.H

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/MAIN.OBJ :  $(SOURCE)  $(DEP_MAIN_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/MAIN.OBJ :  $(SOURCE)  $(DEP_MAIN_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/MAIN.OBJ :  $(SOURCE)  $(DEP_MAIN_) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/MAIN.OBJ :  $(SOURCE)  $(DEP_MAIN_) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\World.c
DEP_WORLD=\
	h:\sam\brender\inc\brender.h\
	.\WORLD.H\
	.\BUFFER.H\
	h:\sam\brender\inc\compiler.h\
	h:\sam\brender\inc\brlimits.h\
	h:\sam\brender\inc\brfile.h\
	h:\sam\brender\inc\brmem.h\
	h:\sam\brender\inc\brdiag.h\
	h:\sam\brender\inc\brerror.h\
	h:\sam\brender\inc\fixed.h\
	h:\sam\brender\inc\scalar.h\
	h:\sam\brender\inc\colour.h\
	h:\sam\brender\inc\matrix.h\
	h:\sam\brender\inc\vector.h\
	h:\sam\brender\inc\angles.h\
	h:\sam\brender\inc\quat.h\
	h:\sam\brender\inc\transfrm.h\
	h:\sam\brender\inc\pixelmap.h\
	h:\sam\brender\inc\brfont.h\
	h:\sam\brender\inc\token.h\
	h:\sam\brender\inc\render.h\
	h:\sam\brender\inc\pool.h\
	h:\sam\brender\inc\custsupt.h\
	h:\sam\brender\inc\order.h\
	h:\sam\brender\inc\fw_p.h\
	h:\sam\brender\inc\math_p.h\
	h:\sam\brender\inc\pm_p.h\
	h:\sam\brender\inc\brv1db.h\
	h:\sam\brender\inc\compend.h\
	h:\sam\brender\inc\pretok.h\
	h:\sam\brender\inc\g386ifix.h\
	h:\sam\brender\inc\wtcifix.h\
	h:\sam\brender\inc\mscifix.h\
	h:\sam\brender\inc\proifix.h\
	h:\sam\brender\inc\vecfns.h\
	h:\sam\brender\inc\math_x.h\
	h:\sam\brender\inc\material.h\
	h:\sam\brender\inc\model.h\
	h:\sam\brender\inc\actor.h\
	h:\sam\brender\inc\camera.h\
	h:\sam\brender\inc\light.h\
	h:\sam\brender\inc\fmt.h\
	h:\sam\brender\inc\v1db_p.h\
	h:\sam\brender\inc\fmt_p.h

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/World.obj :  $(SOURCE)  $(DEP_WORLD) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/World.obj :  $(SOURCE)  $(DEP_WORLD) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/World.obj :  $(SOURCE)  $(DEP_WORLD) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/World.obj :  $(SOURCE)  $(DEP_WORLD) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\App.c
DEP_APP_C=\
	h:\sam\brender\inc\brender.h\
	.\APP.H\
	.\WORLD.H\
	.\BUFFER.H\
	h:\sam\brender\inc\compiler.h\
	h:\sam\brender\inc\brlimits.h\
	h:\sam\brender\inc\brfile.h\
	h:\sam\brender\inc\brmem.h\
	h:\sam\brender\inc\brdiag.h\
	h:\sam\brender\inc\brerror.h\
	h:\sam\brender\inc\fixed.h\
	h:\sam\brender\inc\scalar.h\
	h:\sam\brender\inc\colour.h\
	h:\sam\brender\inc\matrix.h\
	h:\sam\brender\inc\vector.h\
	h:\sam\brender\inc\angles.h\
	h:\sam\brender\inc\quat.h\
	h:\sam\brender\inc\transfrm.h\
	h:\sam\brender\inc\pixelmap.h\
	h:\sam\brender\inc\brfont.h\
	h:\sam\brender\inc\token.h\
	h:\sam\brender\inc\render.h\
	h:\sam\brender\inc\pool.h\
	h:\sam\brender\inc\custsupt.h\
	h:\sam\brender\inc\order.h\
	h:\sam\brender\inc\fw_p.h\
	h:\sam\brender\inc\math_p.h\
	h:\sam\brender\inc\pm_p.h\
	h:\sam\brender\inc\brv1db.h\
	h:\sam\brender\inc\compend.h\
	h:\sam\brender\inc\pretok.h\
	h:\sam\brender\inc\g386ifix.h\
	h:\sam\brender\inc\wtcifix.h\
	h:\sam\brender\inc\mscifix.h\
	h:\sam\brender\inc\proifix.h\
	h:\sam\brender\inc\vecfns.h\
	h:\sam\brender\inc\math_x.h\
	h:\sam\brender\inc\material.h\
	h:\sam\brender\inc\model.h\
	h:\sam\brender\inc\actor.h\
	h:\sam\brender\inc\camera.h\
	h:\sam\brender\inc\light.h\
	h:\sam\brender\inc\fmt.h\
	h:\sam\brender\inc\v1db_p.h\
	h:\sam\brender\inc\fmt_p.h

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/App.obj :  $(SOURCE)  $(DEP_APP_C) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/App.obj :  $(SOURCE)  $(DEP_APP_C) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/App.obj :  $(SOURCE)  $(DEP_APP_C) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/App.obj :  $(SOURCE)  $(DEP_APP_C) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DISPATCH.C

!IF  "$(CFG)" == "Win32 Fixed (Release)"

$(INTDIR)/DISPATCH.OBJ :  $(SOURCE)  $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Fixed (Debug)"

$(INTDIR)/DISPATCH.OBJ :  $(SOURCE)  $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Release)"

$(INTDIR)/DISPATCH.OBJ :  $(SOURCE)  $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Float (Debug)"

$(INTDIR)/DISPATCH.OBJ :  $(SOURCE)  $(INTDIR)

!ENDIF 

# End Source File
# End Group
# End Project
################################################################################
