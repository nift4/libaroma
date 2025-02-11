@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\hb\" (
	mkdir "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\hb"
)
pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\hb"

echo Building Harfbuzz
%LIBAROMA_GPP% -c ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
  ^
  -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN -DHAVE_FREETYPE ^
  %LIBAROMA_CFLAGS% ^
	^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-font.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-face.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-math.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-default.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset-input.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-hangul.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-gobject-structs.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-khmer.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-map.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-metrics.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-font.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-aat-map.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-buffer-serialize.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-directwrite.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-number.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-face.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-shape-plan.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-uniscribe.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-cff2-table.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ucd.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-style.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-static.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-coretext.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-syllabic.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-shaper.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-var.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-aat-layout.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-indic.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-glib.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ft.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-myanmar.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-meta.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-tag.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-normalize.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-indic-table.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-fallback.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-buffer.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-cff1-table.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-set.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-icu.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset-cff1.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset-plan.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-common.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset-cff-common.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-color.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-graphite2.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-thai.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset-cff2.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-subset.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-shape.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-gdi.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-fallback-shape.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-blob.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-hebrew.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-draw.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-unicode.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-arabic.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-vowel-constraints.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-layout.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-shape-complex-use.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-map.cc" ^
	"%LIBAROMA_BASE%\libs\harfbuzz\src\hb-ot-name.cc" ^
  -I"%LIBAROMA_BASE%\libs\freetype\builds" ^
  -I"%LIBAROMA_BASE%\libs\freetype\include" ^
  -I"%LIBAROMA_BASE%\libs\harfbuzz\src" ^
  -I"%LIBAROMA_BASE%\libs\ucdn"

popd