#!/bin/sh

DIR=`dirname $0`

echo "OSNAME=$OSNAME"

if [[ "$OSNAME" == Mac* ]]
then
echo "Building using config macos x86_64"
g++ $DIR/src/main/native/src/compressions_Lz4Compression.cc $DIR/src/main/native/lz4/lz4.c  \
    -I/System/Library/Frameworks/JavaVM.framework/Headers \
    /System/Library/Frameworks/JavaVM.framework/JavaVM \
    -fPIC -O2 -DNDEBUG -shared \
    -o $DIR/target/libcompressions.dylib
else
echo "Building using config linux x86_64"
g++ $DIR/src/main/native/src/compressions_Lz4Compression.cc $DIR/src/main/native/lz4/lz4.c  \
    -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux \
    -L$(JAVA_HOME)/jre/lib/amd64/server -L$(JAVA_HOME)/lib/amd64/server -ljvm \
    -fPIC -O2 -DNDEBUG -shared \
    -o $DIR/target/libcompressions.so
fi
