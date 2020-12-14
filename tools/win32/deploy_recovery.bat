@ECHO OFF
cls
if "%2"=="r" GOTO run
call build_app.bat %1
:run
echo Waiting for device (recovery)
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% push bin\%1 /tmp
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% push ..\..\examples\%1\zip\res.zip /sdcard/
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% shell chmod 755 /tmp/%1
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% shell killall -19 recovery
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% shell /tmp/%1 0 0 /sdcard/res.zip
%LIBAROMA_ADB% wait-for-recovery
%LIBAROMA_ADB% shell killall -18 recovery
