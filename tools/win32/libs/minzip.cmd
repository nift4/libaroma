@echo off
pushd %~dp0
mkdir ..\obj\%LIBAROMA_ARCH%\minzip
cd ..\obj\%LIBAROMA_ARCH%\minzip

echo Compiling minzip
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -Doff64_t=off_t ^
  ^
    %LIBAROMA_BASE%\libs\minzip\Hash.c ^
    %LIBAROMA_BASE%\libs\minzip\SysUtil.c ^
    %LIBAROMA_BASE%\libs\minzip\DirUtil.c ^
    %LIBAROMA_BASE%\libs\minzip\Inlines.c ^
    %LIBAROMA_BASE%\libs\minzip\Zip.c ^
  ^
    -I%LIBAROMA_BASE%\libs\selinux\include ^
    -I%LIBAROMA_BASE%\libs\zlib\src

popd
