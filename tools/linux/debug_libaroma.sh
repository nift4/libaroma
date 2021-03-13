#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
rm -rf libaroma
mkdir -p libaroma
cd libaroma

echo \(DEBUG\) Building aroma minui wrapper

$LIBAROMA_GCC -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -lstdc++ \
  -fPIC -DPIC -g \
 \
  $LIBAROMA_DEBUG_CFLAGS \
 \
  ../../../src/contrib/platform/linux/aroma_minui.cpp \
  -I../../../libs/minui \
  -I../../../src/contrib/platform/linux/include

echo \(DEBUG\) Building libaroma
$LIBAROMA_GCC -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -lstdc++ -g \
  -fPIC -DPIC \
 \
  $LIBAROMA_DEBUG_CFLAGS \
 \
  -DLIBAROMA_CONFIG_DEBUG=$LIBAROMA_CONFIG_DEBUG \
  -DLIBAROMA_CONFIG_DEBUG_FILE=$LIBAROMA_CONFIG_DEBUG_FILE \
  -DLIBAROMA_CONFIG_DEBUG_MEMORY=$LIBAROMA_CONFIG_DEBUG_MEMORY \
  -DLIBAROMA_CONFIG_COMPILER_MESSAGE=$LIBAROMA_CONFIG_COMPILER_MESSAGE \
  -DLIBAROMA_CONFIG_SHMEMFB=$LIBAROMA_CONFIG_SHMEMFB \
  -DANDROID=1 -D__ANDROID__ \
 \
  ../../../src/contrib/platform/linux/fb_driver.c \
  ../../../src/contrib/platform/linux/hid_driver.c \
  ../../../src/contrib/platform/linux/platform.c \
 \
  ../../../src/aroma/aroma.c \
  ../../../src/aroma/version.c \
  ../../../src/aroma/utils/*.c \
  ../../../src/aroma/debug/*.c \
  ../../../src/aroma/graph/*.c \
  ../../../src/aroma/graph/draw/*.c \
  ../../../src/aroma/graph/engine/*.c \
  ../../../src/aroma/hid/*.c \
  ../../../src/aroma/ui/*.c \
  ../../../src/aroma/controls/*.c \
  ../../../src/aroma/controls/listitem/*.c \
 \
  -I../../../include \
  -I../../../src/contrib/platform/linux/include \
  -I../../../src \
  -I../../../libs/zlib/src \
  -I../../../libs/freetype/builds \
  -I../../../libs/freetype/include \
  -I../../../libs/minzip \
  -I../../../libs/selinux/include \
  -I../../../libs/png \
  -I../../../libs/jpeg \
  -I../../../libs/harfbuzz-ng/src \
  -I../../../libs/harfbuzz-ng/src/hb-ucdn \

cd ..

mkdir -p bin
cd $olddir
