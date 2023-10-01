#!/system/bin/sh

if [ ! "$(whoami)" = "root" ]; then
echo "Run with root"
exit 1
fi

if [ $# -eq 1 ]; then
OUTPUTDIR=$1
elif [ $# -eq 0 ]; then
OUTPUTDIR=/sdcard
else
echo "Usage: $0 <output directory>"
exit 1
fi

LOGDIR=$(getprop vendor.logger.log_storage)
cp $LOGDIR/*.txt $OUTPUTDIR
echo "Logs copied to $OUTPUTDIR"
