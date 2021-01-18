#!/bin/sh
cd ../obj

echo Compiling minzip
$LIBAROMA_GCC -c \
  -save-temps \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s -Doff64_t=__off64_t \
  \
    ../../../libs/minzip/Hash.c \
    ../../../libs/minzip/SysUtil.c \
    ../../../libs/minzip/DirUtil.c \
    ../../../libs/minzip/Inlines.c \
    ../../../libs/minzip/Zip.c \
  \
    -I../../../libs/selinux/include \
    -I../../../libs/zlib/src

cd ../libs
