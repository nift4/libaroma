@echo off 
if not %LIBAROMA_BUILD_JPEG% == 1 (
	echo Not needed to build jpeg
	goto :eof
)
pushd %~dp0

mkdir ..\obj\%LIBAROMA_ARCH%\jpeg
cd ..\obj\%LIBAROMA_ARCH%\jpeg

echo compiling jpeg (NEON optimized)
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  -DAVOID_TABLES ^
  -DNV_ARM_NEON ^
  -DANDROID_TILE_BASED_DECODE ^
 ^
  %LIBAROMA_BASE%\libs\jpeg\jcapimin.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcapistd.c ^
  %LIBAROMA_BASE%\libs\jpeg\jccoefct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jccolor.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcdctmgr.c ^
  %LIBAROMA_BASE%\libs\jpeg\jchuff.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcinit.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcmainct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcmarker.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcmaster.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcomapi.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcparam.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcphuff.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcprepct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jcsample.c ^
  %LIBAROMA_BASE%\libs\jpeg\jctrans.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdapimin.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdapistd.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdatadst.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdatasrc.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdcoefct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdcolor.c ^
  %LIBAROMA_BASE%\libs\jpeg\jddctmgr.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdhuff.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdinput.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdmainct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdmarker.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdmaster.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdmerge.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdphuff.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdpostct.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdsample.c ^
  %LIBAROMA_BASE%\libs\jpeg\jdtrans.c ^
  %LIBAROMA_BASE%\libs\jpeg\jerror.c ^
  %LIBAROMA_BASE%\libs\jpeg\jfdctflt.c ^
  %LIBAROMA_BASE%\libs\jpeg\jfdctfst.c ^
  %LIBAROMA_BASE%\libs\jpeg\jfdctint.c ^
  %LIBAROMA_BASE%\libs\jpeg\jidctflt.c ^
  %LIBAROMA_BASE%\libs\jpeg\jidctfst.c ^
  %LIBAROMA_BASE%\libs\jpeg\jidctint.c ^
  %LIBAROMA_BASE%\libs\jpeg\jidctred.c ^
  %LIBAROMA_BASE%\libs\jpeg\jquant1.c ^
  %LIBAROMA_BASE%\libs\jpeg\jquant2.c ^
  %LIBAROMA_BASE%\libs\jpeg\jutils.c ^
  %LIBAROMA_BASE%\libs\jpeg\jmemmgr.c ^
  ^
  ^
  %LIBAROMA_BASE%\libs\jpeg\jsimd_arm_neon.S ^
  %LIBAROMA_BASE%\libs\jpeg\jsimd_neon.c ^
  %LIBAROMA_BASE%\libs\jpeg\jmem-android.c ^
  ^
  ^
  -I%LIBAROMA_BASE%\libs\jpeg
cd ..\..\..\libs
popd
