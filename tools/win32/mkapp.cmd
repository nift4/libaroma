@echo off
if not defined LIBAROMA_BASE (
	echo Libaroma environment not set, please run envsetup.cmd first.
	goto :eof
)

if "%1"=="/?" ( goto :showhelp
) else if "%1"=="-h" ( goto :showhelp
) else if "%1"=="--help" ( goto :showhelp
)

setlocal enableextensions enabledelayedexpansion

set "tried_lib= "
:retry_lib_missing
set "missing_lib="
for %%L in (aroma drm freetype hb jpeg minui minzip mman png squirrel ucdn zlib) do (
	rem if folder doesn't exist because it 's not needed, ignore the warning
	if not exist "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\%%L\" (
		set should_warn=1
		if %%L == drm if %LIBAROMA_BUILD_DRM% == 0 set should_warn=0
		if %%L == hb if %LIBAROMA_BUILD_HARFBUZZ% == 0 set should_warn=0
		if %%L == jpeg if %LIBAROMA_BUILD_JPEG% == 0 set should_warn=0
		if %%L == minui if %LIBAROMA_BUILD_MINUI% == 0 set should_warn=0
		if %%L == squirrel if %LIBAROMA_BUILD_RUNTIME% == 0 set should_warn=0
		if !should_warn! == 1 set missing_lib=%%L
	)
)

if defined missing_lib (
	if ""=="" ( echo harfbuzz library was not compiled
	) else echo !missing_lib! library was not compiled.
	if !missing_lib! == !tried_lib! (
		echo Cannot continue, exiting.
		goto :eof
	) else (
		if !missing_lib! == aroma (
			call "%LIBAROMA_BASE%\tools\win32\mkaroma.cmd"
		) else call "%LIBAROMA_BASE%\tools\win32\libs\!missing_lib!.cmd"
	)
	set tried_lib=!missing_lib!
	goto :retry_lib_missing
)
endlocal

pushd "%LIBAROMA_BASE%\out\libs\%LIBAROMA_TARGET_NAME%\"

rem here we use relative path because it's needed later
if not exist "..\..\..\apps\" (
	echo Apps folder not found, cannot find app source
	popd
	goto :eof
) else if not exist "%LIBAROMA_BASE%\apps\%1\" (
	echo Source for ^<%1^> not found
	popd
	goto :eof
)

REM
REM GET OBJECT/SOURCE FILE PATHS
REM This is done because some toolchains don't have wildcard support enabled for Windows, and
REM Windows CMD (unlike Linux shells) doesn't expand wildcards and let the programs do so.
REM WARNING: Windows CMD variables have a limit of 8191 characters, so if all the paths require
REM more than that this will fail to fully set the variables. That should never be the case...
REM
set "drm_objs= "
if "%LIBAROMA_BUILD_DRM%" == "1" (
	for /f "tokens=*" %%F in ('dir /b /a:-d "drm\*.o"') do call set drm_objs=%%drm_objs%% drm\%%F
)
set "freetype_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "freetype\*.o"') do call set freetype_objs=%%freetype_objs%% freetype\%%F
set "hb_objs= "
if "%LIBAROMA_BUILD_HARFBUZZ%" == "1" (
	for /f "tokens=*" %%F in ('dir /b /a:-d "hb\*.o"') do call set hb_objs=%%hb_objs%% hb\%%F
)
set "jpeg_objs= "
if "%LIBAROMA_BUILD_JPEG%" == "1" (
	for /f "tokens=*" %%F in ('dir /b /a:-d "jpeg\*.o"') do call set jpeg_objs=%%jpeg_objs%% jpeg\%%F
)
set "minui_objs= "
if "%LIBAROMA_BUILD_MINUI%" == "1" (
	for /f "tokens=*" %%F in ('dir /b /a:-d "minui\*.o"') do call set minui_objs=%%minui_objs%% minui\%%F
)
set "minzip_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "minzip\*.o"') do call set minzip_objs=%%minzip_objs%% minzip\%%F
set "mman_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "mman\*.o"') do call set mman_objs=%%mman_objs%% mman\%%F
set "png_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "png\*.o"') do call set png_objs=%%png_objs%% png\%%F
set "ucdn_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "ucdn\*.o"') do call set ucdn_objs=%%ucdn_objs%% ucdn\%%F
set "zlib_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "zlib\*.o"') do call set zlib_objs=%%zlib_objs%% zlib\%%F
set "libaroma_objs= "
for /f "tokens=*" %%F in ('dir /b /a:-d "aroma\*.o"') do call set libaroma_objs=%%libaroma_objs%% aroma\%%F
set "app_files= "
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\..\apps\%1\*.c"') do call set app_files=%%app_files%% "..\..\..\apps\%1\%%F"

REM 
REM Additional C files list (sources_list.txt)
REM This allows to build files with C files in subdirectories
REM (by default, this script just compiles *.c on apps/<appname>)
REM 
if exist "..\..\..\apps\%1\sourcelist.txt" (
	rem echo Parsing additional sources
	for /f "tokens=*" %%F in (..\..\..\apps\%1\sourcelist.txt) do call set app_files=%%app_files%% "..\..\..\apps\%1\%%F"
)

if not exist "%LIBAROMA_BASE%\out\bin\" mkdir "%LIBAROMA_BASE%\out\bin\"

echo Building %1%LIBAROMA_DEBUG_SUFFIX% for %LIBAROMA_ARCH% %LIBAROMA_ARCH_APPEND%
set target_name=%1-%LIBAROMA_TARGET_NAME%

%LIBAROMA_GCC% ^
  %LIBAROMA_APP_STATIC% ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -fPIC -DPIC %LIBAROMA_STRIP_OBJECT% ^
  ^
  %LIBAROMA_CFLAGS% ^
  ^
  -DLIBAROMA_CONFIG_DEBUG=%LIBAROMA_CONFIG_DEBUG% ^
  -DLIBAROMA_CONFIG_DEBUG_FILE=%LIBAROMA_CONFIG_DEBUG_FILE% ^
  -DLIBAROMA_CONFIG_DEBUG_MEMORY=%LIBAROMA_CONFIG_DEBUG_MEMORY% ^
  -DLIBAROMA_CONFIG_COMPILER_MESSAGE=%LIBAROMA_CONFIG_COMPILER_MESSAGE% ^
  -DLIBAROMA_CONFIG_SHMEMFB=%LIBAROMA_CONFIG_SHMEMFB% ^
	^
	%drm_objs% ^
	%freetype_objs% ^
	%hb_objs% ^
	%jpeg_objs% ^
	%minui_objs% ^
	%minzip_objs% ^
	%mman_objs% ^
	%png_objs% ^
	%ucdn_objs% ^
	%zlib_objs% ^
	%libaroma_objs% ^
	%app_files% ^
	^
  -I"%LIBAROMA_BASE%\include" ^
  -I"%LIBAROMA_BASE%\libs\selinux\include" ^
  -I"%LIBAROMA_BASE%\src" ^
  -I"%LIBAROMA_BASE%\apps\%1" ^
  -o "%LIBAROMA_BASE%\out\bin\%target_name%" ^
  ^
  -lm -lpthread -lstdc++ %LIBAROMA_ADDITIONAL_LIBS%

set "target_name="
popd

goto :eof

:showhelp
call %LIBAROMA_BASE%\tools\win32\aromahelp.cmd %~n0
goto :eof