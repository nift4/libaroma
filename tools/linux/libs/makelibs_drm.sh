#!/bin/sh
cd ../obj

echo Compiling drm
$LIBAROMA_GCC -c \
  -save-temps \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -DUSE_MMAP \
 \
  $LIBAROMA_CFLAGS \
 \
  	../../../libs/drm/xf86drm.c \
    ../../../libs/drm/xf86drmMode.c \
  -I../../../libs/drm

cd ../libs
