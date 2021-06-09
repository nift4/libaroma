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
set target_name=%1-%LIBAROMA_TARGET_NAME%
if not exist "%LIBAROMA_BASE%\out\bin\%target_name%" (
	echo Could not find %target_name% at bin folder! Maybe it didn't build successfully?
	goto :eof
)
echo Going to run %target_name% on device (recovery mode)
echo Waiting for device in recovery mode (to skip wait press ctrl-c and enter N)
adb wait-for-recovery
adb push "%LIBAROMA_BASE%\out\bin\%target_name%" /tmp
if exist "%LIBAROMA_BASE%\apps\%1\zip\res.zip" (
	adb push "%LIBAROMA_BASE%\apps\%1\zip\res.zip" /tmp/res.zip
)
adb shell chmod 0755 /tmp/%target_name%
adb shell pkill -19 recovery
if %LIBAROMA_DEBUG_ENABLED% == 1 (
	if defined LIBAROMA_GDB (
		echo Pushing GDB to /sbin...
		adb shell mount -o remount,rw /
		adb push %LIBAROMA_GDB% /sbin/gdb
		adb shell chmod 0755 /sbin/gdb
		adb shell mkdir /bin
		adb shell ln -s /sbin/sh /bin/sh
		adb shell gdb -ex run --args /tmp/%target_name% 0 0 /tmp/res.zip
	) else (
		adb shell /tmp/%target_name% 0 0 /tmp/res.zip
	)
) else (
	adb shell /tmp/%target_name% 0 0 /tmp/res.zip
)
adb shell pkill -18 recovery
set "target_name="

goto :eof

:showhelp
call "%LIBAROMA_BASE%\tools\win32\aromahelp.cmd" %~n0
goto :eof
