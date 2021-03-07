@ECHO OFF
cd ..\obj

echo Compiling MinUI
%LIBAROMA_GCC% -c ^
  -save-temps ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -Wl,-s -DUSE_MMAP ^
  ^
    ../../../libs/minui/events.cpp ^
    ../../../libs/minui/graphics.cpp ^
    ../../../libs/minui/graphics_drm.cpp ^
    ../../../libs/minui/graphics_fbdev.cpp ^
    ../../../libs/minui/resources.cpp ^
  ^
	-I../../../libs/minui ^
	-I../../../libs/drm ^
	-I../../../libs/png %LIBAROMA_CINCLUDES%

cd ..\libs

if "%1"=="-f" GOTO DONE
pause
:DONE
