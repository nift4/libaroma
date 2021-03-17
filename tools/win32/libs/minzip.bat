@echo off
mkdir ..\obj\minzip
cd ..\obj\minzip

echo Compiling minzip
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -Doff64_t=off_t ^
  ^
    ..\..\..\..\libs\minzip\Hash.c ^
    ..\..\..\..\libs\minzip\SysUtil.c ^
    ..\..\..\..\libs\minzip\DirUtil.c ^
    ..\..\..\..\libs\minzip\Inlines.c ^
    ..\..\..\..\libs\minzip\Zip.c ^
  ^
    -I..\..\..\..\libs\selinux\include ^
    -I..\..\..\..\libs\zlib\src

cd ..\..\libs
