@ECHO OFF

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if "%1"=="" ( goto :showhelp
) else if "%1"=="/?" ( goto :showhelp
) else if "%1"=="-h" ( goto :showhelp
) else if "%1"=="--help" ( goto :showhelp
)
if "%2"=="r" goto run
call "%LIBAROMA_BASE%\tools\win32\mkapp.cmd" %1
if %errorlevel% NEQ 0 echo Build error detected, not running app && goto :eof

:run
set target_name=%1-%LIBAROMA_TARGET_NAME%.exe
if not exist "%LIBAROMA_BASE%\out\bin\%target_name%" (
	echo Could not find %target_name% at bin folder! Maybe it didn't build successfully?
	goto :eof
)
echo Going to run %target_name%
if %LIBAROMA_DEBUG_ENABLED% == 1 (
	if defined LIBAROMA_GDB (
		"%LIBAROMA_GDB%" -ex run --args "%LIBAROMA_BASE%\out\bin\%target_name%" "%LIBAROMA_BASE%\apps\%1\zip\res.zip"
	) else (
		"%LIBAROMA_BASE%\out\bin\%target_name%" "%LIBAROMA_BASE%\apps\%1\zip\res.zip"
	)
) else (
	"%LIBAROMA_BASE%\out\bin\%target_name%" "%LIBAROMA_BASE%\apps\%1\zip\res.zip"
)
set "target_name="

goto :eof

:showhelp
call "%LIBAROMA_BASE%\tools\win32\aromahelp.cmd" %~n0
goto :eof
