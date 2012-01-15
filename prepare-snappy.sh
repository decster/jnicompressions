#!/bin/sh

DIR=`dirname $0`

if [ -d $DIR/src/main/native/snappy ]
then
echo "snappy already exists" 
else
echo "svn checkout http://snappy.googlecode.com/svn/trunk/ $DIR/src/main/native/snappy"
svn checkout http://snappy.googlecode.com/svn/trunk/ $DIR/src/main/native/snappy
CURRENTDIR=`pwd`
cd $DIR/src/main/native/snappy
sh autogen.sh
./configure
cd $CURRENTDIR
fi
