@echo off
setlocal

set UNREAL_EDITOR="C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
set PROJECT_FILE="%CD%\Tag.uproject"

%UNREAL_EDITOR% %PROJECT_FILE% -Deterministic -nosplash -Unattended -nopause -nosound -NullRHI -nocontentbrowser -ini:EditorPerProjectUserSettings:[/Script/UnrealEd.LevelEditorPlaySettings]:PlayNetMode=PIE_ListenServer -ExecCmds="Automation RunTests Project.Functional Tests.Tag.Tests;quit" -StdOut -ReportOutputPath="%CD%\Saved\Automation\Reports" -LogCmds="Global Warning, LogCheckSubobjects Fatal, LogAutomationController Verbose"

endlocal
@echo on