@echo off
if "%1"=="" (
	echo app.cmd - build libaroma app
	echo Usage: %0 ^<appname^>
	echo.
	echo ^<appname^>    Name of the application to be built
	goto :eof
)
if not exist %~dp0\libaroma\%LIBAROMA_ARCH% (
	echo Libaroma for %LIBAROMA_ARCH% wasn't found! Exiting...
	goto :eof
)
pushd %~dp0\libaroma\%LIBAROMA_ARCH%

REM
REM GET OBJECT/SOURCE FILE PATHS
REM This is done because some toolchains don't have wildcard support enabled for Windows, and
REM Windows CMD (unlike Linux shells) doesn't expand wildcards and let the programs do so.
REM WARNING: Windows CMD variables have a limit of 8192 characters, so if all the paths require
REM more than that this will fail to fully set the variables. That should never be the case...
REM
set drm_objs= 
if "%LIBAROMA_BUILD_DRM%" == "1" (
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\drm\*.o"') do call set drm_objs=%%drm_objs%% ..\..\obj\%LIBAROMA_ARCH%\drm\%%F
)
set freetype_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\freetype\*.o"') do call set freetype_objs=%%freetype_objs%% ..\..\obj\%LIBAROMA_ARCH%\freetype\%%F
set hb_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\hb\*.o"') do call set hb_objs=%%hb_objs%% ..\..\obj\%LIBAROMA_ARCH%\hb\%%F
set hbucdn_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\hbucdn\*.o"') do call set hbucdn_objs=%%hbucdn_objs%% ..\..\obj\%LIBAROMA_ARCH%\hbucdn\%%F
set jpeg_objs= 
if "%LIBAROMA_BUILD_JPEG%" == "1" (
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\jpeg\*.o"') do call set jpeg_objs=%%jpeg_objs%% ..\..\obj\%LIBAROMA_ARCH%\jpeg\%%F
)
set minui_objs= 
if "%LIBAROMA_BUILD_MINUI%" == "1" (
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\minui\*.o"') do call set minui_objs=%%minui_objs%% ..\..\obj\%LIBAROMA_ARCH%\minui\%%F
)
set minzip_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\minzip\*.o"') do call set minzip_objs=%%minzip_objs%% ..\..\obj\%LIBAROMA_ARCH%\minzip\%%F
set png_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\png\*.o"') do call set png_objs=%%png_objs%% ..\..\obj\%LIBAROMA_ARCH%\png\%%F
set zlib_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\obj\%LIBAROMA_ARCH%\zlib\*.o"') do call set zlib_objs=%%zlib_objs%% ..\..\obj\%LIBAROMA_ARCH%\zlib\%%F
set libaroma_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\libaroma\%LIBAROMA_ARCH%\*.o"') do call set libaroma_objs=%%libaroma_objs%% ..\..\libaroma\%LIBAROMA_ARCH%\%%F
set target_files=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\..\..\examples\%1\*.c"') do call set target_files=%%target_files%% ..\..\..\..\examples\%1\%%F

echo Building %1 at %cd%
%LIBAROMA_GCC% ^
  -static ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -w %LIBAROMA_STRIP_OBJECT% -D_dowildcard=1 ^
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
    %hbucdn_objs% ^
    %jpeg_objs% ^
    %minui_objs% ^
    %minzip_objs% ^
    %png_objs% ^
    %zlib_objs% ^
    %libaroma_objs% ^
    %target_files% ^
  ^
  -I%LIBAROMA_BASE%\include ^
  -I%LIBAROMA_BASE%\libs\selinux\include ^
  -I%LIBAROMA_BASE%\src ^
  -o ..\..\bin\%1-%LIBAROMA_ARCH% ^
  ^
  -lm -lpthread -lstdc++ -lrt
  
popd