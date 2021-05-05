@echo off
setlocal enableextensions enabledelayedexpansion
pushd %~dp0
if %errorlevel% NEQ 0 ( 
	echo Unable to get script dir, exiting!
	goto :eof
)

set LIBAROMA_DEBUG_ENABLED=0
if "%1" == "d" (
	set LIBAROMA_DEBUG_ENABLED=1
)

REM save current directory into ram, go to scriptdir\..\.. and 
REM use %cd% to get relative path as absolute 
REM (Win32 needs the realpath command, seriously)
pushd %~dp0\..\..
set LIBAROMA_BASE=%cd%
set LIBAROMA_BASE_REL=..\..
popd

rem these ones should be manually changed:
rem LIBAROMA_ARCH can be arm, arm64, x86, x86_64 or mips
rem LIBAORMA_ARCH_APPEND should be filled only when building for ARM, andit's value
rem should be "neon". If building for armv6 or any other, leave empty.
rem LIBAROMA_PLATFORM can be android, linux, sdl, qnx or rpi (special for raspberry pi)
set LIBAROMA_ARCH=arm
set LIBAROMA_ARCH_APPEND=neon
set LIBAROMA_PLATFORM=linux

rem LIBAROMA_TOOLCHAIN_PREFIX is the full path to your toolchain+prefix (if any),
rem use this and put your toolchain path here, if cross compiling
set LIBAROMA_TOOLCHAIN_PREFIX=C:\Users\MLX\projects\armhf-6.3\bin\arm-linux-gnueabihf-
set LIBAROMA_GCC=%LIBAROMA_TOOLCHAIN_PREFIX%gcc
set LIBAROMA_GPP=%LIBAROMA_TOOLCHAIN_PREFIX%g++

rem some default values
set "LIBAROMA_ADDITIONAL_LIBS= "
set LIBAROMA_BUILD_DRM=0
set LIBAROMA_BUILD_JPEG=0
set LIBAROMA_BUILD_MINUI=0
set LIBAROMA_CONFIG_DEBUG=3
set LIBAROMA_CONFIG_DEBUG_FILE=0

rem enable openmp and disable warnings by default
set LIBAROMA_CFLAGS=-fopenmp -DLIBAROMA_CONFIG_OPENMP=1 -w

rem if building for android, revert platform to linux 
rem and enable android in cflags
if "%LIBAROMA_PLATFORM%" == "android" (
	set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -DANDROID=1 -D__ANDROID__
	set LIBAROMA_PLATFORM=linux
)

rem if debug is enabled, use minimal cflags with debug.
rem otherwise, use optimizations and strip output
if %LIBAROMA_DEBUG_ENABLED% == 1 (
	set LIBAROMA_DEBUG_PREFIX=-debug
	set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -g -ggdb -DLIBAROMA_CONFIG_DEBUG_TRACE=3
) else (
	set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -s -O3 -ftree-vectorize -fprefetch-loop-arrays -funsafe-math-optimizations
)

rem enable architecture optimizations
if "%LIBAROMA_ARCH%" == "arm" (
	if "%LIBAROMA_ARCH_APPEND%" == "neon" (
		set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -mfloat-abi=hard -mfpu=neon -D__ARM_HAVE_NEON
		if not %LIBAROMA_DEBUG_ENABLED% == 1 (
			set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -mvectorize-with-neon-quad
		)
	)
) else (
	if "%LIBAROMA_ARCH%" == "x86" set i386=1
	if "%LIBAROMA_ARCH%" == "x86_64" set i386=1
	if "%i386%" == "1" (
		set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -D__i386 
	)
	if defined %i386% set "i386="
)

