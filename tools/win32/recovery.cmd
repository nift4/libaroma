@ECHO OFF
if "%1"=="" (
	echo recovery.cmd - build app and run it on recovery-mode device
	echo Usage: %0 ^<appname^> [r]
	echo.
	echo ^<appname^>    Name of the application to be executed
	echo r            Skip building, push and run previously built app
	goto :eof
)
if "%2"=="r" GOTO run
call app.cmd %1
:run
pushd %~dp0
if not exist bin\%1-%LIBAROMA_ARCH% (
	echo Could not find %1-%LIBAROMA_ARCH% at bin folder! Maybe it didn't build successfully?
	goto :eof
)
echo Going to run %1-%LIBAROMA_ARCH% on device (recovery mode)
echo Waiting for device in recovery mode (to skip wait press ctrl-c and enter N)
adb wait-for-recovery
adb push bin\%1-%LIBAROMA_ARCH% /tmp
rem adb wait-for-recovery
adb push %LIBAROMA_BASE%\examples\%1\zip\res.zip /tmp/res.zip
rem adb wait-for-recovery
adb shell chmod 755 /tmp/%1-%LIBAROMA_ARCH%
rem adb wait-for-recovery
adb shell pkill -19 recovery
rem adb wait-for-recovery
adb shell /tmp/%1-%LIBAROMA_ARCH% 0 0 /tmp/res.zip
rem adb wait-for-recovery
adb shell pkill -18 recovery
popd