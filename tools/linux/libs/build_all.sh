#!/bin/sh

olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
rm -rf ${LIBAROMA_BASE}/tools/linux/obj
mkdir -p ${LIBAROMA_BASE}/tools/linux/obj
./drm.sh
./freetype.sh
./harfbuzz.sh
./jpeg.sh
./minui.sh
./minzip.sh
./ucdn.sh
./png.sh
./zlib.sh
cd $olddir
