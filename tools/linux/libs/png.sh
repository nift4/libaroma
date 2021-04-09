#!/bin/sh
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/png
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/png

echo Compiling png
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s \
 \
  ${LIBAROMA_CFLAGS} \
 \
    ${LIBAROMA_BASE}/libs/png/png.c \
  	${LIBAROMA_BASE}/libs/png/pngerror.c \
  	${LIBAROMA_BASE}/libs/png/pngget.c \
  	${LIBAROMA_BASE}/libs/png/pngmem.c \
  	${LIBAROMA_BASE}/libs/png/pngpread.c \
  	${LIBAROMA_BASE}/libs/png/pngread.c \
  	${LIBAROMA_BASE}/libs/png/pngrio.c \
  	${LIBAROMA_BASE}/libs/png/pngrtran.c \
  	${LIBAROMA_BASE}/libs/png/pngrutil.c \
  	${LIBAROMA_BASE}/libs/png/pngset.c \
  	${LIBAROMA_BASE}/libs/png/pngtrans.c \
  	${LIBAROMA_BASE}/libs/png/pngwio.c \
  	${LIBAROMA_BASE}/libs/png/pngwrite.c \
  	${LIBAROMA_BASE}/libs/png/pngwtran.c \
  	${LIBAROMA_BASE}/libs/png/pngwutil.c \
  \
  	${LIBAROMA_BASE}/libs/png/arm/arm_init.c \
		${LIBAROMA_BASE}/libs/png/arm/filter_neon.S \
		${LIBAROMA_BASE}/libs/png/arm/filter_neon_intrinsics.c \
		${LIBAROMA_BASE}/libs/png/arm/palette_neon_intrinsics.c \
  \
  -I${LIBAROMA_BASE}/libs/png \
  -I${LIBAROMA_BASE}/libs/zlib

cd ${LIBAROMA_BASE}/tools/linux/libs
