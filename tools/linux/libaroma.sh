#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
rm -rf libaroma/${LIBAROMA_ARCH}
mkdir -p libaroma/${LIBAROMA_ARCH}
cd libaroma/${LIBAROMA_ARCH}

# build minui wrapper if going to target linux and arch isn't non-neon arm
if [ "${LIBAROMA_PLATFORM}" = "linux" ] && [ "${LIBAROMA_ARCH}" != "arm" ]; then
echo Building aroma minui wrapper
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -lstdc++ \
  -fPIC -DPIC \
 \
  ${LIBAROMA_CFLAGS} \
 \
  ${LIBAROMA_BASE}/src/contrib/platform/${LIBAROMA_PLATFORM}/aroma_minui.cpp \
  -I${LIBAROMA_BASE}/libs/minui \
  -I${LIBAROMA_BASE}/src/contrib/platform/${LIBAROMA_PLATFORM}/include
fi

echo Building libaroma for ${LIBAROMA_ARCH}
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -lstdc++ \
  -fPIC -DPIC \
 \
  ${LIBAROMA_CFLAGS} \
 \
  -DLIBAROMA_CONFIG_DEBUG=$LIBAROMA_CONFIG_DEBUG \
  -DLIBAROMA_CONFIG_DEBUG_FILE=$LIBAROMA_CONFIG_DEBUG_FILE \
  -DLIBAROMA_CONFIG_DEBUG_MEMORY=$LIBAROMA_CONFIG_DEBUG_MEMORY \
  -DLIBAROMA_CONFIG_COMPILER_MESSAGE=$LIBAROMA_CONFIG_COMPILER_MESSAGE \
  -DLIBAROMA_CONFIG_SHMEMFB=$LIBAROMA_CONFIG_SHMEMFB \
 \
  ${LIBAROMA_BASE}/src/aroma/arch/${LIBAROMA_ARCH_OPTIMIZATIONS} \
  ${LIBAROMA_BASE}/src/contrib/platform/${LIBAROMA_PLATFORM}/*.c \
 \
  ${LIBAROMA_BASE}/src/aroma/aroma.c \
  ${LIBAROMA_BASE}/src/aroma/version.c \
  ${LIBAROMA_BASE}/src/aroma/utils/*.c \
  ${LIBAROMA_BASE}/src/aroma/debug/*.c \
  ${LIBAROMA_BASE}/src/aroma/graph/*.c \
  ${LIBAROMA_BASE}/src/aroma/graph/draw/*.c \
  ${LIBAROMA_BASE}/src/aroma/graph/engine/*.c \
  ${LIBAROMA_BASE}/src/aroma/hid/*.c \
  ${LIBAROMA_BASE}/src/aroma/ui/*.c \
  ${LIBAROMA_BASE}/src/aroma/controls/*.c \
  ${LIBAROMA_BASE}/src/aroma/controls/listitem/*.c \
 \
  -I${LIBAROMA_BASE}/include \
  -I${LIBAROMA_BASE}/src/contrib/platform/${LIBAROMA_PLATFORM}/include \
  -I${LIBAROMA_BASE}/src \
  -I${LIBAROMA_BASE}/libs/zlib/src \
  -I${LIBAROMA_BASE}/libs/freetype/builds \
  -I${LIBAROMA_BASE}/libs/freetype/include \
  -I${LIBAROMA_BASE}/libs/minzip \
  -I${LIBAROMA_BASE}/libs/selinux/include \
  -I${LIBAROMA_BASE}/libs/png \
  -I${LIBAROMA_BASE}/libs/jpeg \
  -I${LIBAROMA_BASE}/libs/harfbuzz-ng/src \
  -I${LIBAROMA_BASE}/libs/harfbuzz-ng/src/hb-ucdn \

mkdir -p ${LIBAROMA_BASE}/tools/linux/bin
cd $olddir
