; gtkglut.nsi
;--------------------------------

!define GTKGLUT_NAME           "gtkglut"
!define GTKGLUT_VERSION         0.3.7
!define GTKGLUT_API_VERSION     0.3

!define PREFIX  ".."

; The name of the installer
Name "GtkGLUT"
Caption "GtkGLUT ${GTKGLUT_VERSION} Setup"

; The file to write
OutFile "gtkglut-win32-${GTKGLUT_VERSION}.exe"
SetCompressor lzma

; The default installation directory
InstallDir "c:\gtk"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${GTKGLUT_API_VERSION}" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

LicenseData "${PREFIX}\COPYING"

;---------- Modern UI ----------
!include "MUI.nsh"

Var MUI_TEMP
Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings
;!define MUI_HEADERIMAGE
;!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp" ; optional

!define MUI_ABORTWARNING

!define MUI_COMPONENTSPAGE_SMALLDESC

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE


;Pages

;Welcome Page
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of GtkGLUT, GLUT replacement based on GTK+ and GtkGLExt.\r\n\r\nPlease install GTK+ and DevC++ first, and don't change default paths (ie. c:\gtk and c:\dev-cpp)!\r\n\r\n$_CLICK"
!insertmacro MUI_PAGE_WELCOME

;License Page
!insertmacro MUI_PAGE_LICENSE "${PREFIX}\COPYING"

;Components Page
!insertmacro MUI_PAGE_COMPONENTS

;Directory Page
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page
;!define MUI_STARTMENUPAGE_DEFAULTFOLDER "GtkGLExt ${GTKGLEXT_API_VERSION}"
;!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
;!define MUI_STARTMENUPAGE_REGISTRY_KEY "SOFTWARE\GtkGLExt\${GTKGLEXT_API_VERSION}"
;!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "StartMenuFolder"
;!insertmacro MUI_PAGE_STARTMENU StartMenu $STARTMENU_FOLDER

;Installation Page
!insertmacro MUI_PAGE_INSTFILES

;Finish Page
!define MUI_FINISHPAGE_NOREBOOTSUPPORT
!insertmacro MUI_PAGE_FINISH

;Uninstall Welcome Page
!insertmacro MUI_UNPAGE_WELCOME

;Uninstall Confirm Page
!insertmacro MUI_UNPAGE_CONFIRM

;Uninstallation Page
!insertmacro MUI_UNPAGE_INSTFILES

;Uninstall Finish Page
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "English" ;first language is the default language
!insertmacro MUI_LANGUAGE "Czech" 
; Pages

;Page license
;Page components
;Page directory
;Page instfiles

;UninstPage uninstConfirm
;UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Runtime and developement libraries (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\lib
  
  ; Put file there
;-------------- Main FILES -------------------
  File "${PREFIX}\lib${GTKGLUT_NAME}-${GTKGLUT_VERSION}.dll.a"
  File "${PREFIX}\lib${GTKGLUT_NAME}.a"
  
  SetOutPath $INSTDIR\bin

  File "${PREFIX}\lib${GTKGLUT_NAME}-${GTKGLUT_VERSION}.dll"
;---------------- Documentation -----------------  
  SetOutPath $INSTDIR\share\doc\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}
  
  ; Put file there
  File "${PREFIX}\doc\html\*.*"

;---------------- Include -----------------  
  SetOutPath $INSTDIR\include\GL
  
  ; Put file there
  File "${PREFIX}\include\gl\*.*"

;------------ REGISTRY ----------------  
  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\${GTKGLUT_API_VERSION}" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLUT" "DisplayName" "GtkGLUT ${GTKGLUT_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLUT" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLUT" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLUT" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
SectionEnd

; The stuff to install
Section "Sources"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\src
  File "${PREFIX}\src\*.*"
  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\progs
  File /r "${PREFIX}\progs\*.c"
  File /r "${PREFIX}\progs\*.dat"
  File /r "${PREFIX}\progs\*.h"
  File /r "${PREFIX}\progs\Makefile"
  File /r "${PREFIX}\progs\readme"

  CreateDirectory $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\objs

  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}
  File "${PREFIX}\Makefile"
  File "${PREFIX}\win32-compile.bat"
  File "${PREFIX}\COPYING"
  File "${PREFIX}\README"
  File "${PREFIX}\README.win32"
  File "${PREFIX}\INSTALL"
  File "${PREFIX}\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}.m4" 

  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32
  File "${PREFIX}\win32\*.txt"
  File "${PREFIX}\win32\*.nsi"
  File "${PREFIX}\win32\*.template"

  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc
  File "${PREFIX}\doc\doxygen.cfg"
  File "${PREFIX}\doc\Makefile"
  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc\images
  File "${PREFIX}\doc\images\*.*"
  SetOutPath $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\include\gl
  File "${PREFIX}\include\gl\*.*"
SectionEnd

; The stuff to install
Section "Dev-C++ Integration"
  ; Set output path to the installation directory.
  SetOutPath c:\dev-cpp\templates
  File "${PREFIX}\win32\GtkGLUT.txt"
  File "${PREFIX}\win32\GtkGLUT.template"
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\GtkGLUT"
  CreateShortCut "$SMPROGRAMS\GtkGLUT\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\GtkGLUT\GtkGLUT API.lnk" "$INSTDIR\share\doc\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}\index.html"
  
  IfFileExists "$INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\Makefile" "" +2
      CreateShortCut "$SMPROGRAMS\GtkGLUT\Source directory.lnk" "$INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}"
  
;  CreateShortCut "$SMPROGRAMS\GtkGLUT\Example2 (MakeNSISW).lnk" "$INSTDIR\example2.nsi" "" "$INSTDIR\example2.nsi" 0
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GtkGLUT"
  DeleteRegKey HKLM "Software\${GTKGLUT_API_VERSION}"

  ; Remove files and uninstaller
  Delete $INSTDIR\lib\lib${GTKGLUT_NAME}-${GTKGLUT_VERSION}.dll.a
  Delete $INSTDIR\lib\lib${GTKGLUT_NAME}.a
  Delete $INSTDIR\bin\lib${GTKGLUT_NAME}-${GTKGLUT_VERSION}.dll

  Delete "$INSTDIR\share\doc\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}\*.*"
  RMDir "$INSTDIR\share\doc\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}"

  Delete "$INSTDIR\include\gl\gtkglut.h"
  Delete "$INSTDIR\include\gl\glut.h"
  Delete "$INSTDIR\include\gl\gtkglut_std.h"
  Delete "$INSTDIR\include\gl\gtkglut_ext.h"
  RMDir "$INSTDIR\include\gl"
  
  ; --- sources ----
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\src\*.*
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\src
  RMDir /r $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\progs

  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\objs

  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\Makefile
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32-compile.bat
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\COPYING
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\README
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\README.win32
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\INSTALL
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\${GTKGLUT_NAME}-${GTKGLUT_API_VERSION}.m4

  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32\gtkglut.nsi
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32\GtkGLUT.template
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32\GtkGLUT.txt
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\win32

  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc\doxygen.cfg
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc\Makefile
  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc\images\*.*
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc\images
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\doc

  Delete $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\include\gl\*.*
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\include\gl
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}\include
  RMDir $INSTDIR\share\src\${GTKGLUT_NAME}-${GTKGLUT_VERSION}
  RMDir $INSTDIR\share\src\

  ;Dev C++
  Delete c:\dev-cpp\templates\GtkGLUT.txt
  Delete c:\dev-cpp\templates\GtkGLUT.template

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\GtkGLUT\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\GtkGLUT"
SectionEnd
