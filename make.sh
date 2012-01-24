#!/bin/bash

DIR=`dirname $0`

echo "OSNAME=$OSNAME"

if [[ "$OSNAME" == Mac* ]]
then
echo "Building using config macos x86_64"
g++ $DIR/src/main/native/src/compressions_Lz4Compression.cc $DIR/src/main/native/lz4/lz4.c  \
    $DIR/src/main/native/src/compressions_SnappyCompression.cc \
    $DIR/src/main/native/snappy/snappy.cc \
    $DIR/src/main/native/snappy/snappy-sinksource.cc \
    $DIR/src/main/native/snappy/snappy-stubs-internal.cc \
    -DHAVE_CONFIG_H \
    -I/System/Library/Frameworks/JavaVM.framework/Headers \
    /System/Library/Frameworks/JavaVM.framework/JavaVM \
    -fPIC -O2 -DNDEBUG -shared -static-libgcc \
    -o $DIR/target/libcompressions.jnilib
cp target/libcompressions.jnilib src/main/resources/com/github/decster/jnicompressions/native/mac/x86_64/
else
echo "Building using config linux x86_64"
g++ $DIR/src/main/native/src/compressions_Lz4Compression.cc $DIR/src/main/native/lz4/lz4.c  \
    $DIR/src/main/native/src/compressions_SnappyCompression.cc \
    $DIR/src/main/native/snappy/snappy.cc \
    $DIR/src/main/native/snappy/snappy-sinksource.cc \
    $DIR/src/main/native/snappy/snappy-stubs-internal.cc \
    -DHAVE_CONFIG_H \
    -I$JAVA_HOME/include -I$JAVA_HOME/include/linux \
    -L$JAVA_HOME/jre/lib/amd64/server -L$JAVA_HOME/lib/amd64/server -ljvm \
    -fPIC -O2 -DNDEBUG -shared -static-libgcc -static-libstdc++ \
    -o $DIR/target/libcompressions.so
cp target/libcompressions.so src/main/resources/com/github/decster/jnicompressions/native/linux/x86_64/
fi
