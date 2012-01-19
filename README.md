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

The speed of compression/decompression using JNI is almost identical to native 
interface. In general, byte array interface is a little bit slower(very little 
for compression, 1x% for decompression) than ByteBuffer interface, because byte 
array interface needs to copy to/from native buffer when crossing JNI boundary. 
If input/output buffer size is reasonable, usually 32KB~256KB, given that 
bulk memory copy is very fast in CPU cache(nearly 20GB/s), byte array interface 
will be acceptable in most scenarios.

Notice: Current build script only support LINUX & MACOSX x86_64.

Build
-----
There are pre-build Mac OS X 64bit binaries available in /lib, currently I do not
have other build envirments, but I will try to add more flatforms and integrate jni 
libraries into a single jar file in the future.

Make jar & native library using:
$mvn package

Then add target/jnicompressions-0.1.0.jar to your classpath, and add 
target/libcompressions.* parent path to your java.library.path

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
    com.github.decster.jnicompressions.Lz4Compression: DirectByteBuffer interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  245M/s Decompress:   596M/s(  998M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  231M/s Decompress:   624M/s( 1002M/s) ratio: 62.3%
                   cp.html:   Compress:  338M/s Decompress:   748M/s( 1545M/s) ratio: 48.4%
                  fields.c:   Compress:  447M/s Decompress:   647M/s( 1383M/s) ratio: 46.8%
             geo.protodata:   Compress:  899M/s Decompress:   520M/s( 2925M/s) ratio: 17.8%
               grammar.lsp:   Compress:  510M/s Decompress:   808M/s( 1573M/s) ratio: 51.4%
                 house.jpg:   Compress: 4547M/s Decompress: 11605M/s(11576M/s) ratio: 100.2%
                      html:   Compress:  708M/s Decompress:   465M/s( 2133M/s) ratio: 21.8%
                  html_x_4:   Compress:  690M/s Decompress:   458M/s( 2078M/s) ratio: 22.1%
               kennedy.xls:   Compress:  537M/s Decompress:   508M/s( 1396M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  410M/s Decompress:   439M/s(  952M/s) ratio: 46.1%
                lcet10.txt:   Compress:  261M/s Decompress:   569M/s( 1011M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress: 1262M/s Decompress:  3604M/s( 4367M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  216M/s Decompress:   667M/s(  998M/s) ratio: 66.8%
                      ptt5:   Compress:  971M/s Decompress:   413M/s( 2442M/s) ratio: 16.9%
                       sum:   Compress:  350M/s Decompress:   721M/s( 1465M/s) ratio: 49.2%
                  terasort:   Compress: 1274M/s Decompress:   691M/s( 3719M/s) ratio: 18.6%
                  urls.10K:   Compress:  374M/s Decompress:   721M/s( 1535M/s) ratio: 47.0%
                   xargs.1:   Compress:  423M/s Decompress:   936M/s( 1489M/s) ratio: 62.9%
                     Total:   Compress:  421M/s Decompress:   631M/s( 1467M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.Lz4Compression: byte array interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  244M/s Decompress:   541M/s(  906M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  221M/s Decompress:   569M/s(  913M/s) ratio: 62.3%
                   cp.html:   Compress:  317M/s Decompress:   633M/s( 1309M/s) ratio: 48.4%
                  fields.c:   Compress:  427M/s Decompress:   567M/s( 1213M/s) ratio: 46.8%
             geo.protodata:   Compress:  848M/s Decompress:   444M/s( 2494M/s) ratio: 17.8%
               grammar.lsp:   Compress:  465M/s Decompress:   683M/s( 1330M/s) ratio: 51.4%
                 house.jpg:   Compress: 2962M/s Decompress:  5063M/s( 5051M/s) ratio: 100.2%
                      html:   Compress:  663M/s Decompress:   402M/s( 1844M/s) ratio: 21.8%
                  html_x_4:   Compress:  652M/s Decompress:   398M/s( 1803M/s) ratio: 22.1%
               kennedy.xls:   Compress:  511M/s Decompress:   452M/s( 1240M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  395M/s Decompress:   400M/s(  867M/s) ratio: 46.1%
                lcet10.txt:   Compress:  257M/s Decompress:   524M/s(  932M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress: 1069M/s Decompress:  2494M/s( 3022M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  211M/s Decompress:   609M/s(  911M/s) ratio: 66.8%
                      ptt5:   Compress:  897M/s Decompress:   352M/s( 2083M/s) ratio: 16.9%
                       sum:   Compress:  332M/s Decompress:   645M/s( 1311M/s) ratio: 49.2%
                  terasort:   Compress: 1150M/s Decompress:   553M/s( 2979M/s) ratio: 18.6%
                  urls.10K:   Compress:  360M/s Decompress:   639M/s( 1361M/s) ratio: 47.0%
                   xargs.1:   Compress:  399M/s Decompress:   784M/s( 1247M/s) ratio: 62.9%
                     Total:   Compress:  406M/s Decompress:   560M/s( 1303M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.SnappyCompression: DirectByteBuffer interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  238M/s Decompress:   373M/s(  624M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  221M/s Decompress:   382M/s(  597M/s) ratio: 64.1%
                   cp.html:   Compress:  339M/s Decompress:   486M/s( 1010M/s) ratio: 48.1%
                  fields.c:   Compress:  433M/s Decompress:   377M/s(  890M/s) ratio: 42.4%
             geo.protodata:   Compress:  761M/s Decompress:   461M/s( 1991M/s) ratio: 23.2%
               grammar.lsp:   Compress:  312M/s Decompress:   538M/s( 1111M/s) ratio: 48.4%
                 house.jpg:   Compress: 3526M/s Decompress: 14091M/s(14108M/s) ratio: 99.9%
                      html:   Compress:  664M/s Decompress:   387M/s( 1642M/s) ratio: 23.6%
                  html_x_4:   Compress:  643M/s Decompress:   377M/s( 1600M/s) ratio: 23.6%
               kennedy.xls:   Compress:  524M/s Decompress:   380M/s(  919M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  358M/s Decompress:   282M/s(  738M/s) ratio: 38.3%
                lcet10.txt:   Compress:  251M/s Decompress:   380M/s(  666M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress: 1267M/s Decompress:  3411M/s( 4153M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  207M/s Decompress:   375M/s(  549M/s) ratio: 68.4%
                      ptt5:   Compress:  931M/s Decompress:   299M/s( 1644M/s) ratio: 18.2%
                       sum:   Compress:  314M/s Decompress:   457M/s(  880M/s) ratio: 51.9%
                  terasort:   Compress: 1096M/s Decompress:   580M/s( 2652M/s) ratio: 21.9%
                  urls.10K:   Compress:  349M/s Decompress:   552M/s( 1085M/s) ratio: 50.9%
                   xargs.1:   Compress:  322M/s Decompress:   547M/s(  922M/s) ratio: 59.4%
                     Total:   Compress:  400M/s Decompress:   436M/s(  965M/s) ratio: 45.2%
    
    com.github.decster.jnicompressions.SnappyCompression: byte array interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  232M/s Decompress:   355M/s(  594M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  212M/s Decompress:   351M/s(  548M/s) ratio: 64.1%
                   cp.html:   Compress:  301M/s Decompress:   443M/s(  920M/s) ratio: 48.1%
                  fields.c:   Compress:  418M/s Decompress:   331M/s(  780M/s) ratio: 42.4%
             geo.protodata:   Compress:  750M/s Decompress:   401M/s( 1733M/s) ratio: 23.2%
               grammar.lsp:   Compress:  380M/s Decompress:   468M/s(  967M/s) ratio: 48.4%
                 house.jpg:   Compress: 2463M/s Decompress:  5474M/s( 5480M/s) ratio: 99.9%
                      html:   Compress:  599M/s Decompress:   333M/s( 1413M/s) ratio: 23.6%
                  html_x_4:   Compress:  615M/s Decompress:   338M/s( 1436M/s) ratio: 23.6%
               kennedy.xls:   Compress:  496M/s Decompress:   347M/s(  840M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  349M/s Decompress:   266M/s(  695M/s) ratio: 38.3%
                lcet10.txt:   Compress:  244M/s Decompress:   360M/s(  631M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress: 1179M/s Decompress:  2400M/s( 2921M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  203M/s Decompress:   353M/s(  517M/s) ratio: 68.4%
                      ptt5:   Compress:  875M/s Decompress:   267M/s( 1467M/s) ratio: 18.2%
                       sum:   Compress:  301M/s Decompress:   416M/s(  801M/s) ratio: 51.9%
                  terasort:   Compress:  867M/s Decompress:   358M/s( 1636M/s) ratio: 21.9%
                  urls.10K:   Compress:  322M/s Decompress:   491M/s(  964M/s) ratio: 50.9%
                   xargs.1:   Compress:  357M/s Decompress:   528M/s(  890M/s) ratio: 59.4%
                     Total:   Compress:  381M/s Decompress:   397M/s(  879M/s) ratio: 45.2%


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

