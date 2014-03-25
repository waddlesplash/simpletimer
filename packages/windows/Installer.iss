#define MyAppName "SimpleTimer"
#define MyAppVerName "SimpleTimer v1.3"
#define MyAppPublisher "waddlesplash"
#define MyAppURL "http://bsimpletimer.sourceforge.net/"
#define MyAppExeName "simpletimer.exe"

#define STFullVer "1.3.0.0"
#define STVer "1.3"
#define STCopyright "2009-2012"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppID={{2AAC9E0F-53AB-40E9-9E21-7E84B05FF01A}
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL=http://sourceforge.net/users/waddlesplash
AppSupportURL=http://sourceforge.net/projects/bsimpletimer/support
AppUpdatesURL=http://sourceforge.net/projects/bsimpletimer/
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
InfoBeforeFile=ChangeLog.txt
OutputBaseFilename=simpletimer-{#STVer}-setup
Compression=lzma2/ultra64
SolidCompression=True
VersionInfoCompany=WaddleSplash
VersionInfoDescription=SimpleTimer Setup
VersionInfoCopyright={#STCopyright}
VersionInfoProductName=SimpleTimer
VersionInfoProductVersion={#STFullVer}
AppCopyright={#STCopyright}
VersionInfoVersion={#STVer}
VersionInfoTextVersion={#STVer}
AllowNoIcons=true
DisableReadyPage=false
AlwaysShowDirOnReadyPage=true
AlwaysShowGroupOnReadyPage=true
ShowLanguageDialog=auto
AppVersion={#STVer}
UninstallDisplayIcon={app}\SimpleTimerLogo.ico
UninstallDisplayName=SimpleTimer
WizardImageFile=Images\InstFront.bmp
WizardSmallImageFile=Images\InstSmallIcon.bmp
InternalCompressLevel=ultra
LicenseFile=License.txt
UsePreviousAppDir=False
MinVersion=0,5.01sp3
SetupIconFile=Images\Installer.ico
OutputDir=Dist-Out\

[Languages]
Name: "English"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Dist\*"; DestDir: "{app}"; Flags: ignoreversion
Source: "Timer\SimpleTimerLogo.ico"; DestDir: "{app}"; DestName: "SimpleTimerLogo.ico"

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}
Name: {group}\{cm:ProgramOnTheWeb,{#MyAppName}}; Filename: {#MyAppURL}
Name: {group}\{cm:UninstallProgram,{#MyAppName}}; Filename: {uninstallexe}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon

[Messages]
ErrorReplacingExistingFile=An error occurred while trying to replace the existing file, perhaps SimpleTimer is running? If this is not the first fime you have seen this message, try rebooting your computer.

[Run]
Filename: "{pf}\Bestimation\SimpleTimer\unins000.exe"; Parameters: "/SILENT"; Flags: skipifdoesntexist; StatusMsg: "Uninstalling previous versions..."
Filename: "{pf}\Bestimation\SimpleTimer\unins001.exe"; Parameters: "/SILENT"; Flags: skipifdoesntexist; StatusMsg: "Uninstalling previous versions..."
Filename: "{pf}\Bestimation\SimpleTimer\unins002.exe"; Parameters: "/SILENT"; Flags: skipifdoesntexist; StatusMsg: "Uninstalling previous versions..."
