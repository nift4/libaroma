#!/bin/sh

if [ ${LIBAROMA_BUILD_MINUI} != 1 ]; then
	echo No need to build minui
	exit
fi

mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/minui
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/minui

echo Compiling minui
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -DUSE_MMAP -fpermissive -w \
 \
  ${LIBAROMA_CFLAGS} \
 \
  	${LIBAROMA_BASE}/libs/minui/events.cpp \
    ${LIBAROMA_BASE}/libs/minui/graphics.cpp \
  	${LIBAROMA_BASE}/libs/minui/graphics_drm.cpp \
  	${LIBAROMA_BASE}/libs/minui/graphics_fbdev.cpp \
  	${LIBAROMA_BASE}/libs/minui/resources.cpp \
  	${LIBAROMA_BASE}/libs/minui/deps/strings.cpp \
  	${LIBAROMA_BASE}/libs/minui/deps/stringprintf.cpp \
  -I${LIBAROMA_BASE}/libs/minui \
  -I${LIBAROMA_BASE}/libs/png \
  -I${LIBAROMA_BASE}/libs/drm

cd ${LIBAROMA_BASE}/tools/linux/libs
