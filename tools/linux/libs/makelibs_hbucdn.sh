#!/bin/sh
mkdir -p ../obj/hbucdn
cd ../obj/hbucdn

echo Compiling hbucdn
$LIBAROMA_GCC -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s \
 \
  ../../../../libs/harfbuzz-ng/src/hb-ucdn/ucdn.c \
 \
  -I../../../../libs/harfbuzz-ng/src/hb-ucdn

cd ../../libs
