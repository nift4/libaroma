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
echo Waiting for device \(recovery\)
adb wait-for-recovery
adb shell mkdir -p /tmp
adb push bin/$1 /tmp
#adb wait-for-recovery
adb push ../../examples/$1/zip/res.zip /tmp/
#adb wait-for-recovery
adb shell chmod 755 /tmp/$1
#adb wait-for-recovery
adb shell pkill -19 recovery
#adb wait-for-recovery
adb shell /tmp/$1 0 0 /tmp/res.zip
#adb wait-for-recovery
adb shell pkill -18 recovery
# trap
cd $olddir
