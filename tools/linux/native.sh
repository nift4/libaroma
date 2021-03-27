#!/bin/sh
olddir=$PWD
scriptdir=$(dirname "$(readlink -f -- "$0")")
if [ -z "$scriptdir" ]; then
	echo Unable to get script dir, exiting!
	exit
fi
cd $scriptdir
if [ "$2" != "r" ]; then
	./app.sh $1
fi
echo Switching to virtual terminal \(requires su\)
# switch to virtual terminal so libaroma don't fight with X for the screen
OLD_VT=$(sudo fgconsole)
if [ -z $OLD_VT ]; then
OLD_VT=7
fi
echo OLD_VT is $OLD_VT
sudo chvt 2
echo running
sudo bin/$1 3 0 ../../examples/$1/zip/res.zip
sudo chvt $OLD_VT
