@echo off
mkdir ..\obj\minui
cd ..\obj\minui

echo Compiling minui
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  	..\..\..\..\libs\minui\events.cpp ^
    ..\..\..\..\libs\minui\graphics.cpp ^
  	..\..\..\..\libs\minui\graphics_drm.cpp ^
  	..\..\..\..\libs\minui\graphics_fbdev.cpp ^
  	..\..\..\..\libs\minui\resources.cpp ^
  -I..\..\..\..\libs\minui ^
  -I..\..\..\..\libs\png ^
  -I..\..\..\..\libs\drm

cd ..\..\libs
