# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Simpview - Win32 Fixed (Debug)
!MESSAGE No configuration specified.  Defaulting to Simpview - Win32 Fixed\
 (Debug).
!ENDIF 

!IF "$(CFG)" != "Simpview - Win32 Fixed (Release)" && "$(CFG)" !=\
 "Simpview - Win32 Fixed (Debug)" && "$(CFG)" !=\
 "Simpview - Win32 Float (Release)" && "$(CFG)" !=\
 "Simpview - Win32 Float (Debug)"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "simp_vc4.mak" CFG="Simpview - Win32 Fixed (Debug)"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simpview - Win32 Fixed (Release)" (based on\
 "Win32 (x86) Application")
!MESSAGE "Simpview - Win32 Fixed (Debug)" (based on "Win32 (x86) Application")
!MESSAGE "Simpview - Win32 Float (Release)" (based on\
 "Win32 (x86) Application")
!MESSAGE "Simpview - Win32 Float (Debug)" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Simpview - Win32 Fixed (Debug)"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"

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

ALL : "$(OUTDIR)\simp_vc4.exe"

CLEAN : 
	-@erase ".\Win32_XR\simp_vc4.exe"
	-@erase ".\Win32_XR\DISPATCH.OBJ"
	-@erase ".\Win32_XR\MLIBCMNT.OBJ"
	-@erase ".\Win32_XR\APP.OBJ"
	-@erase ".\Win32_XR\MAIN.OBJ"
	-@erase ".\Win32_XR\BUFFER.OBJ"
	-@erase ".\Win32_XR\WORLD.OBJ"
	-@erase ".\Win32_XR\APP.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D BASED_FIXED=1 /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /Gz /Zp4 /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D BASED_FIXED=1 /Fp"$(INTDIR)/simp_vc4.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Win32_XR/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/APP.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simp_vc4.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/simp_vc4.pdb" /machine:I386\
 /out:"$(OUTDIR)/simp_vc4.exe" 
LINK32_OBJS= \
	"$(INTDIR)/DISPATCH.OBJ" \
	"$(INTDIR)/MLIBCMNT.OBJ" \
	"$(INTDIR)/APP.OBJ" \
	"$(INTDIR)/MAIN.OBJ" \
	"$(INTDIR)/BUFFER.OBJ" \
	"$(INTDIR)/WORLD.OBJ" \
	"$(INTDIR)/APP.res"

"$(OUTDIR)\simp_vc4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"

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

ALL : "$(OUTDIR)\simp_vc4.exe"

CLEAN : 
	-@erase ".\Win32_XD\vc40.pdb"
	-@erase ".\Win32_XD\vc40.idb"
	-@erase ".\Win32_XD\simp_vc4.exe"
	-@erase ".\Win32_XD\APP.OBJ"
	-@erase ".\Win32_XD\DISPATCH.OBJ"
	-@erase ".\Win32_XD\MAIN.OBJ"
	-@erase ".\Win32_XD\MLIBCMNT.OBJ"
	-@erase ".\Win32_XD\WORLD.OBJ"
	-@erase ".\Win32_XD\BUFFER.OBJ"
	-@erase ".\Win32_XD\APP.res"
	-@erase ".\Win32_XD\simp_vc4.ilk"
	-@erase ".\Win32_XD\simp_vc4.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D BASED_FIXED=1 /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /Gz /Zp4 /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D BASED_FIXED=1 /Fp"$(INTDIR)/simp_vc4.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Win32_XD/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/APP.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simp_vc4.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /debugtype:both /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/simp_vc4.pdb" /debug\
 /debugtype:both /machine:I386 /out:"$(OUTDIR)/simp_vc4.exe" 
LINK32_OBJS= \
	"$(INTDIR)/APP.OBJ" \
	"$(INTDIR)/DISPATCH.OBJ" \
	"$(INTDIR)/MAIN.OBJ" \
	"$(INTDIR)/MLIBCMNT.OBJ" \
	"$(INTDIR)/WORLD.OBJ" \
	"$(INTDIR)/BUFFER.OBJ" \
	"$(INTDIR)/APP.res"

"$(OUTDIR)\simp_vc4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"

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

ALL : "$(OUTDIR)\simp_vc4.exe"

