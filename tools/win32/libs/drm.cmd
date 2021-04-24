@echo off
if not %LIBAROMA_BUILD_DRM% == 1 (
	echo Not needed to build libdrm
	goto :eof
)
pushd %~dp0
mkdir ..\obj\%LIBAROMA_ARCH%\drm
cd ..\obj\%LIBAROMA_ARCH%\drm

echo Compiling libdrm
%LIBAROMA_GCC% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% -DUSE_MMAP ^
 ^
  %LIBAROMA_CFLAGS% ^
 ^
  	%LIBAROMA_BASE%\libs\drm\xf86drm.c ^
  	%LIBAROMA_BASE%\libs\drm\xf86drmMode.c ^
  -I%LIBAROMA_BASE%\libs\drm

popd
