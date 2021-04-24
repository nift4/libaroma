#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir

LIBAROMA_DEBUG_ENABLED=0
if [ "$1" = "d" ]; then
	LIBAROMA_DEBUG_ENABLED=1
fi

export LIBAROMA_BASE="$(readlink -f $PWD/../..)"

# these ones should be manually changed:
# LIBAROMA_ARCH can be arm, arm64, x86, x86_64 or mips
# LIBAORMA_ARCH_APPEND should be filled only when building for ARM, andit's value
# should be "neon". If building for armv6 or any other, leave empty.
# LIBAROMA_PLATFORM can be linux, sdl, qnx or rpi (special for raspberry pi)
export LIBAROMA_ARCH="arm"
export LIBAROMA_ARCH_APPEND="neon"
export LIBAROMA_PLATFORM="linux"

# LIBAROMA_TOOLCHAIN_PREFIX is the full path to your toolchain+prefix (if any),
# use this to automatically find compiler (like gcc at /usr/bin)
#LIBAROMA_TOOLCHAIN_PREFIX=""
# or use this and put your toolchain path here, if cross compiling
LIBAROMA_TOOLCHAIN_PREFIX="/home/mlx/projects/musl-10/bin/arm-linux-musleabihf-"
export LIBAROMA_GCC="${LIBAROMA_TOOLCHAIN_PREFIX}gcc"
export LIBAROMA_GPP="${LIBAROMA_TOOLCHAIN_PREFIX}g++"

# some default values
LIBAROMA_ADDITIONAL_LIBS=""
LIBAROMA_BUILD_DRM=0
LIBAROMA_BUILD_JPEG=1
LIBAROMA_BUILD_MINUI=0
LIBAROMA_DRM_OBJ=""
LIBAROMA_JPEG_OBJ="${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/jpeg/*.o"
LIBAROMA_MINUI_OBJ=""
LIBAROMA_CONFIG_DEBUG="3"
LIBAROMA_CONFIG_DEBUG_FILE="0"
LIBAROMA_DEBUG_PREFIX=""

# enable openmp and disable warnings by default
LIBAROMA_CFLAGS="-fopenmp -DLIBAROMA_CONFIG_OPENMP=1 -w"

# if debug is enabled, use minimal cflags with debug.
# otherwise, use optimizations and strip output
if [ ${LIBAROMA_DEBUG_ENABLED} = 1 ]; then
	LIBAROMA_DEBUG_PREFIX="-debug"
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -g -ggdb -DLIBAROMA_CONFIG_DEBUG_TRACE=3"
else
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -s -O3 -ftree-vectorize -fprefetch-loop-arrays -funsafe-math-optimizations"
fi

# enable architecture optimizations
if [ "${LIBAROMA_ARCH}" = "arm" ] && [ "${LIBAROMA_ARCH_APPEND}" = "neon" ]; then
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -mfloat-abi=hard -mfpu=neon -D__ARM_HAVE_NEON"
	if [ ${LIBAROMA_DEBUG_ENABLED} != 1 ]; then
		LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -mvectorize-with-neon-quad"
	fi
elif [ "${LIBAROMA_ARCH}" = "x86" ] || [ "${LIBAROMA_ARCH}" = "x86_64" ]; then
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -D__i386 "
fi

# if not building for ARM, disable JPEG (TODO: make jpeg neon optimizations optional)
if [ "${LIBAROMA_ARCH_APPEND}" != "neon" ]; then
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -DLIBAROMA_CONFIG_NOJPEG=1"
	LIBAROMA_BUILD_JPEG=0
	LIBAROMA_JPEG_OBJ=""
fi

# link with SDL library if needed, otherwise build
# as statically linkable (not possible when using SDL)
if [ "${LIBAROMA_PLATFORM}" = "sdl" ]; then
	LIBAROMA_ADDITIONAL_LIBS="${LIBAROMA_ADDITIONAL_LIBS} -lSDL"
