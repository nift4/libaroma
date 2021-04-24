@echo off
pushd %~dp0
mkdir ..\obj\%LIBAROMA_ARCH%\hb
cd ..\obj\%LIBAROMA_ARCH%\hb

echo Compiling hb
%LIBAROMA_GPP% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
 ^
  -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN -DHAVE_FREETYPE ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-font.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-face.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-math.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-default.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset-input.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-hangul.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-gobject-structs.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-khmer.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-map.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-metrics.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-font.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-aat-map.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-buffer-serialize.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-directwrite.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-number.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-face.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-shape-plan.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-uniscribe.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-cff2-table.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ucd.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-style.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-static.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-coretext.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-syllabic.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-shaper.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-var.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-aat-layout.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-indic.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-glib.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ft.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-myanmar.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-meta.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-tag.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-normalize.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-indic-table.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-fallback.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-buffer.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-cff1-table.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-set.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-icu.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset-cff1.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset-plan.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-common.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset-cff-common.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-color.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-graphite2.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-thai.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset-cff2.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-subset.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-shape.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-gdi.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-fallback-shape.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-blob.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-hebrew.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-draw.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-unicode.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-arabic.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-vowel-constraints.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-layout.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-shape-complex-use.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-map.cc ^
	%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ot-name.cc ^
  -I%LIBAROMA_BASE%\libs\freetype\builds ^
  -I%LIBAROMA_BASE%\libs\freetype\include ^
  -I%LIBAROMA_BASE%\libs\harfbuzz-ng\src ^
  -I%LIBAROMA_BASE%\libs\harfbuzz-ng\src\hb-ucdn

popd
