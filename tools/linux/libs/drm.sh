#!/bin/sh

if [ $LIBAROMA_BUILD_DRM != 1 ]; then
	echo No need to build libdrm
	exit
fi

mkdir -p ../obj/drm
cd ../obj/drm

echo Compiling libdrm
$LIBAROMA_GCC -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -DUSE_MMAP \
 \
  $LIBAROMA_CFLAGS \
 \
  	../../../../libs/drm/xf86drm.c \
  	../../../../libs/drm/xf86drmMode.c \
  -I../../../../libs/drm

cd ../../libs
