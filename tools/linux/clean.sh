#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
if [ "$1" = "all" ]; then
	echo Removing built files for all architectures
	rm -rf ./libaroma/*
else
	echo Removing built files for $LIBAROMA_ARCH
	rm -rf ./libaroma/$LIBAROMA_ARCH
fi
cd $olddir
