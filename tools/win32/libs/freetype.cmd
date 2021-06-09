@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%~n0"

if "%LIBAROMA_BUILD_HARFBUZZ%" == "1" (
	echo Building Freetype (Harfbuzz enhanced^)
) else echo Building Freetype
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
  ^
  -DFT2_BUILD_LIBRARY -DFT_CONFIG_OPTION_SUBPIXEL_RENDERING ^
  -DDARWIN_NO_CARBON %LIBAROMA_FREETYPE_CFLAGS% ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftbbox.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftbitmap.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftdebug.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftfstype.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftglyph.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftlcdfil.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftstroke.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\fttype1.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftbase.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftsystem.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftinit.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\base\ftgasp.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\raster\raster.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\sfnt\sfnt.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\smooth\smooth.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\autofit\autofit.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\truetype\truetype.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\cff\cff.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\cid\type1cid.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\bdf\bdf.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\type1\type1.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\type42\type42.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\winfonts\winfnt.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\pcf\pcf.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\pfr\pfr.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\psaux\psaux.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\psnames\psnames.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\pshinter\pshinter.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\sdf\ftsdf.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\sdf\ftbsdf.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\sdf\ftsdfrend.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\gzip\ftgzip.c" ^
	"%LIBAROMA_BASE%\libs\freetype\src\lzw\ftlzw.c" ^
	^
  -I"%LIBAROMA_BASE%\libs\zlib" ^
  -I"%LIBAROMA_BASE%\libs\harfbuzz\src" ^
  -I"%LIBAROMA_BASE%\libs\freetype\builds" ^
  -I"%LIBAROMA_BASE%\libs\freetype\include" ^
  -I"%LIBAROMA_BASE%\libs\freetype\src\sdf"

popd
