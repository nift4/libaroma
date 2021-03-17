@ECHO OFF
REM
REM DEBUG FLAG - SET AS ENABLED IF D IS PASSED AS PARAMETER
REM
if "%1"=="d" (
	echo Debug flags enabled
	set LIBAROMA_DEBUG_ENABLED=1
) else (
	set LIBAROMA_DEBUG_ENABLED=0
)

REM
REM STRIP OBJECT SYMBOLS - GIVES SMALLER OUTPUT, NOT USED IF DEBUG ENABLED
REM
if "%LIBAROMA_DEBUG"=="1" (
	set "LIBAROMA_STRIP_OBJECT= "
) else (
	set LIBAROMA_STRIP_OBJECT=-Wl,-s
)

REM
REM TOOLCHAIN AND ADB
REM
REM set LIBAROMA_GDB=C:\arm\bin\arm-linux-gnueabihf-gdb.exe
set LIBAROMA_ADB=C:\ADB\adb.exe
set LIBAROMA_GCC=C:\Users\MLX\projects\armhf-6.3\bin\arm-linux-gnueabihf-gcc.exe
set LIBAROMA_GPP=C:\Users\MLX\projects\armhf-6.3\bin\arm-linux-gnueabihf-g++.exe

if not exist %LIBAROMA_GCC% (
	echo The provided toolchain doesn't seem to exist. 
	echo Please edit the LIBAROMA_GCC and LIBAROMA_GPP lines at this file ^(%0^) with the correct paths.
	goto EOF
)

if not exist %LIBAROMA_ADB% (
	echo The ADB program doesn't exist at the defined path. 
	echo Compiling should work, but you won't be able to test using recovery.bat
)

REM
REM PRODUCTION CFLAGS - USE DEBUG IF ENABLED
REM
if "%LIBAROMA_DEBUG_ENABLED%"=="1" (
	set LIBAROMA_CFLAGS=-g -fopenmp -mfloat-abi=hard -mfpu=neon -D__ARM_HAVE_NEON -DLIBAROMA_CONFIG_OPENMP=1 -DLIBAROMA_CONFIG_DEBUG_TRACE=3
) else (
	set LIBAROMA_CFLAGS=-O3 -ftree-vectorize -mvectorize-with-neon-quad -fprefetch-loop-arrays -funsafe-math-optimizations -fopenmp -mfloat-abi=hard -mfpu=neon -D__ARM_HAVE_NEON -DLIBAROMA_CONFIG_OPENMP=1 -DLIBAROMA_CONFIG_DEBUG_TRACE=0
)

REM
REM LIBAROMA CONFIGS
REM
if "%LIBAROMA_DEBUG_ENABLED%"=="1" (
	set LIBAROMA_CONFIG_DEBUG=5
	set LIBAROMA_CONFIG_DEBUG_FILE=2
) else (
	set LIBAROMA_CONFIG_DEBUG=3
	set LIBAROMA_CONFIG_DEBUG_FILE=0
)
set LIBAROMA_CONFIG_DEBUG_MEMORY=0
set LIBAROMA_CONFIG_COMPILER_MESSAGE=1
set LIBAROMA_CONFIG_SHMEMFB=0

cmd /k

:EOF