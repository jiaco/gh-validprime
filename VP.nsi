!include "Library.nsh"
!include "MUI2.nsh"

Name ValidPrime

OutFile "ValidPrime_installer_v0.9.3.exe"

InstallDir $PROGRAMFILES\GeneHuggers\ValidPrime

InstallDirRegKey HKLM "Software\GeneHuggers\ValidPrime" "Install_Dir"

RequestExecutionLevel admin

Var ALREADY_INSTALLED

;----------------------------------------------

;Page components
;Page directory
;Page instfiles

!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

;UninstPage uninstConfirm
;UninstPage instfiles

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

;----------------------------------------------

!define MINGW_BIN "C:\MinGW\bin"
!define QT_BIN "C:\Qt\4.7.3\bin"

!define	GCC_DLL	"libgcc_s_dw2-1.dll"
!define	GCC_LIB '"${QT_BIN}\${GCC_DLL}"'
!define	CPP_DLL	"libstdc++-6.dll"
!define	CPP_LIB '"${MINGW_BIN}\${CPP_DLL}"'

!define	MGW_DLL	"mingwm10.dll"
!define	MGW_LIB '"${QT_BIN}\${MGW_DLL}"'

!define	QTC_DLL "QtCore4.dll"
!define	QTC_LIB '"${QT_BIN}\${QTC_DLL}"'
!define	QTG_DLL "QtGui4.dll"
!define	QTG_LIB '"${QT_BIN}\${QTG_DLL}"'


!define	GH_VER "5.9.05"
!define	GH_DLL '${GH_VER}.dll'
;!define	GH_LIBS "C:\Gh"
!define	GH_LIBS "C:\Gh"

;!define	MYSQL_DLL "libmysql.dll"
;!define	QMYSQL_DLL "qsqlmysql4.dll"
;!define	QSQLITE_DLL "qsqlite4.dll"
;!define	QODBC_DLL "qsqlodbc4.dll"

;!define	MYSQL_LIB '"${GH_LIBS}\${MYSQL_DLL}"'
;!define	QMYSQL_LIB '"${GH_LIBS}\${QMYSQL_DLL}"'
;!define	QSQLITE_LIB '"${GH_LIBS}\${QSQLITE_DLL}"'
;!define	QODBC_LIB '"${GH_LIBS}\${QODBC_DLL}"'

!define	GHC_DLL 'GhCore.${GH_DLL}'
!define	GHC_LIB '"${GH_LIBS}\${GHC_DLL}"'

!define	GHB_DLL 'GhBio.${GH_DLL}'
!define	GHB_LIB '"${GH_LIBS}\${GHB_DLL}"'

!define	GHG_DLL 'GhGui.${GH_DLL}'
!define	GHG_LIB '"${GH_LIBS}\${GHG_DLL}"'

!define	GHS_DLL 'GhSalsa.${GH_DLL}'
!define	GHS_LIB '"${GH_LIBS}\${GHS_DLL}"'


SectionGroup	"Dependencies"

;	Section "-"
;		IfFileExists "$INSTDIR\ValidPrime.exe" 0 new_install
;			StrCpy $ALREADY_INSTALLED 1
;		new_install:
;	SectionEnd

	Section	"MinGW"
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${GCC_LIB} $SYSDIR\${GCC_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${CPP_LIB} $SYSDIR\${CPP_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${MGW_LIB} $SYSDIR\${MGW_DLL} $SYSDIR
	SectionEnd

	Section "Qt DLLs"
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${QTC_LIB} $SYSDIR\${QTC_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${QTG_LIB} $SYSDIR\${QTG_DLL} $SYSDIR
	SectionEnd

;	Section "SQL DLLs"
;		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${MYSQL_LIB} $SYSDIR\${MYSQL_DLL} $SYSDIR
;		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${QMYSQL_LIB} $SYSDIR\${QMYSQL_DLL} $SYSDIR
;		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${QSQLITE_LIB} $SYSDIR\${QSQLITE_DLL} $SYSDIR
;		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${QODBC_LIB} $SYSDIR\${QODBC_DLL} $SYSDIR
;	SectionEnd


	Section "GH DLLs"
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${GHC_LIB} $SYSDIR\${GHC_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${GHB_LIB} $SYSDIR\${GHB_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${GHG_LIB} $SYSDIR\${GHG_DLL} $SYSDIR
		!insertmacro InstallLib REGDLL $ALREADY_INSTALLED NOREBOOT_PROTECTED ${GHS_LIB} $SYSDIR\${GHS_DLL} $SYSDIR
	SectionEnd

SectionGroupEnd

Section "GeneHuggers"
;	SectionIn RO
	SetOutPath $INSTDIR

	File "release\ValidPrime.exe"

	WriteRegStr HKLM SOFTWARE\GeneHuggers\ValidPrime "Install_Dir" "$INSTDIR"
	
;
;	presume we need a subdir in the registry after genehuggers for each app

	WriteRegStr HKLM \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\GeneHuggers" \
	"DisplayName" "ValidPrime"
	WriteRegStr HKLM \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\GeneHuggers" \
	"UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegDWORD HKLM \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\GeneHuggers" \
	"NoModify" 1
	WriteRegDWORD HKLM \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\GeneHuggers" \
	"NoRepair" 1

	WriteUninstaller "uninstall.exe"
SectionEnd

Section "Start Menu Shortcuts"
	CreateDirectory "$SMPROGRAMS\GeneHuggers"
	CreateShortCut "$SMPROGRAMS\GeneHuggers\ValidPrime.lnk" \
	"$INSTDIR\ValidPrime.exe" "" "$INSTDIR\ValidPrime.exe" 0
	CreateShortCut "$SMPROGRAMS\GeneHuggers\Uninstall.lnk" \
	"$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd

Section "Uninstall"

	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${GCC_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${CPP_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${MGW_DLL}

	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${QTC_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${QTG_DLL}
	
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${GHC_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${GHB_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${GHG_DLL}
	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${GHS_DLL}

;	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${MYSQL_DLL}
;	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${QMYSQL_DLL}
;	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${QSQLITE_DLL}
;	!insertmacro UninstallLib REGDLL SHARED NOREBOOT_PROTECTED $SYSDIR\${QODBC_DLL}

	DeleteRegKey HKLM \
	"Software\Microsoft\Windows\CurrentVersion\Uninstall\GeneHuggers"
	DeleteRegKey HKLM SOFTWARE\GeneHuggers\ValidPrime

	Delete $INSTDIR\ValidPrime.exe
	Delete $INSTDIR\uninstall.exe

	Delete "$SMPROGRAMS\GeneHuggers\ValidPrime\*.*"

	RMDir "$SMPROGRAMS\GeneHuggers\ValidPrime"
	RMDir "$INSTDIR"
SectionEnd
