#!/bin/sh

olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
rm -rf ../obj
mkdir -p ../obj
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
