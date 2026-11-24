#!/bin/sh
# $Id: bootstrap.sh 94 2009-09-04 10:18:49Z tdb $

set -e

REQUIRED="core datafile fs strings io"
CFLAGS="-Iinclude `pkg-config --cflags sigc++-2.0`"
LIBS="`pkg-config --libs sigc++-2.0` -lpthread"
if [ -e /usr/lib/libdl.so ]; then
	LIBS="$LIBS -ldl"
fi
if [ -z "$LIBPATH" -o "${LIBPATH%%/*}" ]; then
	LIBPATH=`pwd`/${LIBPATH:-..}
fi

rm -rf include
mkdir -p include/msp

sources=source/*.cpp

missing=""
for i in $REQUIRED; do
	path=`ls -1d $LIBPATH/*$i* 2>/dev/null | head -n1`
	if [ -z "$path"  -o ! -d "$path/source" ]; then
		missing="$missing msp$i"
	else
		if [ $i = "core" ]; then
			ln -sf $path/source/* include/msp/
		else
			ln -sf $path/source include/msp/$i
		fi
		sources="$sources `find $path/source -name '*.cpp'`"
	fi
done

if [ ! -z "$missing" ]; then
	echo "The following libraries were not found:$missing"
	echo "I looked for them in $LIBPATH"
	echo "If they are somewhere else, please adjust the LIBPATH environment variable."
	exit 1
fi

echo "Compiling builder-stage1.  This may take several minutes."
g++ $sources -o builder-stage1 $CFLAGS $LIBS
echo "Using builder-stage1 to compile builder."
./builder-stage1
