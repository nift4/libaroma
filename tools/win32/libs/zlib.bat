@echo off
mkdir ..\obj\zlib
cd ..\obj\zlib

echo Compiling zlib
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
    ..\..\..\..\libs\zlib\src\adler32.c ^
  	..\..\..\..\libs\zlib\src\compress.c ^
  	..\..\..\..\libs\zlib\src\crc32.c ^
  	..\..\..\..\libs\zlib\src\deflate.c ^
  	..\..\..\..\libs\zlib\src\gzclose.c ^
  	..\..\..\..\libs\zlib\src\gzlib.c ^
  	..\..\..\..\libs\zlib\src\gzread.c ^
  	..\..\..\..\libs\zlib\src\gzwrite.c ^
  	..\..\..\..\libs\zlib\src\infback.c ^
  	..\..\..\..\libs\zlib\src\inflate.c ^
  	..\..\..\..\libs\zlib\src\inftrees.c ^
  	..\..\..\..\libs\zlib\src\inffast.c ^
  	..\..\..\..\libs\zlib\src\trees.c ^
  	..\..\..\..\libs\zlib\src\uncompr.c ^
  	..\..\..\..\libs\zlib\src\zutil.c ^
  -I..\..\..\..\libs\zlib

cd ..\..\libs
