#!/bin/sh
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/freetype
cd ${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/freetype

echo Compiling freetype
${LIBAROMA_GCC} -c \
  -fdata-sections -ffunction-sections -Wl,--gc-sections \
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG \
  -fPIC -DPIC -Wl,-s\
 \
  -DFT2_BUILD_LIBRARY -DFT_CONFIG_OPTION_SUBPIXEL_RENDERING \
  -DDARWIN_NO_CARBON \
 \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftbbox.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftbitmap.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftdebug.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftfstype.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftglyph.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftlcdfil.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftstroke.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/fttype1.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftbase.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftsystem.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftinit.c \
  ${LIBAROMA_BASE}/libs/freetype/src/base/ftgasp.c \
  ${LIBAROMA_BASE}/libs/freetype/src/raster/raster.c \
  ${LIBAROMA_BASE}/libs/freetype/src/sfnt/sfnt.c \
  ${LIBAROMA_BASE}/libs/freetype/src/smooth/smooth.c \
  ${LIBAROMA_BASE}/libs/freetype/src/autofit/autofit.c \
  ${LIBAROMA_BASE}/libs/freetype/src/truetype/truetype.c \
  ${LIBAROMA_BASE}/libs/freetype/src/cff/cff.c \
  ${LIBAROMA_BASE}/libs/freetype/src/cid/type1cid.c \
  ${LIBAROMA_BASE}/libs/freetype/src/bdf/bdf.c \
  ${LIBAROMA_BASE}/libs/freetype/src/type1/type1.c \
  ${LIBAROMA_BASE}/libs/freetype/src/type42/type42.c \
  ${LIBAROMA_BASE}/libs/freetype/src/winfonts/winfnt.c \
  ${LIBAROMA_BASE}/libs/freetype/src/pcf/pcf.c \
  ${LIBAROMA_BASE}/libs/freetype/src/pfr/pfr.c \
  ${LIBAROMA_BASE}/libs/freetype/src/psaux/psaux.c \
  ${LIBAROMA_BASE}/libs/freetype/src/psnames/psnames.c \
  ${LIBAROMA_BASE}/libs/freetype/src/pshinter/pshinter.c \
  ${LIBAROMA_BASE}/libs/freetype/src/sdf/ftsdf.c \
  ${LIBAROMA_BASE}/libs/freetype/src/sdf/ftbsdf.c \
  ${LIBAROMA_BASE}/libs/freetype/src/sdf/ftsdfrend.c \
  ${LIBAROMA_BASE}/libs/freetype/src/gzip/ftgzip.c \
  ${LIBAROMA_BASE}/libs/freetype/src/lzw/ftlzw.c \
 \
  -I${LIBAROMA_BASE}/libs/zlib \
  -I${LIBAROMA_BASE}/libs/freetype/builds \
  -I${LIBAROMA_BASE}/libs/harfbuzz-ng/src \
  -I${LIBAROMA_BASE}/libs/freetype/include \
  -I${LIBAROMA_BASE}/libs/freetype/src/sdf

cd ${LIBAROMA_BASE}/tools/linux/libs
