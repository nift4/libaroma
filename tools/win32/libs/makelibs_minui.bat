@ECHO OFF
cd ..\obj

echo Compiling MinUI
%LIBAROMA_GCC% -c ^
  -save-temps ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -Wl,-s -Doff64_t=__off64_t^
  ^
    ../../../libs/minui/events.cpp ^
    ../../../libs/minui/graphics.cpp ^
    ../../../libs/minui/graphics_drm.cpp ^
    ../../../libs/minui/resources.cpp ^
  ^
	-I../../../libs/minui %LIBAROMA_CINCLUDES%

cd ..\libs

if "%1"=="-f" GOTO DONE
pause
:DONE
