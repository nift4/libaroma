@ECHO OFF
cls
cd bin
adb wait-for-device
if "%2"=="r" GOTO run
call build_app.bat %1
:run
%LIBAROMA_ADB% shell su -c mount -o rw,remount /
%LIBAROMA_ADB% shell su -c mkdir /tmp
%LIBAROMA_ADB% push %1 /sdcard/
%LIBAROMA_ADB% shell su -c cp /sdcard/%1 /tmp/
%LIBAROMA_ADB% shell su -c chmod 755 /tmp/%1
%LIBAROMA_ADB% shell su -c killall -19 system_server
%LIBAROMA_ADB% shell su -c killall -19 surfaceflinger
%LIBAROMA_ADB% shell su -c sleep 1
%LIBAROMA_ADB% shell su -c /tmp/%1
%LIBAROMA_ADB% shell su -c sleep 1
%LIBAROMA_ADB% shell su -c killall -18 surfaceflinger
%LIBAROMA_ADB% shell su -c killall -18 system_server
cd ..
