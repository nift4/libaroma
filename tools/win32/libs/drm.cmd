@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

echo Building LibDRM
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP -D_GNU_SOURCE -DMAJOR_IN_SYSMACROS ^
  ^
  %LIBAROMA_CFLAGS% ^
	^
  	"%LIBAROMA_BASE%\libs\drm\xf86drm.c" ^
  	"%LIBAROMA_BASE%\libs\drm\xf86drmMode.c" ^
  -I"%LIBAROMA_BASE%\libs\drm"

popd
