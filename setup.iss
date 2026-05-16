; Programa: Studentu Projektas v3.0
; Autorius:Mykolas Skaisgiris
; Versija:3.0

#define MyAppName "Studentu Projektas"
#define MyAppVersion "3.0"
#define MyAppPublisher "VU"
#define MyAppURL "https://github.com/BrotatoTornado/Stud_Uzd2/tree/v3.0"
#define MyAppExeName "programa_manoVector.exe"
#define MyAppFolder "VU\Mykolas Skaisgiris"

[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

; Diegimo katalogas
DefaultDirName={autopf}\{#MyAppFolder}
DefaultGroupName={#MyAppFolder}
AllowNoIcons=no

; Išdiegimo palaikymas
Uninstallable=yes
UninstallDisplayName={#MyAppName} {#MyAppVersion}
UninstallDisplayIcon={app}\{#MyAppExeName}
CreateUninstallRegKey=yes

; Administratoriaus teisės (privaloma)
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=

; Išvesties diegimo failo parametrai
OutputDir=installer\output
OutputBaseFilename=StudentuProjektasV3_Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
WizardResizable=no

; Versijų informacija
VersionInfoVersion={#MyAppVersion}.0
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName} {#MyAppVersion} diegimas
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}.0

; Licencija (neprivaloma – jei yra LICENSE failas)
; LicenseFile=LICENSE.txt

; Pageidaujamas diegimo aplankas Windows versijoms
MinVersion=10.0

[Languages]
Name: "lithuanian"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Nuorodų kūrimo parinktys
Name: "desktopicon";     Description: "Sukurti nuorodą darbalaukyje";   GroupDescription: "Papildomos nuorodos:"; Flags: unchecked
Name: "startmenuicon";   Description: "Sukurti nuorodą Start meniu";     GroupDescription: "Papildomos nuorodos:"; Flags: checkedonce

[Files]
; Pagrindinis vykdomasis failas
Source: "programa_manoVector.exe"; DestDir: "{app}"; Flags: ignoreversion

; Papildoma: std::vector versija palyginimui
Source: "programa_vector.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

Source: "programa_list.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

Source: "programa_deque.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

; Spartos analizė
Source: "spartos_analize.exe"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

; Testavimo failai
Source: "studfailai\studentai100000.txt";   DestDir: "{app}"; Flags: ignoreversion
Source: "studfailai\studentai1000000.txt";  DestDir: "{app}"; Flags: ignoreversion
Source: "studfailai\studentai10000000.txt";  DestDir: "{app}"; Flags: ignoreversion

; Dokumentacija
Source: "docs\html\*"; DestDir: "{app}\docs"; Flags: ignoreversion recursesubdirs skipifsourcedoesntexist

; README
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Icons]
; Start meniu nuorodos (pagrindinė programa)
Name: "{group}\{#MyAppName}";                                Filename: "{app}\{#MyAppExeName}"; Tasks: startmenuicon
Name: "{group}\Spartos analize";                             Filename: "{app}\spartos_analize.exe"; Tasks: startmenuicon; Flags: dontcloseonexit
;Name: "{group}\Dokumentacija";                               Filename: "{app}\docs\index.html"; Tasks: startmenuicon;
Name: "{group}\Pasalinti {#MyAppName}";                      Filename: "{uninstallexe}"

; Darbastalio nuoroda
Name: "{autodesktop}\{#MyAppName}";                          Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
; Paleisti programą po diegimo (neprivaloma)
Filename: "{app}\{#MyAppExeName}"; \
    Description: "Paleisti {#MyAppName} dabar"; \
    Flags: nowait postinstall skipifsilent shellexec runascurrentuser

[UninstallDelete]
; Išvalyti sugeneruotus failus
Type: filesandordirs; Name: "{app}"
Type: filesandordirs; Name: "{app}\docs"
Type: files;          Name: "{app}\*.md"
Type: files;          Name: "{app}\*.txt"

[Registry]
; Programos registracija (administrator teisės)
Root: HKLM; \
    Subkey: "Software\{#MyAppPublisher}\{#MyAppName}"; \
    ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; \
    Flags: uninsdeletekey

Root: HKLM; \
    Subkey: "Software\{#MyAppPublisher}\{#MyAppName}"; \
    ValueType: string; ValueName: "Version"; ValueData: "{#MyAppVersion}"; \
    Flags: uninsdeletekey

; Manifestas administratoriaus teisėms (pridedamas prie exe)
; Pastaba: alternatyviai naudokite mt.exe manifest įrankį
Root: HKLM; \
    Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; \
    ValueType: string; \
    ValueName: "{app}\{#MyAppExeName}"; \
    ValueData: "RUNASADMIN"; \
    Flags: uninsdeletevalue

[Messages]
; Lietuviški pranešimai
WelcomeLabel1=Sveiki atvykę į [name] diegimo vedlį
WelcomeLabel2=Ši programa įdiegs [name/ver] į Jūsų kompiuterį.%n%nRekomenduojame uždaryti visas kitas programas prieš tęsiant diegimą.
FinishedLabel=Diegimas baigtas. [name] sėkmingai įdiegta.
UninstallAppFullTitle=Šalinti {#MyAppName} {#MyAppVersion}
