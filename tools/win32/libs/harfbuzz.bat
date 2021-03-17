@echo off
mkdir ..\obj\hb
cd ..\obj\hb

echo Compiling hb
%LIBAROMA_GPP% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
 ^
  -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN -DHAVE_FREETYPE ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-font.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-face.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-math.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-default.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset-input.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-hangul.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-gobject-structs.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-khmer.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-map.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-metrics.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-font.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-aat-map.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-buffer-serialize.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-directwrite.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-number.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-face.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-shape-plan.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-uniscribe.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-cff2-table.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ucd.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-style.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-static.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-coretext.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-syllabic.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-shaper.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-var.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-aat-layout.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-indic.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-glib.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ft.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-myanmar.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-meta.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-tag.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-normalize.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-indic-table.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-fallback.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-buffer.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-cff1-table.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-set.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-icu.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset-cff1.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset-plan.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-common.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset-cff-common.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-color.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-graphite2.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-thai.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset-cff2.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-subset.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-shape.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-gdi.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-fallback-shape.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-blob.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-hebrew.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-draw.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-unicode.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-arabic.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-vowel-constraints.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-layout.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-shape-complex-use.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-map.cc ^
	..\..\..\..\libs\harfbuzz-ng\src\hb-ot-name.cc ^
  -I..\..\..\..\libs\freetype\builds ^
  -I..\..\..\..\libs\freetype\include ^
  -I..\..\..\..\libs\harfbuzz-ng\src ^
  -I..\..\..\..\libs\harfbuzz-ng\src\hb-ucdn

cd ..\..\libs
