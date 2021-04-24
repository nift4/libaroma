@echo off
pushd %~dp0
mkdir ..\obj\%LIBAROMA_ARCH%\hbucdn
cd ..\obj\%LIBAROMA_ARCH%\hbucdn

echo Compiling hbucdn
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
 ^
  %LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ucdn\ucdn.c ^
 ^
  -I%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ucdn

popd
