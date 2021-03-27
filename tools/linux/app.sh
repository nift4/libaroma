#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir/libaroma

echo Building $1
$LIBAROMA_GCC \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC \
  \
    $LIBAROMA_CFLAGS \
  \
    -DLIBAROMA_CONFIG_DEBUG=$LIBAROMA_CONFIG_DEBUG \
    -DLIBAROMA_CONFIG_DEBUG_FILE=$LIBAROMA_CONFIG_DEBUG_FILE \
    -DLIBAROMA_CONFIG_DEBUG_MEMORY=$LIBAROMA_CONFIG_DEBUG_MEMORY \
    -DLIBAROMA_CONFIG_COMPILER_MESSAGE=$LIBAROMA_CONFIG_COMPILER_MESSAGE \
    -DLIBAROMA_CONFIG_SHMEMFB=$LIBAROMA_CONFIG_SHMEMFB \
  \
    $LIBAROMA_DRM_OBJ \
    ../obj/freetype/*.o \
    ../obj/hb/*.o \
    ../obj/hbucdn/*.o \
    $LIBAROMA_JPEG_OBJ \
    $LIBAROMA_MINUI_OBJ \
    ../obj/minzip/*.o \
    ../obj/png/*.o \
    ../obj/zlib/*.o \
    ../libaroma/*.o \
    ../../../examples/$1/*.c \
  \
  -I../../../include \
  -I../../../libs/selinux/include \
  -I../../../src \
  -o ../bin/$1 \
  \
  -lm -lpthread -lstdc++ -lrt $LIBAROMA_ADDITIONAL_LIBS
cd $olddir
