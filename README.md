Introduction
------------

JNICompressions includes JNI wrappers for some of the fastest native 
lightweight compression libraries, which includes:

LZ4:  
http://lz4.googlecode.com/svn/trunk  
revision 51  

Google Snappy  
http://code.google.com/p/snappy/  
trunk version  

LZ4 and Snappy is bundled with this library, so you do not need to install LZ4
and snappy before building this library.

There are 3 interfaces provided:  

    * DirectBuffer interface;
      The speed of DirectBuffer interface is almost identical to native C++ interface performance;

    * Reusable ByteArray interface;
      In general, byte array interface is a little bit slower(very little 
      for compression, 1x% for decompression) than ByteBuffer interface, because byte 
      array interface needs to copy to/from native buffer when crossing JNI boundary. 
      If input/output buffer size is reasonable, usually 32KB~256KB, given that 
      bulk memory copy is very fast in CPU cache(nearly 20GB/s), byte array interface 
      will be acceptable in most scenarios.

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

Here are some test results on my MacbookPro:

    Test environment:
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


Just for comparison, here are some test results using C++ instead of Java/JNI.
You can find the test code in another project of mine:
[NativeTask](https://github.com/decster/nativetask)

    Compress:  252M/s Decompress:   642M/s( 1074M/s) ratio: 59.7% - alice29.txt
    Compress:  235M/s Decompress:   666M/s( 1069M/s) ratio: 62.3% - asyoulik.txt
    Compress:  352M/s Decompress:   891M/s( 1841M/s) ratio: 48.4% - cp.html
    Compress:  457M/s Decompress:   706M/s( 1509M/s) ratio: 46.8% - fields.c
    Compress:  931M/s Decompress:   599M/s( 3367M/s) ratio: 17.8% - geo.protodata
    Compress:  490M/s Decompress:   980M/s( 1908M/s) ratio: 51.4% - grammar.lsp
    Compress: 4779M/s Decompress: 14640M/s(14603M/s) ratio: 100.2% - house.jpg
    Compress:  710M/s Decompress:   525M/s( 2409M/s) ratio: 21.8% - html
    Compress:  695M/s Decompress:   511M/s( 2314M/s) ratio: 22.1% - html_x_4
    Compress:  547M/s Decompress:   540M/s( 1482M/s) ratio: 36.4% - kennedy.xls
    Compress:  411M/s Decompress:   474M/s( 1026M/s) ratio: 46.1% - kppkn.gtb
    Compress:  266M/s Decompress:   615M/s( 1094M/s) ratio: 56.2% - lcet10.txt
    Compress: 1273M/s Decompress:  3874M/s( 4694M/s) ratio: 82.5% - mapreduce-osdi-1.pdf
    Compress:  220M/s Decompress:   707M/s( 1058M/s) ratio: 66.8% - plrabn12.txt
    Compress:  983M/s Decompress:   454M/s( 2685M/s) ratio: 16.9% - ptt5
    Compress:  357M/s Decompress:   810M/s( 1646M/s) ratio: 49.2% - sum
    Compress: 1191M/s Decompress:   719M/s( 3872M/s) ratio: 18.6% - terasort
    Compress:  377M/s Decompress:   790M/s( 1682M/s) ratio: 47.0% - urls.10K
    Compress:  423M/s Decompress:  1064M/s( 1692M/s) ratio: 62.9% - xargs.1
    Compress:  428M/s Decompress:   681M/s( 1584M/s) ratio: 43.0% - Total
    [       OK ] Perf.RawCompressionLz4 (13406 ms)
    [ RUN      ] Perf.RawCompressionSnappy
    Block size: 64K
    Compress:  236M/s Decompress:   372M/s(  622M/s) ratio: 59.8% - alice29.txt
    Compress:  223M/s Decompress:   378M/s(  590M/s) ratio: 64.1% - asyoulik.txt
    Compress:  345M/s Decompress:   564M/s( 1172M/s) ratio: 48.1% - cp.html
    Compress:  444M/s Decompress:   450M/s( 1060M/s) ratio: 42.4% - fields.c
    Compress:  799M/s Decompress:   512M/s( 2211M/s) ratio: 23.2% - geo.protodata
    Compress:  457M/s Decompress:   558M/s( 1154M/s) ratio: 48.4% - grammar.lsp
    Compress: 3475M/s Decompress: 18215M/s(18237M/s) ratio: 99.9% - house.jpg
    Compress:  663M/s Decompress:   410M/s( 1740M/s) ratio: 23.6% - html
    Compress:  656M/s Decompress:   417M/s( 1768M/s) ratio: 23.6% - html_x_4
    Compress:  529M/s Decompress:   387M/s(  935M/s) ratio: 41.3% - kennedy.xls
    Compress:  369M/s Decompress:   287M/s(  749M/s) ratio: 38.3% - kppkn.gtb
    Compress:  251M/s Decompress:   380M/s(  665M/s) ratio: 57.1% - lcet10.txt
    Compress: 1329M/s Decompress:  3559M/s( 4333M/s) ratio: 82.1% - mapreduce-osdi-1.pdf
    Compress:  204M/s Decompress:   367M/s(  537M/s) ratio: 68.4% - plrabn12.txt
    Compress:  893M/s Decompress:   300M/s( 1645M/s) ratio: 18.2% - ptt5
    Compress:  318M/s Decompress:   454M/s(  875M/s) ratio: 51.9% - sum
    Compress: 1142M/s Decompress:   557M/s( 2546M/s) ratio: 21.9% - terasort
    Compress:  348M/s Decompress:   581M/s( 1142M/s) ratio: 50.9% - urls.10K
    Compress:  379M/s Decompress:   573M/s(  966M/s) ratio: 59.4% - xargs.1
    Compress:  400M/s Decompress:   442M/s(  979M/s) ratio: 45.2% - Total

