@ECHO OFF
cls
if "%2"=="r" GOTO run
call build_app.bat %1
:run
echo Waiting for device...
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c mount -o rw,remount /
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c mkdir /tmp
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% push bin\%1 /sdcard/
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% push ..\..\examples\%1\zip\res.zip /sdcard/
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c cp /sdcard/%1 /tmp/
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c chmod 755 /tmp/%1
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c killall -19 system_server
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c killall -19 surfaceflinger
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c /tmp/%1
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c killall -18 surfaceflinger
%LIBAROMA_ADB% wait-for-device
%LIBAROMA_ADB% shell su -c killall -18 system_server