if "%LIBAROMA_ARCH%" == "arm" (
	if "%LIBAROMA_ARCH_APPEND%" == "neon" (
	rem if targeting ARM with NEON support, build JPEG 
	rem TODO: make jpeg neon optimizations optional
		set LIBAROMA_BUILD_JPEG=1
	) else (
		set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -DLIBAROMA_CONFIG_NOJPEG=1
	)
) else (
rem otherwise, disable jpeg in cflags (it's already disabled in default config)
	set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -DLIBAROMA_CONFIG_NOJPEG=1
)

rem link with SDL library if needed, otherwise build
rem as statically linkable (not possible when using SDL)
if "%LIBAROMA_PLATFORM%" == "sdl" (
	set LIBAROMA_ADDITIONAL_LIBS=%LIBAROMA_ADDITIONAL_LIBS% -lSDL
) else if "%LIBAROMA_PLATFORM%" == "sdl2" (
	set LIBAROMA_ADDITIONAL_LIBS=%LIBAROMA_ADDITIONAL_LIBS% -lSDL2
) else (
	set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -static
	rem if platform is Linux (and architecture isn't non-neon arm), build minui and drm
	if "%LIBAROMA_PLATFORM%" == "linux" set LIBAROMA_BUILD_DRM=1
	if "%LIBAROMA_PLATFORM%" == "android" set LIBAROMA_BUILD_DRM=1
	if "%LIBAROMA_ARCH%" == "arm" (
		if not "%LIBAROMA_ARCH_APPEND%" == "neon" set LIBAROMA_BUILD_DRM=0
	)
	if "!LIBAROMA_BUILD_DRM!" == "1" (
		set LIBAROMA_BUILD_MINUI=1
		set LIBAROMA_CFLAGS=%LIBAROMA_CFLAGS% -DLIBAROMA_GFX_MINUI=1
	)
)

set LIBAROMA_ARCH_OPTIMIZATIONS= 
for /f "tokens=*" %%F in ('dir /b /a:-d "%LIBAROMA_BASE%\src\aroma\arch\%LIBAROMA_ARCH%\*.S"') do call set LIBAROMA_ARCH_OPTIMIZATIONS=%%LIBAROMA_ARCH_OPTIMIZATIONS%% %LIBAROMA_BASE_REL%\..\..\src\aroma\arch\%LIBAROMA_ARCH%\%%F
rem MIPS optimizations are in a .c file, not in a .S one
if "%LIBAROMA_ARCH%" == "mips" (
	rem Remember that Windows CMD doesn't support wildcards! Read app.sh for more info.
	rem set LIBAROMA_ARCH_OPTIMIZATIONS=mips\*.c
	set LIBAROMA_ARCH_OPTIMIZATIONS= 
	for /f "tokens=*" %%F in ('dir /b /a:-d "%LIBAROMA_BASE%\src\aroma\arch\mips\*.c"') do call set LIBAROMA_ARCH_OPTIMIZATIONS=%%LIBAROMA_ARCH_OPTIMIZATIONS%% %LIBAROMA_BASE_REL%..\..\src\aroma\arch\mips\%%F
)

rem set debug levels:
rem (parenthesis means debug function name in code, see src\aroma\debug.h)
rem 1=error (ALOGE)
rem 2=1+info (ALOGI)
rem 3=2+string (ALOGS)
rem 4=3+warnings (ALOGW)
rem 5=4+verbose (ALOGV)
rem 6=5+handled input events (ALOGT)
rem 7=6+raw input events (ALOGRT)
if %LIBAROMA_DEBUG_ENABLED% == 1 (
	set LIBAROMA_CONFIG_DEBUG=5
	set LIBAROMA_CONFIG_DEBUG_FILE=2
)

set LIBAROMA_CONFIG_DEBUG_MEMORY=0
set LIBAROMA_CONFIG_COMPILER_MESSAGE=1
set LIBAROMA_CONFIG_SHMEMFB=0

set LIBAROMA_DONE_TEXT=Ready to build libaroma
if defined LIBAROMA_DEBUG_PREFIX (
	set LIBAROMA_DONE_TEXT=%LIBAROMA_DONE_TEXT%%LIBAROMA_DEBUG_PREFIX%
)
set LIBAROMA_DONE_TEXT=%LIBAROMA_DONE_TEXT% targeting %LIBAROMA_PLATFORM% on %LIBAROMA_ARCH%
rem if arch is ARM, add optimization info
if "%LIBAROMA_ARCH%" == "arm" (
	if "%LIBAROMA_ARCH_APPEND%" == "neon" (
		set LIBAROMA_DONE_TEXT=%LIBAROMA_DONE_TEXT% (NEON-optimized^)
	) else (
		set LIBAROMA_DONE_TEXT=%LIBAROMA_DONE_TEXT% (no optimizations)
	)
)
echo %LIBAROMA_DONE_TEXT%
set "%LIBAROMA_DONE_TEXT="

rem now go to the original directory 
rem and start a new cmd instance
popd
cmd.exe /k "title Libaroma%LIBAROMA_DEBUG_PREFIX% build environment"
endlocal