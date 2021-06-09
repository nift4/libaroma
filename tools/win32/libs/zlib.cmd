@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

echo Building ZLib
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP ^
  ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\zlib\src\adler32.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\compress.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\crc32.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\deflate.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\gzclose.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\gzlib.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\gzread.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\gzwrite.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\infback.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\inflate.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\inftrees.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\inffast.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\trees.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\uncompr.c" ^
	"%LIBAROMA_BASE%\libs\zlib\src\zutil.c" ^
  -I"%LIBAROMA_BASE%\libs\zlib"

popd
