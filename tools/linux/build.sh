#!/bin/sh

olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting! 
	exit
fi
cd $scriptdir
./libs/makelibs.sh
./libaroma.sh
./build_app.sh test
cd $olddir
