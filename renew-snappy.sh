#!/bin/sh

DIR=`dirname $0`

if [ -d $DIR/src/main/native/snappy ]
then
echo "snappy already exists, delete first"
rm -rf $DIR/src/main/native/snappy
fi

echo "svn checkout http://snappy.googlecode.com/svn/trunk/ $DIR/src/main/native/snappy"
svn checkout http://snappy.googlecode.com/svn/trunk/ $DIR/src/main/native/snappy
CURRENTDIR=`pwd`
cd $DIR/src/main/native/snappy
echo "run autogen.sh"
sh autogen.sh
echo "run ./configure"
./configure
cd $CURRENTDIR