elif [ "${LIBAROMA_PLATFORM}" = "sdl2" ]; then
	LIBAROMA_ADDITIONAL_LIBS="${LIBAROMA_ADDITIONAL_LIBS} -lSDL2"
else
	LIBAROMA_CFLAGS="${LIBAROMA_CFLAGS} -static"
	# if platform is Linux (and architecture isn't non-neon arm), build minui and drm
	if [ "${LIBAROMA_PLATFORM}" = "linux" ] || \
		([ "${LIBAROMA_PLATFORM}" = "linux" ]  && [ "${LIBAROMA_ARCH}" = "arm" ] && [ "${LIBAROMA_ARCH_APPEND}" = "neon" ]); then
		LIBAROMA_BUILD_DRM=1
		LIBAROMA_BUILD_MINUI=1
		LIBAROMA_DRM_OBJ="${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/drm/*.o"
		LIBAROMA_MINUI_OBJ="${LIBAROMA_BASE}/tools/linux/obj/${LIBAROMA_ARCH}/minui/*.o"
	fi
fi

LIBAROMA_ARCH_OPTIMIZATIONS="${LIBAROMA_ARCH}/*.S"
# MIPS optimizations are in a .c file, not in a .S one
if [ "${LIBAROMA_ARCH}" = "mips" ]; then
	LIBAROMA_ARCH_OPTIMIZATIONS="mips/*.c"
elif [ "${LIBAROMA_ARCH}" = "arm" ] && [ "${LIBAROMA_ARCH_APPEND}" = "neon" ]; then
	LIBAROMA_ARCH_OPTIMIZATIONS="arm/*.S"
fi

# set debug levels:
# (parenthesis means debug function name in code, see src/aroma/debug.h)
# 1=error (ALOGE)
# 2=1+info (ALOGI)
# 3=2+string (ALOGS)
# 4=3+warnings (ALOGW)
# 5=4+verbose (ALOGV)
# 6=5+handled input events (ALOGT)
# 7=6+raw input events (ALOGRT)
if [ ${LIBAROMA_DEBUG_ENABLED} = 1 ]; then
	LIBAROMA_CONFIG_DEBUG="5"
	LIBAROMA_CONFIG_DEBUG_FILE="2"
fi

# export the variables so the shell can use them
export LIBAROMA_CFLAGS
export LIBAROMA_BUILD_DRM
export LIBAROMA_BUILD_JPEG
export LIBAROMA_BUILD_MINUI
export LIBAROMA_DRM_OBJ
export LIBAROMA_JPEG_OBJ
export LIBAROMA_MINUI_OBJ
export LIBAROMA_CONFIG_DEBUG
export LIBAROMA_CONFIG_DEBUG_FILE
export LIBAROMA_CONFIG_DEBUG_MEMORY="0"
export LIBAROMA_CONFIG_COMPILER_MESSAGE="1"
export LIBAROMA_CONFIG_SHMEMFB="0"
export LIBAROMA_ARCH_OPTIMIZATIONS
export LIBAROMA_ADDITIONAL_LIBS
export LIBAROMA_DEBUG_ENABLED

LIBAROMA_DONE_TEXT="Ready to build libaroma$LIBAROMA_DEBUG_PREFIX targeting ${LIBAROMA_PLATFORM} on ${LIBAROMA_ARCH}"
#if arch is ARM, add optimization info
if [ "${LIBAROMA_ARCH}" = "arm" ]; then
	if [ "${LIBAROMA_ARCH_APPEND}" = "neon" ]; then
		LIBAROMA_DONE_TEXT="${LIBAROMA_DONE_TEXT} (NEON-optimized)"
	else
		LIBAROMA_DONE_TEXT="${LIBAROMA_DONE_TEXT} (no optimizations)"
	fi
fi
echo ${LIBAROMA_DONE_TEXT}

# if bash is available, use it. else, use sh
# TODO: use shell that started the script
# (not the current one, as it's hardcoded at the top)
if [ -x "$(command -v bash)" ]; then
	bash
else sh
fi
cd $olddir
