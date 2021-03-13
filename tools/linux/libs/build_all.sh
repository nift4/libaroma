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
./zlib.sh
./freetype.sh
./hb.sh
./hbucdn.sh
./drm.sh
./minui.sh
./minzip.sh
./png.sh
./jpeg.sh
cd $olddir
