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
./makelibs_zlib.sh
./makelibs_freetype.sh
./makelibs_hb.sh
./makelibs_hbucdn.sh
./makelibs_minzip.sh
./makelibs_png.sh
./makelibs_squirrel.sh
./makelibs_jpeg.sh
cd $olddir
