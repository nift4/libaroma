@ECHO OFF
cd ..\obj

echo Compiling MinUI
%LIBAROMA_GCC% -c ^
  -save-temps ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -Wl,-s -Doff64_t=off_t^
  ^
  	../../../libs/drm/xf86drm.c ^
    ../../../libs/drm/xf86drmMode.c ^
  -I../../../libs/drm %LIBAROMA_CINCLUDES%

cd ..\libs

if "%1"=="-f" GOTO DONE
pause
:DONE
