#!/bin/sh
cd ../obj

echo Compiling minui
$LIBAROMA_GCC -c \
  -save-temps \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -std=c++17 -Wl,-s -DUSE_MMAP \
 \
  $LIBAROMA_CFLAGS \
 \
  	../../../libs/minui/android-stringprintf.cpp \
  	../../../libs/minui/android-strings.cpp \
  	../../../libs/minui/events.cpp \
        ../../../libs/minui/graphics.cpp \
  	../../../libs/minui/graphics_drm.cpp \
  	../../../libs/minui/resources.cpp \
  -I../../../libs/minui \
  -I../../../libs/drm

cd ../libs
