@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if "%1"=="/?" ( goto :showhelp
) else if "%1"=="-h" ( goto :showhelp
) else if "%1"=="--help" ( goto :showhelp
)

if "%1" == "all" (
rem delete all built targets
	echo Clearing built files for all targets
	if exist "%LIBAROMA_BASE%\out\libs\" rmdir /S /Q "%LIBAROMA_BASE%\out\libs"
) else if not "%1" == "" (
rem delete built files for specified library
	echo Clearing built files for %1 library (%LIBAROMA_TARGET_NAME%^)
	if exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%1\" rmdir /S /Q "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%1"
) else (
rem by default, delete built files for current architecture 
	echo Clearing built files for %LIBAROMA_TARGET_NAME%
	if exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\" rmdir /S /Q "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%"
)

goto :eof

:showhelp
call %LIBAROMA_BASE%\tools\win32\aromahelp.cmd %~n0
goto :eof