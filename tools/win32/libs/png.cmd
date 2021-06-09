@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

echo Building LibPNG
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
  ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\png\png.c" ^
	"%LIBAROMA_BASE%\libs\png\pngerror.c" ^
	"%LIBAROMA_BASE%\libs\png\pngget.c" ^
	"%LIBAROMA_BASE%\libs\png\pngmem.c" ^
	"%LIBAROMA_BASE%\libs\png\pngpread.c" ^
	"%LIBAROMA_BASE%\libs\png\pngread.c" ^
	"%LIBAROMA_BASE%\libs\png\pngrio.c" ^
	"%LIBAROMA_BASE%\libs\png\pngrtran.c" ^
	"%LIBAROMA_BASE%\libs\png\pngrutil.c" ^
	"%LIBAROMA_BASE%\libs\png\pngset.c" ^
	"%LIBAROMA_BASE%\libs\png\pngtrans.c" ^
	"%LIBAROMA_BASE%\libs\png\pngwio.c" ^
	"%LIBAROMA_BASE%\libs\png\pngwrite.c" ^
	"%LIBAROMA_BASE%\libs\png\pngwtran.c" ^
	"%LIBAROMA_BASE%\libs\png\pngwutil.c" ^
  ^
	"%LIBAROMA_BASE%\libs\png\arm\arm_init.c" ^
		"%LIBAROMA_BASE%\libs\png\arm\filter_neon.S" ^
		"%LIBAROMA_BASE%\libs\png\arm\filter_neon_intrinsics.c" ^
		"%LIBAROMA_BASE%\libs\png\arm\palette_neon_intrinsics.c" ^
  ^
  -I"%LIBAROMA_BASE%\libs\png" ^
  -I"%LIBAROMA_BASE%\libs\zlib\src"

popd
