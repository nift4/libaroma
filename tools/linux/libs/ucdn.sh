#!/bin/sh
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/hbucdn
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/hbucdn

echo Compiling hbucdn
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s \
 \
  ${LIBAROMA_BASE}/libs/harfbuzz-ng/src/hb-ucdn/ucdn.c \
 \
  -I${LIBAROMA_BASE}/libs/harfbuzz-ng/src/hb-ucdn

cd ${LIBAROMA_BASE}/tools/linux/libs
