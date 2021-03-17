@echo off
mkdir ..\obj\freetype
cd ..\obj\freetype

echo Compiling freetype
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
 ^
  -DFT2_BUILD_LIBRARY -DFT_CONFIG_OPTION_SUBPIXEL_RENDERING ^
  -DDARWIN_NO_CARBON ^
 ^
  ..\..\..\..\libs\freetype\src\base\ftbbox.c ^
  ..\..\..\..\libs\freetype\src\base\ftbitmap.c ^
  ..\..\..\..\libs\freetype\src\base\ftdebug.c ^
  ..\..\..\..\libs\freetype\src\base\ftfstype.c ^
  ..\..\..\..\libs\freetype\src\base\ftglyph.c ^
  ..\..\..\..\libs\freetype\src\base\ftlcdfil.c ^
  ..\..\..\..\libs\freetype\src\base\ftstroke.c ^
  ..\..\..\..\libs\freetype\src\base\fttype1.c ^
  ..\..\..\..\libs\freetype\src\base\ftbase.c ^
  ..\..\..\..\libs\freetype\src\base\ftsystem.c ^
  ..\..\..\..\libs\freetype\src\base\ftinit.c ^
  ..\..\..\..\libs\freetype\src\base\ftgasp.c ^
  ..\..\..\..\libs\freetype\src\raster\raster.c ^
  ..\..\..\..\libs\freetype\src\sfnt\sfnt.c ^
  ..\..\..\..\libs\freetype\src\smooth\smooth.c ^
  ..\..\..\..\libs\freetype\src\autofit\autofit.c ^
  ..\..\..\..\libs\freetype\src\truetype\truetype.c ^
  ..\..\..\..\libs\freetype\src\cff\cff.c ^
  ..\..\..\..\libs\freetype\src\cid\type1cid.c ^
  ..\..\..\..\libs\freetype\src\bdf\bdf.c ^
  ..\..\..\..\libs\freetype\src\type1\type1.c ^
  ..\..\..\..\libs\freetype\src\type42\type42.c ^
  ..\..\..\..\libs\freetype\src\winfonts\winfnt.c ^
  ..\..\..\..\libs\freetype\src\pcf\pcf.c ^
  ..\..\..\..\libs\freetype\src\pfr\pfr.c ^
  ..\..\..\..\libs\freetype\src\psaux\psaux.c ^
  ..\..\..\..\libs\freetype\src\psnames\psnames.c ^
  ..\..\..\..\libs\freetype\src\pshinter\pshinter.c ^
  ..\..\..\..\libs\freetype\src\sdf\ftsdf.c ^
  ..\..\..\..\libs\freetype\src\sdf\ftbsdf.c ^
  ..\..\..\..\libs\freetype\src\sdf\ftsdfrend.c ^
  ..\..\..\..\libs\freetype\src\gzip\ftgzip.c ^
  ..\..\..\..\libs\freetype\src\lzw\ftlzw.c ^
 ^
  -I..\..\..\..\libs\zlib ^
  -I..\..\..\..\libs\freetype\builds ^
  -I..\..\..\..\libs\harfbuzz-ng\src ^
  -I..\..\..\..\libs\freetype\include ^
  -I..\..\..\..\libs\freetype\src\sdf

cd ..\..\libs
