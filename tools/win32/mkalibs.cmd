@echo off

if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if "%1"=="/?" ( goto :showhelp
) else if "%1"=="-h" ( goto :showhelp
) else if "%1"=="--help" ( goto :showhelp
) else if exist "%LIBAROMA_BASE%\tools\win32\libs\%1.cmd" (
	call "%LIBAROMA_BASE%\tools\win32\libs\%1.cmd"
	goto :eof
) else if not "%1"=="" (
	if "%1"=="hb" (
		call "%LIBAROMA_BASE%\tools\win32\libs\harfbuzz.cmd"
		goto :eof
	) else (
	echo Unknown library %1
	goto :eof
	)
)

if "%LIBAROMA_BUILD_DRM%" == "1" call "%LIBAROMA_BASE%\tools\win32\libs\drm.cmd"
call "%LIBAROMA_BASE%\tools\win32\libs\freetype.cmd"
if "%LIBAROMA_BUILD_HARFBUZZ%" == "1" call "%LIBAROMA_BASE%\tools\win32\libs\harfbuzz.cmd"
if "%LIBAROMA_BUILD_JPEG%" == "1" call "%LIBAROMA_BASE%\tools\win32\libs\jpeg.cmd"
if "%LIBAROMA_BUILD_MINUI%" == "1" call "%LIBAROMA_BASE%\tools\win32\libs\minui.cmd"
call "%LIBAROMA_BASE%\tools\win32\libs\minzip.cmd"
call "%LIBAROMA_BASE%\tools\win32\libs\png.cmd"
call "%LIBAROMA_BASE%\tools\win32\libs\ucdn.cmd"
call "%LIBAROMA_BASE%\tools\win32\libs\zlib.cmd"

goto :eof

:showhelp
call %LIBAROMA_BASE%\tools\win32\aromahelp.cmd %~n0
goto :eof
