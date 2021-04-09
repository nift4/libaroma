#!/bin/sh

if [ ${LIBAROMA_BUILD_DRM} != 1 ]; then
	echo No need to build libdrm
	exit
fi

mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/drm
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/drm

echo Compiling libdrm
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -DUSE_MMAP \
 \
  ${LIBAROMA_CFLAGS} \
 \
  	${LIBAROMA_BASE}/libs/drm/xf86drm.c \
  	${LIBAROMA_BASE}/libs/drm/xf86drmMode.c \
  -I${LIBAROMA_BASE}/libs/drm

cd ${LIBAROMA_BASE}/tools/linux/libs
