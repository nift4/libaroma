@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

echo Building MinUI
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP ^
  ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\minui\events.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\graphics.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\graphics_drm.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\graphics_fbdev.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\resources.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\deps\stringprintf.cpp" ^
	"%LIBAROMA_BASE%\libs\minui\deps\strings.cpp" ^
  -I"%LIBAROMA_BASE%\libs\minui" ^
  -I"%LIBAROMA_BASE%\libs\png" ^
  -I"%LIBAROMA_BASE%\libs\drm"

popd