CLEAN : 
	-@erase ".\Win32_FR\simp_vc4.exe"
	-@erase ".\Win32_FR\APP.OBJ"
	-@erase ".\Win32_FR\WORLD.OBJ"
	-@erase ".\Win32_FR\DISPATCH.OBJ"
	-@erase ".\Win32_FR\MLIBCMNT.OBJ"
	-@erase ".\Win32_FR\MAIN.OBJ"
	-@erase ".\Win32_FR\BUFFER.OBJ"
	-@erase ".\Win32_FR\APP.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D BASED_FLOAT=1 /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /Gz /Zp4 /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /D BASED_FLOAT=1 /Fp"$(INTDIR)/simp_vc4.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Win32_FR/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/APP.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simp_vc4.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/simp_vc4.pdb" /machine:I386\
 /out:"$(OUTDIR)/simp_vc4.exe" 
LINK32_OBJS= \
	"$(INTDIR)/APP.OBJ" \
	"$(INTDIR)/WORLD.OBJ" \
	"$(INTDIR)/DISPATCH.OBJ" \
	"$(INTDIR)/MLIBCMNT.OBJ" \
	"$(INTDIR)/MAIN.OBJ" \
	"$(INTDIR)/BUFFER.OBJ" \
	"$(INTDIR)/APP.res"

"$(OUTDIR)\simp_vc4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"

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

ALL : "$(OUTDIR)\simp_vc4.exe"

CLEAN : 
	-@erase ".\Win32_FD\vc40.pdb"
	-@erase ".\Win32_FD\vc40.idb"
	-@erase ".\Win32_FD\simp_vc4.exe"
	-@erase ".\Win32_FD\BUFFER.OBJ"
	-@erase ".\Win32_FD\APP.OBJ"
	-@erase ".\Win32_FD\MAIN.OBJ"
	-@erase ".\Win32_FD\DISPATCH.OBJ"
	-@erase ".\Win32_FD\WORLD.OBJ"
	-@erase ".\Win32_FD\MLIBCMNT.OBJ"
	-@erase ".\Win32_FD\APP.res"
	-@erase ".\Win32_FD\simp_vc4.ilk"
	-@erase ".\Win32_FD\simp_vc4.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D BASED_FLOAT=1 /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /Gz /Zp4 /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D BASED_FLOAT=1 /Fp"$(INTDIR)/simp_vc4.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Win32_FD/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/APP.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simp_vc4.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /debugtype:both /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/simp_vc4.pdb" /debug\
 /debugtype:both /machine:I386 /out:"$(OUTDIR)/simp_vc4.exe" 
LINK32_OBJS= \
	"$(INTDIR)/BUFFER.OBJ" \
	"$(INTDIR)/APP.OBJ" \
	"$(INTDIR)/MAIN.OBJ" \
	"$(INTDIR)/DISPATCH.OBJ" \
	"$(INTDIR)/WORLD.OBJ" \
	"$(INTDIR)/MLIBCMNT.OBJ" \
	"$(INTDIR)/APP.res"

"$(OUTDIR)\simp_vc4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Simpview - Win32 Fixed (Release)"
# Name "Simpview - Win32 Fixed (Debug)"
# Name "Simpview - Win32 Float (Release)"
# Name "Simpview - Win32 Float (Debug)"

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"

!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"

!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"

