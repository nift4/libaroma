#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir/libaroma/${LIBAROMA_ARCH}

echo Building $1 for ${LIBAROMA_ARCH}
${LIBAROMA_GCC} \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC \
  \
    ${LIBAROMA_CFLAGS} \
  \
    -DLIBAROMA_CONFIG_DEBUG=${LIBAROMA_CONFIG_DEBUG} \
    -DLIBAROMA_CONFIG_DEBUG_FILE=${LIBAROMA_CONFIG_DEBUG_FILE} \
    -DLIBAROMA_CONFIG_DEBUG_MEMORY=${LIBAROMA_CONFIG_DEBUG_MEMORY} \
    -DLIBAROMA_CONFIG_COMPILER_MESSAGE=${LIBAROMA_CONFIG_COMPILER_MESSAGE} \
    -DLIBAROMA_CONFIG_SHMEMFB=${LIBAROMA_CONFIG_SHMEMFB} \
  \
    $LIBAROMA_DRM_OBJ \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/freetype/*.o \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/hb/*.o \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/hbucdn/*.o \
    $LIBAROMA_JPEG_OBJ \
    $LIBAROMA_MINUI_OBJ \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/minzip/*.o \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/png/*.o \
    ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/zlib/*.o \
    ${LIBAROMA_BASE}/tools/linux/libaroma/${LIBAROMA_ARCH}/*.o \
    ${LIBAROMA_BASE}/examples/$1/*.c \
  \
  -I${LIBAROMA_BASE}/include \
  -I${LIBAROMA_BASE}/libs/selinux/include \
  -I${LIBAROMA_BASE}/src \
  -I${LIBAROMA_BASE}/libs/selinux/include \
  -I${LIBAROMA_BASE}/examples/$1 \
  -o ${LIBAROMA_BASE}/tools/linux/bin/$1-${LIBAROMA_ARCH} \
  \
  -lm -lpthread -lstdc++ -lrt ${LIBAROMA_ADDITIONAL_LIBS}

cd $olddir
