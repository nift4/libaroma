pushd %~dp0
if "%LIBAROMA_BUILD_DRM%" == "1" call drm.cmd
call freetype.cmd
call harfbuzz.cmd
if "%LIBAROMA_BUILD_JPEG%" == "1" call jpeg.cmd
if "%LIBAROMA_BUILD_MINUI%" == "1" call minui.cmd
call minzip.cmd
call png.cmd
call ucdn.cmd
call zlib.cmd
popd