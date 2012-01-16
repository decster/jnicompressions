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
               alice29.txt:   Compress:  232M/s Decompress:   540M/s(  904M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  211M/s Decompress:   544M/s(  872M/s) ratio: 62.3%
                   cp.html:   Compress:  310M/s Decompress:   703M/s( 1452M/s) ratio: 48.4%
                  fields.c:   Compress:  434M/s Decompress:   540M/s( 1154M/s) ratio: 46.8%
             geo.protodata:   Compress:  826M/s Decompress:   478M/s( 2688M/s) ratio: 17.8%
               grammar.lsp:   Compress:  429M/s Decompress:   789M/s( 1536M/s) ratio: 51.4%
                 house.jpg:   Compress: 4158M/s Decompress: 10743M/s(10717M/s) ratio: 100.2%
                      html:   Compress:  648M/s Decompress:   425M/s( 1949M/s) ratio: 21.8%
                  html_x_4:   Compress:  626M/s Decompress:   417M/s( 1891M/s) ratio: 22.1%
               kennedy.xls:   Compress:  489M/s Decompress:   461M/s( 1265M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  386M/s Decompress:   392M/s(  849M/s) ratio: 46.1%
                lcet10.txt:   Compress:  240M/s Decompress:   511M/s(  908M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress: 1065M/s Decompress:  3301M/s( 4000M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  199M/s Decompress:   585M/s(  876M/s) ratio: 66.8%
                      ptt5:   Compress:  892M/s Decompress:   362M/s( 2144M/s) ratio: 16.9%
                       sum:   Compress:  324M/s Decompress:   632M/s( 1285M/s) ratio: 49.2%
                  terasort:   Compress: 1120M/s Decompress:   655M/s( 3524M/s) ratio: 18.6%
                  urls.10K:   Compress:  338M/s Decompress:   652M/s( 1388M/s) ratio: 47.0%
                   xargs.1:   Compress:  359M/s Decompress:   921M/s( 1464M/s) ratio: 62.9%
                     Total:   Compress:  386M/s Decompress:   566M/s( 1316M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.Lz4Compression: byte array interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  225M/s Decompress:   476M/s(  797M/s) ratio: 59.7%
              asyoulik.txt:   Compress:  208M/s Decompress:   514M/s(  825M/s) ratio: 62.3%
                   cp.html:   Compress:  294M/s Decompress:   630M/s( 1302M/s) ratio: 48.4%
                  fields.c:   Compress:  400M/s Decompress:   489M/s( 1045M/s) ratio: 46.8%
             geo.protodata:   Compress:  777M/s Decompress:   405M/s( 2275M/s) ratio: 17.8%
               grammar.lsp:   Compress:  434M/s Decompress:   405M/s(  788M/s) ratio: 51.4%
                 house.jpg:   Compress: 2733M/s Decompress:  4509M/s( 4497M/s) ratio: 100.2%
                      html:   Compress:  607M/s Decompress:   373M/s( 1712M/s) ratio: 21.8%
                  html_x_4:   Compress:  590M/s Decompress:   361M/s( 1635M/s) ratio: 22.1%
               kennedy.xls:   Compress:  468M/s Decompress:   408M/s( 1121M/s) ratio: 36.4%
                 kppkn.gtb:   Compress:  365M/s Decompress:   363M/s(  786M/s) ratio: 46.1%
                lcet10.txt:   Compress:  235M/s Decompress:   463M/s(  823M/s) ratio: 56.2%
      mapreduce-osdi-1.pdf:   Compress:  968M/s Decompress:  2282M/s( 2766M/s) ratio: 82.5%
              plrabn12.txt:   Compress:  193M/s Decompress:   539M/s(  806M/s) ratio: 66.8%
                      ptt5:   Compress:  815M/s Decompress:   321M/s( 1900M/s) ratio: 16.9%
                       sum:   Compress:  304M/s Decompress:   600M/s( 1220M/s) ratio: 49.2%
                  terasort:   Compress: 1004M/s Decompress:   499M/s( 2684M/s) ratio: 18.6%
                  urls.10K:   Compress:  326M/s Decompress:   572M/s( 1218M/s) ratio: 47.0%
                   xargs.1:   Compress:  339M/s Decompress:   764M/s( 1215M/s) ratio: 62.9%
                     Total:   Compress:  371M/s Decompress:   503M/s( 1169M/s) ratio: 43.0%
    
    com.github.decster.jnicompressions.SnappyCompression: DirectByteBuffer interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  216M/s Decompress:   338M/s(  565M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  199M/s Decompress:   347M/s(  541M/s) ratio: 64.1%
                   cp.html:   Compress:  300M/s Decompress:   454M/s(  944M/s) ratio: 48.1%
                  fields.c:   Compress:  396M/s Decompress:   333M/s(  786M/s) ratio: 42.4%
             geo.protodata:   Compress:  693M/s Decompress:   430M/s( 1855M/s) ratio: 23.2%
               grammar.lsp:   Compress:  339M/s Decompress:   499M/s( 1031M/s) ratio: 48.4%
                 house.jpg:   Compress: 3243M/s Decompress: 12267M/s(12282M/s) ratio: 99.9%
                      html:   Compress:  590M/s Decompress:   357M/s( 1515M/s) ratio: 23.6%
                  html_x_4:   Compress:  576M/s Decompress:   348M/s( 1478M/s) ratio: 23.6%
               kennedy.xls:   Compress:  474M/s Decompress:   346M/s(  836M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  333M/s Decompress:   263M/s(  687M/s) ratio: 38.3%
                lcet10.txt:   Compress:  229M/s Decompress:   342M/s(  599M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress: 1042M/s Decompress:  2798M/s( 3406M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  183M/s Decompress:   330M/s(  483M/s) ratio: 68.4%
                      ptt5:   Compress:  814M/s Decompress:   274M/s( 1507M/s) ratio: 18.2%
                       sum:   Compress:  290M/s Decompress:   422M/s(  814M/s) ratio: 51.9%
                  terasort:   Compress:  946M/s Decompress:   534M/s( 2442M/s) ratio: 21.9%
                  urls.10K:   Compress:  309M/s Decompress:   501M/s(  985M/s) ratio: 50.9%
                   xargs.1:   Compress:  286M/s Decompress:   504M/s(  849M/s) ratio: 59.4%
                     Total:   Compress:  359M/s Decompress:   394M/s(  873M/s) ratio: 45.2%
    
    com.github.decster.jnicompressions.SnappyCompression: byte array interface, block size: 64K, time: 1000
               alice29.txt:   Compress:  208M/s Decompress:   313M/s(  523M/s) ratio: 59.8%
              asyoulik.txt:   Compress:  196M/s Decompress:   326M/s(  508M/s) ratio: 64.1%
                   cp.html:   Compress:  280M/s Decompress:   419M/s(  870M/s) ratio: 48.1%
                  fields.c:   Compress:  375M/s Decompress:   313M/s(  738M/s) ratio: 42.4%
             geo.protodata:   Compress:  612M/s Decompress:   352M/s( 1519M/s) ratio: 23.2%
               grammar.lsp:   Compress:  356M/s Decompress:   338M/s(  698M/s) ratio: 48.4%
                 house.jpg:   Compress: 1976M/s Decompress:  4964M/s( 4970M/s) ratio: 99.9%
                      html:   Compress:  543M/s Decompress:   307M/s( 1302M/s) ratio: 23.6%
                  html_x_4:   Compress:  554M/s Decompress:   313M/s( 1331M/s) ratio: 23.6%
               kennedy.xls:   Compress:  452M/s Decompress:   317M/s(  767M/s) ratio: 41.3%
                 kppkn.gtb:   Compress:  321M/s Decompress:   242M/s(  632M/s) ratio: 38.3%
                lcet10.txt:   Compress:  224M/s Decompress:   328M/s(  573M/s) ratio: 57.1%
      mapreduce-osdi-1.pdf:   Compress:  986M/s Decompress:  2137M/s( 2601M/s) ratio: 82.1%
              plrabn12.txt:   Compress:  184M/s Decompress:   320M/s(  467M/s) ratio: 68.4%
                      ptt5:   Compress:  789M/s Decompress:   237M/s( 1302M/s) ratio: 18.2%
                       sum:   Compress:  273M/s Decompress:   371M/s(  716M/s) ratio: 51.9%
                  terasort:   Compress:  910M/s Decompress:   448M/s( 2051M/s) ratio: 21.9%
                  urls.10K:   Compress:  300M/s Decompress:   452M/s(  887M/s) ratio: 50.9%
                   xargs.1:   Compress:  308M/s Decompress:   525M/s(  884M/s) ratio: 59.4%
                     Total:   Compress:  348M/s Decompress:   363M/s(  803M/s) ratio: 45.2%


Just for comparison, here are some test results using C++ instead of Java/JNI.
You can find the test code in another project of mine:
[NativeTask](https://github.com/decster/nativetask)

    Compress:  232M/s Decompress:   612M/s( 1024M/s) ratio: 59.7% - alice29.txt
    Compress:  214M/s Decompress:   630M/s( 1011M/s) ratio: 62.3% - asyoulik.txt
    Compress:  317M/s Decompress:   822M/s( 1698M/s) ratio: 48.4% - cp.html
    Compress:  414M/s Decompress:   639M/s( 1367M/s) ratio: 46.8% - fields.c
    Compress:  816M/s Decompress:   570M/s( 3201M/s) ratio: 17.8% - geo.protodata
    Compress:  441M/s Decompress:   799M/s( 1556M/s) ratio: 51.4% - grammar.lsp
    Compress: 3672M/s Decompress: 14519M/s(14483M/s) ratio: 100.2% - house.jpg
    Compress:  647M/s Decompress:   511M/s( 2342M/s) ratio: 21.8% - html
    Compress:  647M/s Decompress:   519M/s( 2353M/s) ratio: 22.1% - html_x_4
    Compress:  497M/s Decompress:   592M/s( 1624M/s) ratio: 36.4% - kennedy.xls
    Compress:  389M/s Decompress:   424M/s(  919M/s) ratio: 46.1% - kppkn.gtb
    Compress:  243M/s Decompress:   600M/s( 1068M/s) ratio: 56.2% - lcet10.txt
    Compress: 1075M/s Decompress:  3678M/s( 4457M/s) ratio: 82.5% - mapreduce-osdi-1.pdf
    Compress:  197M/s Decompress:   608M/s(  909M/s) ratio: 66.8% - plrabn12.txt
    Compress:  892M/s Decompress:   411M/s( 2432M/s) ratio: 16.9% - ptt5
    Compress:  319M/s Decompress:   688M/s( 1399M/s) ratio: 49.2% - sum
    Compress: 1047M/s Decompress:   653M/s( 3513M/s) ratio: 18.6% - terasort
    Compress:  305M/s Decompress:   697M/s( 1484M/s) ratio: 47.0% - urls.10K
    Compress:  326M/s Decompress:   796M/s( 1266M/s) ratio: 62.9% - xargs.1
    Compress:  380M/s Decompress:   649M/s( 1510M/s) ratio: 43.0% - Total
    [       OK ] Perf.RawCompressionLz4 (14695 ms)
    [ RUN      ] Perf.RawCompressionSnappy
    Block size: 64K
    Compress:  211M/s Decompress:   343M/s(  574M/s) ratio: 59.8% - alice29.txt
    Compress:  204M/s Decompress:   331M/s(  516M/s) ratio: 64.1% - asyoulik.txt
    Compress:  305M/s Decompress:   504M/s( 1048M/s) ratio: 48.1% - cp.html
    Compress:  386M/s Decompress:   387M/s(  913M/s) ratio: 42.4% - fields.c
    Compress:  700M/s Decompress:   469M/s( 2025M/s) ratio: 23.2% - geo.protodata
    Compress:  414M/s Decompress:   511M/s( 1057M/s) ratio: 48.4% - grammar.lsp
    Compress: 3139M/s Decompress: 14031M/s(14048M/s) ratio: 99.9% - house.jpg
    Compress:  590M/s Decompress:   374M/s( 1587M/s) ratio: 23.6% - html
    Compress:  578M/s Decompress:   367M/s( 1558M/s) ratio: 23.6% - html_x_4
    Compress:  477M/s Decompress:   344M/s(  832M/s) ratio: 41.3% - kennedy.xls
    Compress:  338M/s Decompress:   262M/s(  684M/s) ratio: 38.3% - kppkn.gtb
    Compress:  225M/s Decompress:   339M/s(  594M/s) ratio: 57.1% - lcet10.txt
    Compress: 1143M/s Decompress:  2836M/s( 3452M/s) ratio: 82.1% - mapreduce-osdi-1.pdf
    Compress:  186M/s Decompress:   337M/s(  493M/s) ratio: 68.4% - plrabn12.txt
    Compress:  793M/s Decompress:   275M/s( 1512M/s) ratio: 18.2% - ptt5
    Compress:  286M/s Decompress:   380M/s(  733M/s) ratio: 51.9% - sum
    Compress:  986M/s Decompress:   490M/s( 2242M/s) ratio: 21.9% - terasort
    Compress:  309M/s Decompress:   522M/s( 1026M/s) ratio: 50.9% - urls.10K
    Compress:  309M/s Decompress:   521M/s(  877M/s) ratio: 59.4% - xargs.1
    Compress:  359M/s Decompress:   398M/s(  881M/s) ratio: 45.2% - Total

