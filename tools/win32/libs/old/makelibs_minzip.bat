@ECHO OFF
cd ..\obj

echo Compiling MinZip
%LIBAROMA_GCC% -c ^
  -save-temps ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -Wl,-s -Doff64_t=off_t^
  ^
    ../../../libs/minzip/Hash.c ^
    ../../../libs/minzip/SysUtil.c ^
    ../../../libs/minzip/DirUtil.c ^
    ../../../libs/minzip/Inlines.c ^
    ../../../libs/minzip/Zip.c ^
  ^
	-I../../../libs/selinux/include ^
    -I../../../libs/zlib/src %LIBAROMA_CINCLUDES%

cd ..\libs

if "%1"=="-f" GOTO DONE
pause
:DONE
