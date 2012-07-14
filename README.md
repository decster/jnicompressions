Introduction
------------

JNICompressions includes JNI wrappers for some of the fastest native 
lightweight compression libraries, which includes:

LZ4:  
http://lz4.googlecode.com/svn/trunk  
revision 68 

Google Snappy  
http://code.google.com/p/snappy/  
version 1.0.5  

LZ4 and Snappy is bundled with this library, so you do not need to install LZ4
and snappy before building this library.

There are 3 interfaces provided:  

    * DirectBuffer interface;
      The speed of DirectBuffer interface is almost identical to native C++ interface performance;

    * Reusable ByteArray interface;
      After using GetPrimitiveArrayCritical instead of malloc & copy, the performance is now the 
      same as DirectBuffer on Oracle JDKs. Thanks Adrien Grand for the advice. 

    * Simple ByteArray interface;
      To improve usability, a very simple byte array interface is added too, but it is 
      much slower, because every compression/decompression need to create new byte arrays(
      involving malloc/free, array initialization, cache invalidate, data copy), 
      this limits total throughput especially in high compression/decompression speed case.

Notice: Current build script only support LINUX & MACOSX x86_64.

Build
-----
There are pre-build Mac OS X 64bit binaries available in /prebuild, currently I do not
have other build environments, sorry for the inconvenience, I will try to add more 
platform and integrate JNI libraries into a single jar file like snappy-java in the future.

Make jar & native library using:
$mvn package

Then add target/jnicompressions-0.1.0.jar to your classpath

Test
----
Run performance test in your own environment:  
$mvn test 

These test results are very sensitive and have very big error ranges, 
1-3% error range is very common, and even bigger for large numbers(speed>3000M/s), 
so don't take small differences too serious.

Here are some test results on my new MacbookPro: 
(I keep the old test results, to show how hardware, library version impact on performance)

    New Test environment: (Lz4 r68 & Snappy 1.0.5)
    Processor Name: Intel Core i7
    Processor Speed:  2.4 GHz
    Number of Processors: 1
    Total Number of Cores:  4
    L2 Cache (per Core):  256 KB
    L3 Cache: 6 MB
    Memory: 8 GB
    gcc version 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.9.00)

    Running com.github.decster.jnicompressions.BlockCompressionTest
    com.github.decster.jnicompressions.Lz4Compression: block size: 64K, dataSize/file: 67108864
    Direct                   Total: Compress:  525M/s Decompress:   766M/s( 1592M/s) ratio: 48.1%
    ByteArray                Total: Compress:  525M/s Decompress:   768M/s( 1596M/s) ratio: 48.1%
    Simple                   Total: Compress:  495M/s Decompress:   586M/s( 1218M/s) ratio: 48.1%
    com.github.decster.jnicompressions.SnappyCompression: block size: 64K, dataSize/file: 67108864
    Direct                   Total: Compress:  473M/s Decompress:   424M/s(  873M/s) ratio: 48.6%
    ByteArray                Total: Compress:  474M/s Decompress:   426M/s(  877M/s) ratio: 48.6%
    Simple                   Total: Compress:  447M/s Decompress:   361M/s(  744M/s) ratio: 48.6%

    Old Test environment: (lz4 r51 & Snappy 1.0.4)
    Processor Name: Intel Core i5
    Processor Speed:  2.3 GHz
    Number of Processors: 1
    Total Number of Cores:  2
    L2 Cache (per Core):  256 KB
    L3 Cache: 3 MB
    Memory: 4 GB
    gcc version 4.2.1 (Apple Inc. build 5659)
    
    Running com.github.decster.jnicompressions.BlockCompressionTest
    com.github.decster.jnicompressions.Lz4Compression: block size: 64K, dataSize/file: 67108864
    Direct                   Total: Compress:  433M/s Decompress:   651M/s( 1354M/s) ratio: 48.1%
    ByteArray                Total: Compress:  417M/s Decompress:   583M/s( 1212M/s) ratio: 48.1%
    Simple                   Total: Compress:  401M/s Decompress:   468M/s(  972M/s) ratio: 48.1%
    com.github.decster.jnicompressions.SnappyCompression: block size: 64K, dataSize/file: 67108864
    Direct                   Total: Compress:  402M/s Decompress:   495M/s( 1019M/s) ratio: 48.6%
    ByteArray                Total: Compress:  390M/s Decompress:   450M/s(  927M/s) ratio: 48.6%
    Simple                   Total: Compress:  373M/s Decompress:   383M/s(  788M/s) ratio: 48.6%

Changes
-------

v0.1.1

  Using GetPrimitiveArrayCritical instead of malloc & copy, thanks to Adrien Grand for the advice. 
  Update LZ4 to r68
  Upate Snappy to 1.0.5


v0.1.0
  initial version
  