!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\MLIBCMNT.C
DEP_CPP_MLIBC=\
	{$(INCLUDE)}"\Brender.h"\
	{$(INCLUDE)}"\Compiler.h"\
	{$(INCLUDE)}"\Brlimits.h"\
	{$(INCLUDE)}"\Brfile.h"\
	{$(INCLUDE)}"\Brmem.h"\
	{$(INCLUDE)}"\Brdiag.h"\
	{$(INCLUDE)}"\Brerror.h"\
	{$(INCLUDE)}"\Fixed.h"\
	{$(INCLUDE)}"\Scalar.h"\
	{$(INCLUDE)}"\Colour.h"\
	{$(INCLUDE)}"\Matrix.h"\
	{$(INCLUDE)}"\Angles.h"\
	{$(INCLUDE)}"\Quat.h"\
	{$(INCLUDE)}"\Transfrm.h"\
	{$(INCLUDE)}"\Pixelmap.h"\
	{$(INCLUDE)}"\Brfont.h"\
	{$(INCLUDE)}"\Token.h"\
	{$(INCLUDE)}"\Render.h"\
	{$(INCLUDE)}"\Pool.h"\
	{$(INCLUDE)}"\Custsupt.h"\
	{$(INCLUDE)}"\Order.h"\
	{$(INCLUDE)}"\Fw_p.h"\
	{$(INCLUDE)}"\Math_p.h"\
	{$(INCLUDE)}"\Pm_p.h"\
	"H:\Sam\Brender\Inc\Brv1db.h"\
	{$(INCLUDE)}"\Compend.h"\
	{$(INCLUDE)}"\Pretok.h"\
	"H:\Sam\Brender\Inc\G386ifix.h"\
	{$(INCLUDE)}"\Wtcifix.h"\
	{$(INCLUDE)}"\Mscifix.h"\
	"H:\Sam\Brender\Inc\Proifix.h"\
	{$(INCLUDE)}"\Vecfns.h"\
	{$(INCLUDE)}"\Math_x.h"\
	{$(INCLUDE)}"\Material.h"\
	{$(INCLUDE)}"\Model.h"\
	{$(INCLUDE)}"\Actor.h"\
	{$(INCLUDE)}"\Camera.h"\
	{$(INCLUDE)}"\Light.h"\
	{$(INCLUDE)}"\Fmt.h"\
	{$(INCLUDE)}"\V1db_p.h"\
	{$(INCLUDE)}"\Fmt_p.h"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\MLIBCMNT.OBJ" : $(SOURCE) $(DEP_CPP_MLIBC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\MLIBCMNT.OBJ" : $(SOURCE) $(DEP_CPP_MLIBC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\MLIBCMNT.OBJ" : $(SOURCE) $(DEP_CPP_MLIBC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\MLIBCMNT.OBJ" : $(SOURCE) $(DEP_CPP_MLIBC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\APP.RC
DEP_RSC_APP_R=\
	".\ARGONAUT.ICO"\
	".\dispatch.h"\
	".\APP.H"\
	".\BUFFER.H"\
	".\WORLD.H"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\APP.res" : $(SOURCE) $(DEP_RSC_APP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\APP.res" : $(SOURCE) $(DEP_RSC_APP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\APP.res" : $(SOURCE) $(DEP_RSC_APP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\APP.res" : $(SOURCE) $(DEP_RSC_APP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BUFFER.C
DEP_CPP_BUFFE=\
	".\BUFFER.H"\
	".\APP.H"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\BUFFER.OBJ" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\BUFFER.OBJ" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\BUFFER.OBJ" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\BUFFER.OBJ" : $(SOURCE) $(DEP_CPP_BUFFE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MAIN.C
DEP_CPP_MAIN_=\
	".\APP.H"\
	".\dispatch.h"\
	".\WORLD.H"\
	".\BUFFER.H"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\MAIN.OBJ" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WORLD.C
DEP_CPP_WORLD=\
	{$(INCLUDE)}"\Brender.h"\
	".\dispatch.h"\
	".\WORLD.H"\
	".\BUFFER.H"\
	{$(INCLUDE)}"\Compiler.h"\
	{$(INCLUDE)}"\Brlimits.h"\
	{$(INCLUDE)}"\Brfile.h"\
	{$(INCLUDE)}"\Brmem.h"\
	{$(INCLUDE)}"\Brdiag.h"\
	{$(INCLUDE)}"\Brerror.h"\
	{$(INCLUDE)}"\Fixed.h"\
	{$(INCLUDE)}"\Scalar.h"\
	{$(INCLUDE)}"\Colour.h"\
	{$(INCLUDE)}"\Matrix.h"\
	{$(INCLUDE)}"\Angles.h"\
	{$(INCLUDE)}"\Quat.h"\
	{$(INCLUDE)}"\Transfrm.h"\
	{$(INCLUDE)}"\Pixelmap.h"\
	{$(INCLUDE)}"\Brfont.h"\
	{$(INCLUDE)}"\Token.h"\
	{$(INCLUDE)}"\Render.h"\
	{$(INCLUDE)}"\Pool.h"\
	{$(INCLUDE)}"\Custsupt.h"\
	{$(INCLUDE)}"\Order.h"\
	{$(INCLUDE)}"\Fw_p.h"\
	{$(INCLUDE)}"\Math_p.h"\
	{$(INCLUDE)}"\Pm_p.h"\
	"H:\Sam\Brender\Inc\Brv1db.h"\
	{$(INCLUDE)}"\Compend.h"\
	{$(INCLUDE)}"\Pretok.h"\
	"H:\Sam\Brender\Inc\G386ifix.h"\
	{$(INCLUDE)}"\Wtcifix.h"\
	{$(INCLUDE)}"\Mscifix.h"\
	"H:\Sam\Brender\Inc\Proifix.h"\
	{$(INCLUDE)}"\Vecfns.h"\
	{$(INCLUDE)}"\Math_x.h"\
	{$(INCLUDE)}"\Material.h"\
	{$(INCLUDE)}"\Model.h"\
	{$(INCLUDE)}"\Actor.h"\
	{$(INCLUDE)}"\Camera.h"\
	{$(INCLUDE)}"\Light.h"\
	{$(INCLUDE)}"\Fmt.h"\
	{$(INCLUDE)}"\V1db_p.h"\
	{$(INCLUDE)}"\Fmt_p.h"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\WORLD.OBJ" : $(SOURCE) $(DEP_CPP_WORLD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\WORLD.OBJ" : $(SOURCE) $(DEP_CPP_WORLD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\WORLD.OBJ" : $(SOURCE) $(DEP_CPP_WORLD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\WORLD.OBJ" : $(SOURCE) $(DEP_CPP_WORLD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\APP.C
DEP_CPP_APP_C=\
	{$(INCLUDE)}"\Brender.h"\
	".\APP.H"\
	".\dispatch.h"\
	".\WORLD.H"\
	".\BUFFER.H"\
	{$(INCLUDE)}"\Compiler.h"\
	{$(INCLUDE)}"\Brlimits.h"\
	{$(INCLUDE)}"\Brfile.h"\
	{$(INCLUDE)}"\Brmem.h"\
	{$(INCLUDE)}"\Brdiag.h"\
	{$(INCLUDE)}"\Brerror.h"\
	{$(INCLUDE)}"\Fixed.h"\
	{$(INCLUDE)}"\Scalar.h"\
	{$(INCLUDE)}"\Colour.h"\
	{$(INCLUDE)}"\Matrix.h"\
	{$(INCLUDE)}"\Angles.h"\
	{$(INCLUDE)}"\Quat.h"\
	{$(INCLUDE)}"\Transfrm.h"\
	{$(INCLUDE)}"\Pixelmap.h"\
	{$(INCLUDE)}"\Brfont.h"\
	{$(INCLUDE)}"\Token.h"\
	{$(INCLUDE)}"\Render.h"\
	{$(INCLUDE)}"\Pool.h"\
	{$(INCLUDE)}"\Custsupt.h"\
	{$(INCLUDE)}"\Order.h"\
	{$(INCLUDE)}"\Fw_p.h"\
	{$(INCLUDE)}"\Math_p.h"\
	{$(INCLUDE)}"\Pm_p.h"\
	"H:\Sam\Brender\Inc\Brv1db.h"\
	{$(INCLUDE)}"\Compend.h"\
	{$(INCLUDE)}"\Pretok.h"\
	"H:\Sam\Brender\Inc\G386ifix.h"\
	{$(INCLUDE)}"\Wtcifix.h"\
	{$(INCLUDE)}"\Mscifix.h"\
	"H:\Sam\Brender\Inc\Proifix.h"\
	{$(INCLUDE)}"\Vecfns.h"\
	{$(INCLUDE)}"\Math_x.h"\
	{$(INCLUDE)}"\Material.h"\
	{$(INCLUDE)}"\Model.h"\
	{$(INCLUDE)}"\Actor.h"\
	{$(INCLUDE)}"\Camera.h"\
	{$(INCLUDE)}"\Light.h"\
	{$(INCLUDE)}"\Fmt.h"\
	{$(INCLUDE)}"\V1db_p.h"\
	{$(INCLUDE)}"\Fmt_p.h"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\APP.OBJ" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\APP.OBJ" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\APP.OBJ" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\APP.OBJ" : $(SOURCE) $(DEP_CPP_APP_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DISPATCH.C
DEP_CPP_DISPA=\
	".\dispatch.h"\
	

!IF  "$(CFG)" == "Simpview - Win32 Fixed (Release)"


"$(INTDIR)\DISPATCH.OBJ" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Fixed (Debug)"


"$(INTDIR)\DISPATCH.OBJ" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Release)"


"$(INTDIR)\DISPATCH.OBJ" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Simpview - Win32 Float (Debug)"


"$(INTDIR)\DISPATCH.OBJ" : $(SOURCE) $(DEP_CPP_DISPA) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
