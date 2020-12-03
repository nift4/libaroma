@ECHO OFF
cls
if "%2"=="r" GOTO run
call build_app.bat %1
:run
%LIBAROMA_ADB% push bin\%1 /tmp
%LIBAROMA_ADB% shell chmod 755 /tmp/%1
%LIBAROMA_ADB% shell killall -19 recovery
%LIBAROMA_ADB% shell /tmp/%1
%LIBAROMA_ADB% shell killall -18 recovery
