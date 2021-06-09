@echo off
if not exist "%~dp0\tools\win32\_aroma_envsetup.cmd" (
	echo fatal error: win32 build tools not accessible!
	goto :eof
)

setlocal enableextensions enabledelayedexpansion

set _LIBAROMA_ENVSETUP_ROOT=%~dp0
set _LIBAROMA_ENVSETUP_ROOT=%_LIBAROMA_ENVSETUP_ROOT:~0,-1%
call "%~dp0\tools\win32\_aroma_envsetup.cmd" %1
set "_LIBAROMA_ENVSETUP_ROOT="

rem add commands to path for current environment
set PATH=%PATH%;%LIBAROMA_BASE%\tools\win32

rem detect open by double click
rem echo %cmdcmdline%|find /i """%~f0""">nul && cmd /k ""
rem disabled because there is something causing variables setting not to become available
cmd.exe /k title Libaroma%LIBAROMA_DEBUG_SUFFIX% build environment