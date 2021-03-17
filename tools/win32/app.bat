@echo off
cd libaroma

REM
REM GET OBJECT/SOURCE FILE PATHS
REM This is done because some toolchains don't have wildcard support enabled for Windows, and
REM Windows CMD (unlike Linux shells) doesn't expand wildcards and let the programs do so.
REM WARNING: Windows CMD variables have a limit of 8192 characters, so if all the paths require
REM more than that this will fail to fully set the variables. That should never be the case...
REM
set drm_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\drm\*.o"') do call set drm_objs=%%drm_objs%% ..\obj\drm\%%F
set freetype_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\freetype\*.o"') do call set freetype_objs=%%freetype_objs%% ..\obj\freetype\%%F
set hb_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\hb\*.o"') do call set hb_objs=%%hb_objs%% ..\obj\hb\%%F
set hbucdn_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\hbucdn\*.o"') do call set hbucdn_objs=%%hbucdn_objs%% ..\obj\hbucdn\%%F
set jpeg_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\jpeg\*.o"') do call set jpeg_objs=%%jpeg_objs%% ..\obj\jpeg\%%F
set minui_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\minui\*.o"') do call set minui_objs=%%minui_objs%% ..\obj\minui\%%F
set minzip_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\minzip\*.o"') do call set minzip_objs=%%minzip_objs%% ..\obj\minzip\%%F
set png_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\png\*.o"') do call set png_objs=%%png_objs%% ..\obj\png\%%F
set zlib_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\obj\zlib\*.o"') do call set zlib_objs=%%zlib_objs%% ..\obj\zlib\%%F
set libaroma_objs=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\libaroma\*.o"') do call set libaroma_objs=%%libaroma_objs%% ..\libaroma\%%F
set target_files=
for /f "tokens=*" %%F in ('dir /b /a:-d "..\..\..\examples\%1\*.c"') do call set target_files=%%target_files%% ..\..\..\examples\%1\%%F

echo Building %1
%LIBAROMA_GCC% ^
  -static ^
  -fdata-sections -ffunction-sections -Wl,--gc-sections ^
  -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_DEBUG ^
  -fPIC -DPIC -w %LIBAROMA_STRIP_OBJECT% -D_dowildcard=1 ^
  ^
    %LIBAROMA_CFLAGS% ^
  ^
    -DLIBAROMA_CONFIG_DEBUG=$LIBAROMA_CONFIG_DEBUG ^
    -DLIBAROMA_CONFIG_DEBUG_FILE=$LIBAROMA_CONFIG_DEBUG_FILE ^
    -DLIBAROMA_CONFIG_DEBUG_MEMORY=$LIBAROMA_CONFIG_DEBUG_MEMORY ^
    -DLIBAROMA_CONFIG_COMPILER_MESSAGE=$LIBAROMA_CONFIG_COMPILER_MESSAGE ^
    -DLIBAROMA_CONFIG_SHMEMFB=$LIBAROMA_CONFIG_SHMEMFB ^
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
  -I..\..\..\include ^
  -I..\..\..\libs\selinux\include ^
  -I..\..\..\src ^
  -o ..\bin\%1 ^
  ^
  -lm -lpthread -lstdc++ -lrt
  
cd ..