; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Lumatone Editor"
#define MyAppVersion "1.0.1"
#define MyAppPublisher "Lumatone"
#define MyAppURL "https://www.lumatone.io/"
#define MyAppExeName "LumatoneSetup.exe"
#define MyAppExeNameDest "Lumatone Editor.exe"
#define MappingAssocName MyAppName + " Mapping"
#define MappingAssocExt ".ltn"
#define MappingAssocKey StringChange(MappingAssocName, " ", "") + MappingAssocExt
#define PaletteAssocName MyAppName + " Palette"
#define PaletteAssocExt ".ltp"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{3D5A3856-6C68-4D61-BC30-716BBB2DB155}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AllowNoIcons=yes
DefaultDirName={autopf64}\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
;PrivilegesRequiredOverridesAllowed=dialog
OutputDir=.\
OutputBaseFilename=Install {#MyAppName} {#MyAppVersion}
Compression=lzma
SolidCompression=yes
UsePreviousAppDir=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]                                                         
Source: "..\..\Builds\VisualStudio2019\x64\Release\App\{#MyAppExeName}"; DestDir: "{app}"; DestName: "{#MyAppExeNameDest}"; Flags: ignoreversion
Source: "..\..\Presets\Mappings\*"; DestDir: "{userdocs}\{#MyAppName}\Mappings"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\..\Presets\Palettes\*"; DestDir: "{userdocs}\{#MyAppName}\Palettes"; Flags: ignoreversion recursesubdirs createallsubdirs uninsneveruninstall
Source: "..\..\Libraries\win64\bin\libssh2-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\Libraries\win64\bin\libcrypto-1_1-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\BinaryData\Assets\PNG\ltn.ico"; DestDir:"{app}"; Flags: ignoreversion
Source: "..\..\BinaryData\Assets\PNG\ltp.ico"; DestDir:"{app}"; Flags: ignoreversion
Source: "vc_redist.x64.exe"; DestDir: "{tmp}"; Flags: ignoreversion deleteafterinstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKA; Subkey: "Software\Classes\{#MappingAssocExt}\OpenWithProgIds"; ValueType: string; ValueName: "{#MappingAssocKey}"; ValueData: "%"; Flags: uninsdeletevalue
Root: HKA; Subkey: "Software\Classes\{#MappingAssocExt}"; ValueType: string; ValueName: ""; ValueData: "{#MappingAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MappingAssocExt}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\ltn.ico"
Root: HKA; Subkey: "Software\Classes\{#MappingAssocExt}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeNameDest}"" ""%1"""
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeNameDest}\SupportedTypes"; ValueType: string; ValueName: {#MappingAssocExt}; ValueData: ""
Root: HKA; Subkey: "Software\Classes\{#PaletteAssocExt}"; ValueType: string; ValueName: ""; ValueData: "{#PaletteAssocName}"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#PaletteAssocExt}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData:"{app}\ltp.ico"

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeNameDest}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeNameDest}"; Tasks: desktopicon

[Code]
// Pulled from https://stackoverflow.com/questions/24574035/how-to-install-microsoft-vc-redistributables-silently-in-inno-setup
// Version 14 will be fine for this application
function VCRedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if RegQueryStringValue(HKEY_LOCAL_MACHINE,
       'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Version', Version) then
  begin
    // Is the installed version at least 14.14 ? 
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.14.26429.03')<0);
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;
  if (Result) then
  begin
    ExtractTemporaryFile('vc_redist.x64.exe');
  end;
end;

[Run]
Filename: "{tmp}\vc_redist.x64.exe"; Description: "Install VC Redistributable package if necessary"; Parameters: "/q /norestart /c:""msiexec /q /i vcredist.msi"""; \
   Check: VCRedistNeedsInstall; StatusMsg: "Installing Microsoft Visual C++ 2015, may take a few minutes..."
Filename: "{app}\{#MyAppExeNameDest}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

