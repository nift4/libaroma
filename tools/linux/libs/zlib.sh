#!/bin/sh
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/zlib
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/zlib

echo Compiling zlib
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -DUSE_MMAP \
 \
  ${LIBAROMA_CFLAGS} \
 \
    ${LIBAROMA_BASE}/libs/zlib/src/adler32.c \
  	${LIBAROMA_BASE}/libs/zlib/src/compress.c \
  	${LIBAROMA_BASE}/libs/zlib/src/crc32.c \
  	${LIBAROMA_BASE}/libs/zlib/src/deflate.c \
  	${LIBAROMA_BASE}/libs/zlib/src/gzclose.c \
  	${LIBAROMA_BASE}/libs/zlib/src/gzlib.c \
  	${LIBAROMA_BASE}/libs/zlib/src/gzread.c \
  	${LIBAROMA_BASE}/libs/zlib/src/gzwrite.c \
  	${LIBAROMA_BASE}/libs/zlib/src/infback.c \
  	${LIBAROMA_BASE}/libs/zlib/src/inflate.c \
  	${LIBAROMA_BASE}/libs/zlib/src/inftrees.c \
  	${LIBAROMA_BASE}/libs/zlib/src/inffast.c \
  	${LIBAROMA_BASE}/libs/zlib/src/trees.c \
  	${LIBAROMA_BASE}/libs/zlib/src/uncompr.c \
  	${LIBAROMA_BASE}/libs/zlib/src/zutil.c \
  -I${LIBAROMA_BASE}/libs/zlib

cd ${LIBAROMA_BASE}/tools/linux/libs
