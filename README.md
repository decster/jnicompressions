Introduction
------------

JNICompressions includes JNI wrappers for some of the fastest native 
lightweight compression libraries, which includes:

LZ4:  
http://lz4.googlecode.com/svn/trunk  
revision 51(Bugfix version)

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
Make jar & native library using:

$mvn package

Then add target/jnicompressions-0.1.0.jar to your classpath, and add 
target/libcompressions.* parent path to your java.library.path

Test
----
Run performance test in your own environment:  
$mvn test 

Here are some test results on my MacbookPro, 

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
    com.github.decster.jnicompressions.Lz4Compression: DirectByteBuffer interface, block size: 64K, time: 400
               alice29.txt:   Compress:  213M/s Decompress:   519M/s(  868M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  204M/s Decompress:   579M/s(  928M/s) ratio: 62.3%
                   cp.html:   Compress:  316M/s Decompress:   677M/s( 1400M/s) ratio: 48.4%
                  fields.c:   Compress:  367M/s Decompress:   624M/s( 1334M/s) ratio: 46.8%
             geo.protodata:   Compress:  784M/s Decompress:   446M/s( 2509M/s) ratio: 17.8%
               grammar.lsp:   Compress:  402M/s Decompress:   468M/s(  910M/s) ratio: 51.4%
                 house.jpg:   Compress: 3997M/s Decompress: 11179M/s(11151M/s) ratio: 100.2%
                      html:   Compress:  572M/s Decompress:   389M/s( 1781M/s) ratio: 21.8%
                  html_x_4:   Compress:  587M/s Decompress:   391M/s( 1774M/s) ratio: 22.1%
               kennedy.xls:   Compress:  493M/s Decompress:   540M/s( 1482M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  346M/s Decompress:   413M/s(  896M/s) ratio: 46.1%
                lcet10.txt:   Compress:  220M/s Decompress:   561M/s(  998M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress: 1071M/s Decompress:  2680M/s( 3248M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  194M/s Decompress:   593M/s(  887M/s) ratio: 66.8%
                      ptt5:   Compress:  835M/s Decompress:   331M/s( 1962M/s) ratio: 16.9%
                       sum:   Compress:  284M/s Decompress:   672M/s( 1366M/s) ratio: 49.2%
                  terasort:   Compress:  964M/s Decompress:   666M/s( 3585M/s) ratio: 18.6%
                  urls.10K:   Compress:  315M/s Decompress:   616M/s( 1310M/s) ratio: 47.0%
                   xargs.1:   Compress:  385M/s Decompress:   820M/s( 1304M/s) ratio: 62.9%
                     Total:   Compress:  366M/s Decompress:   583M/s( 1355M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.Lz4Compression: byte array interface, block size: 64K, time: 400
               alice29.txt:   Compress:  204M/s Decompress:   495M/s(  829M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  195M/s Decompress:   546M/s(  876M/s) ratio: 62.3%
                   cp.html:   Compress:  305M/s Decompress:   455M/s(  940M/s) ratio: 48.4%
                  fields.c:   Compress:  382M/s Decompress:   558M/s( 1193M/s) ratio: 46.8%
             geo.protodata:   Compress:  715M/s Decompress:   369M/s( 2073M/s) ratio: 17.8%
               grammar.lsp:   Compress:  416M/s Decompress:   651M/s( 1267M/s) ratio: 51.4%
                 house.jpg:   Compress: 2089M/s Decompress:  3738M/s( 3729M/s) ratio: 100.2%
                      html:   Compress:  531M/s Decompress:   360M/s( 1650M/s) ratio: 21.8%
                  html_x_4:   Compress:  537M/s Decompress:   344M/s( 1561M/s) ratio: 22.1%
               kennedy.xls:   Compress:  465M/s Decompress:   479M/s( 1314M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  334M/s Decompress:   384M/s(  833M/s) ratio: 46.1%
                lcet10.txt:   Compress:  215M/s Decompress:   502M/s(  893M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress:  938M/s Decompress:  2214M/s( 2683M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  188M/s Decompress:   529M/s(  791M/s) ratio: 66.8%
                      ptt5:   Compress:  773M/s Decompress:   275M/s( 1629M/s) ratio: 16.9%
                       sum:   Compress:  289M/s Decompress:   651M/s( 1324M/s) ratio: 49.2%
                  terasort:   Compress:  827M/s Decompress:   466M/s( 2506M/s) ratio: 18.6%
                  urls.10K:   Compress:  302M/s Decompress:   590M/s( 1255M/s) ratio: 47.0%
                   xargs.1:   Compress:  393M/s Decompress:   753M/s( 1198M/s) ratio: 62.9%
                     Total:   Compress:  349M/s Decompress:   520M/s( 1210M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.SnappyCompression: DirectByteBuffer interface, block size: 64K, time: 400
               alice29.txt:   Compress:  216M/s Decompress:   345M/s(  576M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  201M/s Decompress:   344M/s(  537M/s) ratio: 64.1%
                   cp.html:   Compress:  279M/s Decompress:   376M/s(  782M/s) ratio: 48.1%
                  fields.c:   Compress:  407M/s Decompress:   400M/s(  943M/s) ratio: 42.4%
             geo.protodata:   Compress:  711M/s Decompress:   405M/s( 1747M/s) ratio: 23.2%
               grammar.lsp:   Compress:  399M/s Decompress:   519M/s( 1072M/s) ratio: 48.4%
                 house.jpg:   Compress: 3437M/s Decompress: 13088M/s(13104M/s) ratio: 99.9%
                      html:   Compress:  602M/s Decompress:   339M/s( 1437M/s) ratio: 23.6%
                  html_x_4:   Compress:  571M/s Decompress:   334M/s( 1417M/s) ratio: 23.6%
               kennedy.xls:   Compress:  471M/s Decompress:   342M/s(  826M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  331M/s Decompress:   266M/s(  694M/s) ratio: 38.3%
                lcet10.txt:   Compress:  229M/s Decompress:   346M/s(  606M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress: 1249M/s Decompress:  2670M/s( 3251M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  189M/s Decompress:   342M/s(  500M/s) ratio: 68.4%
                      ptt5:   Compress:  810M/s Decompress:   271M/s( 1486M/s) ratio: 18.2%
                       sum:   Compress:  265M/s Decompress:   417M/s(  804M/s) ratio: 51.9%
                  terasort:   Compress:  960M/s Decompress:   510M/s( 2335M/s) ratio: 21.9%
                  urls.10K:   Compress:  306M/s Decompress:   495M/s(  973M/s) ratio: 50.9%
                   xargs.1:   Compress:  318M/s Decompress:   510M/s(  859M/s) ratio: 59.4%
                     Total:   Compress:  360M/s Decompress:   394M/s(  872M/s) ratio: 45.2%
    
    com.github.decster.jnicompressions.SnappyCompression: byte array interface, block size: 64K, time: 400
               alice29.txt:   Compress:  208M/s Decompress:   322M/s(  538M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  191M/s Decompress:   315M/s(  492M/s) ratio: 64.1%
                   cp.html:   Compress:  303M/s Decompress:   375M/s(  779M/s) ratio: 48.1%
                  fields.c:   Compress:  384M/s Decompress:   368M/s(  867M/s) ratio: 42.4%
             geo.protodata:   Compress:  607M/s Decompress:   362M/s( 1565M/s) ratio: 23.2%
               grammar.lsp:   Compress:  352M/s Decompress:   348M/s(  720M/s) ratio: 48.4%
                 house.jpg:   Compress: 2265M/s Decompress:  4740M/s( 4745M/s) ratio: 99.9%
                      html:   Compress:  528M/s Decompress:   293M/s( 1243M/s) ratio: 23.6%
                  html_x_4:   Compress:  549M/s Decompress:   300M/s( 1274M/s) ratio: 23.6%
               kennedy.xls:   Compress:  451M/s Decompress:   323M/s(  781M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  326M/s Decompress:   240M/s(  628M/s) ratio: 38.3%
                lcet10.txt:   Compress:  222M/s Decompress:   324M/s(  567M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress:  907M/s Decompress:  1830M/s( 2228M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  182M/s Decompress:   320M/s(  468M/s) ratio: 68.4%
                      ptt5:   Compress:  748M/s Decompress:   237M/s( 1303M/s) ratio: 18.2%
                       sum:   Compress:  257M/s Decompress:   361M/s(  695M/s) ratio: 51.9%
                  terasort:   Compress:  935M/s Decompress:   364M/s( 1666M/s) ratio: 21.9%
                  urls.10K:   Compress:  301M/s Decompress:   448M/s(  881M/s) ratio: 50.9%
                   xargs.1:   Compress:  257M/s Decompress:   368M/s(  620M/s) ratio: 59.4%
                     Total:   Compress:  345M/s Decompress:   361M/s(  798M/s) ratio: 45.2%

Just for comparison, here are some test results using C++ instead of Java/JNI.
You can find the test code in another project of mine:
[NativeTask](https://github.com/decster/nativetask)

    [ RUN      ] Perf.RawCompressionLz4
    Block size: 64K
    Compress:  217M/s Decompress:   558M/s(  934M/s) ratio: 59.7% - alice29.txt
    Compress:  204M/s Decompress:   570M/s(  914M/s) ratio: 62.3% - asyoulik.txt
    Compress:  321M/s Decompress:   728M/s( 1505M/s) ratio: 48.4% - cp.html
    Compress:  420M/s Decompress:   545M/s( 1165M/s) ratio: 46.8% - fields.c
    Compress:  832M/s Decompress:   432M/s( 2427M/s) ratio: 17.8% - geo.protodata
    Compress:  350M/s Decompress:   635M/s( 1236M/s) ratio: 51.4% - grammar.lsp
    Compress: 3959M/s Decompress: 13476M/s(13442M/s) ratio: 100.2% - house.jpg
    Compress:  607M/s Decompress:   430M/s( 1974M/s) ratio: 21.8% - html
    Compress:  554M/s Decompress:   377M/s( 1710M/s) ratio: 22.1% - html_x_4
    Compress:  489M/s Decompress:   558M/s( 1532M/s) ratio: 36.4% - kennedy.xls
    Compress:  353M/s Decompress:   397M/s(  860M/s) ratio: 46.1% - kppkn.gtb
    Compress:  223M/s Decompress:   556M/s(  988M/s) ratio: 56.2% - lcet10.txt
    Compress: 1126M/s Decompress:  3431M/s( 4158M/s) ratio: 82.5% - mapreduce-osdi-1.pdf
    Compress:  191M/s Decompress:   604M/s(  904M/s) ratio: 66.8% - plrabn12.txt
    Compress:  826M/s Decompress:   324M/s( 1918M/s) ratio: 16.9% - ptt5
    Compress:  291M/s Decompress:   698M/s( 1419M/s) ratio: 49.2% - sum
    Compress:  978M/s Decompress:   652M/s( 3508M/s) ratio: 18.6% - terasort
    Compress:  319M/s Decompress:   699M/s( 1487M/s) ratio: 47.0% - urls.10K
    Compress:  397M/s Decompress:   779M/s( 1238M/s) ratio: 62.9% - xargs.1
    Compress:  367M/s Decompress:   599M/s( 1392M/s) ratio: 43.0% - Total
    [       OK ] Perf.RawCompressionLz4 (15369 ms)
    [ RUN      ] Perf.RawCompressionSnappy
    Block size: 64K
    Compress:  216M/s Decompress:   350M/s(  584M/s) ratio: 59.8% - alice29.txt
    Compress:  201M/s Decompress:   349M/s(  544M/s) ratio: 64.1% - asyoulik.txt
    Compress:  302M/s Decompress:   496M/s( 1031M/s) ratio: 48.1% - cp.html
    Compress:  395M/s Decompress:   418M/s(  987M/s) ratio: 42.4% - fields.c
    Compress:  700M/s Decompress:   417M/s( 1799M/s) ratio: 23.2% - geo.protodata
    Compress:  352M/s Decompress:   520M/s( 1075M/s) ratio: 48.4% - grammar.lsp
    Compress: 3387M/s Decompress: 14795M/s(14812M/s) ratio: 99.9% - house.jpg
    Compress:  614M/s Decompress:   380M/s( 1611M/s) ratio: 23.6% - html
    Compress:  580M/s Decompress:   377M/s( 1602M/s) ratio: 23.6% - html_x_4
    Compress:  479M/s Decompress:   350M/s(  846M/s) ratio: 41.3% - kennedy.xls
    Compress:  341M/s Decompress:   262M/s(  685M/s) ratio: 38.3% - kppkn.gtb
    Compress:  230M/s Decompress:   346M/s(  606M/s) ratio: 57.1% - lcet10.txt
    Compress: 1161M/s Decompress:  2897M/s( 3528M/s) ratio: 82.1% - mapreduce-osdi-1.pdf
    Compress:  189M/s Decompress:   338M/s(  495M/s) ratio: 68.4% - plrabn12.txt
    Compress:  827M/s Decompress:   276M/s( 1513M/s) ratio: 18.2% - ptt5
    Compress:  295M/s Decompress:   431M/s(  830M/s) ratio: 51.9% - sum
    Compress:  981M/s Decompress:   521M/s( 2385M/s) ratio: 21.9% - terasort
    Compress:  314M/s Decompress:   537M/s( 1055M/s) ratio: 50.9% - urls.10K
    Compress:  337M/s Decompress:   589M/s(  993M/s) ratio: 59.4% - xargs.1
    Compress:  364M/s Decompress:   404M/s(  896M/s) ratio: 45.2% - Total
    [       OK ] Perf.RawCompressionSnappy (17227 ms)
    [----------] 2 tests from Perf (32596 ms total)

