JNICompressions includes JNI wrappers for some fastest native 
lightweight compression libraries. Currently we have:

lz4: 
http://lz4.googlecode.com/svn/trunk
revision 46

Lz4 is bundled with this library, so you do not need to install lz4
before compile this.

The compression/decompression speed using JNI is almost identical to native 
interface. Generally, byte array interface is a little slower than ByteArray 
interface, because byte array needs to copy to/from native buffer when cross 
JNI boundary, luckily if your buffer size is reasonable(usually 32KB~256KB), 
bulk memory copy is very fast in CPU cache(about 16~24G/s), 
so byte array interface is acceptable in most scenarios.

You can run performance test in your own environment using:

$mvn test 


Here are some test results using some test data included in google snappy:

Test environment:
MacbookPro
Processor Name: Intel Core i5
  Processor Speed:  2.3 GHz
  Number of Processors: 1
  Total Number of Cores:  2
  L2 Cache (per Core):  256 KB
  L3 Cache: 3 MB
  Memory: 4 GB
gcc version 4.2.1 (Apple Inc. build 5659)

Running compressions.BlockCompressionTest
compressions.Lz4Compression: DirectByteBuffer interface, block size: 64K, time: 600
           alice29.txt:   Compress:  229M/s Decompress:   619M/s( 1036M/s) ratio: 59.7%
          asyoulik.txt:   Compress:  213M/s Decompress:   635M/s( 1019M/s) ratio: 62.3%
               cp.html:   Compress:  325M/s Decompress:   718M/s( 1483M/s) ratio: 48.4%
              fields.c:   Compress:  437M/s Decompress:   627M/s( 1340M/s) ratio: 46.8%
         geo.protodata:   Compress:  857M/s Decompress:   467M/s( 2622M/s) ratio: 17.8%
           grammar.lsp:   Compress:  340M/s Decompress:   724M/s( 1408M/s) ratio: 51.4%
             house.jpg:   Compress: 4200M/s Decompress: 11647M/s(11618M/s) ratio: 100.2%
                  html:   Compress:  633M/s Decompress:   455M/s( 2087M/s) ratio: 21.8%
              html_x_4:   Compress:  619M/s Decompress:   437M/s( 1981M/s) ratio: 22.1%
           kennedy.xls:   Compress:  527M/s Decompress:   608M/s( 1668M/s) ratio: 36.4%
             kppkn.gtb:   Compress:  368M/s Decompress:   452M/s(  979M/s) ratio: 46.1%
            lcet10.txt:   Compress:  237M/s Decompress:   608M/s( 1082M/s) ratio: 56.2%
  mapreduce-osdi-1.pdf:   Compress: 1162M/s Decompress:  3752M/s( 4546M/s) ratio: 82.5%
          plrabn12.txt:   Compress:  207M/s Decompress:   653M/s(  977M/s) ratio: 66.8%
                  ptt5:   Compress:  900M/s Decompress:   355M/s( 2101M/s) ratio: 16.9%
                   sum:   Compress:  316M/s Decompress:   747M/s( 1518M/s) ratio: 49.2%
              terasort:   Compress: 1078M/s Decompress:   758M/s( 4080M/s) ratio: 18.6%
              urls.10K:   Compress:  331M/s Decompress:   711M/s( 1513M/s) ratio: 47.0%
               xargs.1:   Compress:  406M/s Decompress:   851M/s( 1354M/s) ratio: 62.9%
                 Total:   Compress:  391M/s Decompress:   651M/s( 1513M/s) ratio: 43.0%

compressions.Lz4Compression: byte array interface, block size: 64K, time: 600
           alice29.txt:   Compress:  223M/s Decompress:   560M/s(  936M/s) ratio: 59.7%
          asyoulik.txt:   Compress:  208M/s Decompress:   561M/s(  900M/s) ratio: 62.3%
               cp.html:   Compress:  303M/s Decompress:   624M/s( 1290M/s) ratio: 48.4%
              fields.c:   Compress:  416M/s Decompress:   548M/s( 1172M/s) ratio: 46.8%
         geo.protodata:   Compress:  792M/s Decompress:   383M/s( 2150M/s) ratio: 17.8%
           grammar.lsp:   Compress:  287M/s Decompress:   539M/s( 1049M/s) ratio: 51.4%
             house.jpg:   Compress: 2416M/s Decompress:  4988M/s( 4976M/s) ratio: 100.2%
                  html:   Compress:  602M/s Decompress:   393M/s( 1804M/s) ratio: 21.8%
              html_x_4:   Compress:  602M/s Decompress:   388M/s( 1759M/s) ratio: 22.1%
           kennedy.xls:   Compress:  504M/s Decompress:   527M/s( 1445M/s) ratio: 36.4%
             kppkn.gtb:   Compress:  353M/s Decompress:   413M/s(  894M/s) ratio: 46.1%
            lcet10.txt:   Compress:  231M/s Decompress:   550M/s(  979M/s) ratio: 56.2%
  mapreduce-osdi-1.pdf:   Compress:  973M/s Decompress:  2474M/s( 2999M/s) ratio: 82.5%
          plrabn12.txt:   Compress:  200M/s Decompress:   587M/s(  879M/s) ratio: 66.8%
                  ptt5:   Compress:  846M/s Decompress:   310M/s( 1837M/s) ratio: 16.9%
                   sum:   Compress:  316M/s Decompress:   667M/s( 1355M/s) ratio: 49.2%
              terasort:   Compress:  992M/s Decompress:   579M/s( 3117M/s) ratio: 18.6%
              urls.10K:   Compress:  327M/s Decompress:   641M/s( 1364M/s) ratio: 47.0%
               xargs.1:   Compress:  377M/s Decompress:   683M/s( 1086M/s) ratio: 62.9%
                 Total:   Compress:  377M/s Decompress:   574M/s( 1334M/s) ratio: 43.0%

Just for comparison, here are some test results using C++ instead of Java/JNI.
You can find the test code in another project of mine:
[NativeTask](https://github.com/decster/nativetask)

[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from Perf
[ RUN      ] Perf.RawCompressionLz4
12/01/14 14:11:12 INFO Block size: 64K
12/01/14 14:11:12 INFO Compress:  228M/s Decompress:   614M/s( 1028M/s) ratio: 59.7% - /Users/decster/projects/jnicompressions/testdata/alice29.txt
12/01/14 14:11:12 INFO Compress:  216M/s Decompress:   635M/s( 1019M/s) ratio: 62.3% - /Users/decster/projects/jnicompressions/testdata/asyoulik.txt
12/01/14 14:11:13 INFO Compress:  344M/s Decompress:   753M/s( 1556M/s) ratio: 48.4% - /Users/decster/projects/jnicompressions/testdata/cp.html
12/01/14 14:11:13 INFO Compress:  356M/s Decompress:   637M/s( 1362M/s) ratio: 46.8% - /Users/decster/projects/jnicompressions/testdata/fields.c
12/01/14 14:11:13 INFO Compress:  842M/s Decompress:   452M/s( 2543M/s) ratio: 17.8% - /Users/decster/projects/jnicompressions/testdata/geo.protodata
12/01/14 14:11:13 INFO Compress:  482M/s Decompress:   792M/s( 1541M/s) ratio: 51.4% - /Users/decster/projects/jnicompressions/testdata/grammar.lsp
12/01/14 14:11:13 INFO Compress: 4653M/s Decompress: 14962M/s(14925M/s) ratio: 100.2% - /Users/decster/projects/jnicompressions/testdata/house.jpg
12/01/14 14:11:13 INFO Compress:  634M/s Decompress:   456M/s( 2092M/s) ratio: 21.8% - /Users/decster/projects/jnicompressions/testdata/html
12/01/14 14:11:13 INFO Compress:  622M/s Decompress:   455M/s( 2062M/s) ratio: 22.1% - /Users/decster/projects/jnicompressions/testdata/html_x_4
12/01/14 14:11:14 INFO Compress:  531M/s Decompress:   610M/s( 1675M/s) ratio: 36.4% - /Users/decster/projects/jnicompressions/testdata/kennedy.xls
12/01/14 14:11:14 INFO Compress:  369M/s Decompress:   437M/s(  948M/s) ratio: 46.1% - /Users/decster/projects/jnicompressions/testdata/kppkn.gtb
12/01/14 14:11:15 INFO Compress:  236M/s Decompress:   605M/s( 1076M/s) ratio: 56.2% - /Users/decster/projects/jnicompressions/testdata/lcet10.txt
12/01/14 14:11:15 INFO Compress: 1154M/s Decompress:  3679M/s( 4458M/s) ratio: 82.5% - /Users/decster/projects/jnicompressions/testdata/mapreduce-osdi-1.pdf
12/01/14 14:11:16 INFO Compress:  203M/s Decompress:   650M/s(  972M/s) ratio: 66.8% - /Users/decster/projects/jnicompressions/testdata/plrabn12.txt
12/01/14 14:11:17 INFO Compress:  902M/s Decompress:   364M/s( 2156M/s) ratio: 16.9% - /Users/decster/projects/jnicompressions/testdata/ptt5
12/01/14 14:11:17 INFO Compress:  314M/s Decompress:   747M/s( 1518M/s) ratio: 49.2% - /Users/decster/projects/jnicompressions/testdata/sum
12/01/14 14:11:17 INFO Compress: 1070M/s Decompress:   694M/s( 3737M/s) ratio: 18.6% - /Users/decster/projects/jnicompressions/testdata/terasort
12/01/14 14:11:18 INFO Compress:  340M/s Decompress:   758M/s( 1612M/s) ratio: 47.0% - /Users/decster/projects/jnicompressions/testdata/urls.10K
12/01/14 14:11:18 INFO Compress:  393M/s Decompress:   861M/s( 1369M/s) ratio: 62.9% - /Users/decster/projects/jnicompressions/testdata/xargs.1
12/01/14 14:11:18 INFO Compress:  392M/s Decompress:   657M/s( 1529M/s) ratio: 43.0% - Total
[       OK ] Perf.RawCompressionLz4 (5794 ms)
[ RUN      ] Perf.RawCompressionSnappy
12/01/14 14:11:18 INFO Block size: 64K
12/01/14 14:11:18 INFO Compress:  134M/s Decompress:   368M/s(  616M/s) ratio: 59.8% - /Users/decster/projects/jnicompressions/testdata/alice29.txt
12/01/14 14:11:19 INFO Compress:  130M/s Decompress:   371M/s(  578M/s) ratio: 64.1% - /Users/decster/projects/jnicompressions/testdata/asyoulik.txt
12/01/14 14:11:19 INFO Compress:  193M/s Decompress:   469M/s(  974M/s) ratio: 48.1% - /Users/decster/projects/jnicompressions/testdata/cp.html
12/01/14 14:11:19 INFO Compress:  212M/s Decompress:   408M/s(  961M/s) ratio: 42.4% - /Users/decster/projects/jnicompressions/testdata/fields.c
12/01/14 14:11:19 INFO Compress:  434M/s Decompress:   445M/s( 1923M/s) ratio: 23.2% - /Users/decster/projects/jnicompressions/testdata/geo.protodata
12/01/14 14:11:19 INFO Compress:  277M/s Decompress:   523M/s( 1082M/s) ratio: 48.4% - /Users/decster/projects/jnicompressions/testdata/grammar.lsp
12/01/14 14:11:19 INFO Compress: 3107M/s Decompress: 17719M/s(17740M/s) ratio: 99.9% - /Users/decster/projects/jnicompressions/testdata/house.jpg
12/01/14 14:11:19 INFO Compress:  362M/s Decompress:   375M/s( 1589M/s) ratio: 23.6% - /Users/decster/projects/jnicompressions/testdata/html
12/01/14 14:11:20 INFO Compress:  361M/s Decompress:   366M/s( 1554M/s) ratio: 23.6% - /Users/decster/projects/jnicompressions/testdata/html_x_4
12/01/14 14:11:21 INFO Compress:  309M/s Decompress:   371M/s(  896M/s) ratio: 41.3% - /Users/decster/projects/jnicompressions/testdata/kennedy.xls
12/01/14 14:11:22 INFO Compress:  192M/s Decompress:   277M/s(  724M/s) ratio: 38.3% - /Users/decster/projects/jnicompressions/testdata/kppkn.gtb
12/01/14 14:11:23 INFO Compress:  141M/s Decompress:   374M/s(  656M/s) ratio: 57.1% - /Users/decster/projects/jnicompressions/testdata/lcet10.txt
12/01/14 14:11:23 INFO Compress:  737M/s Decompress:  3315M/s( 4035M/s) ratio: 82.1% - /Users/decster/projects/jnicompressions/testdata/mapreduce-osdi-1.pdf
12/01/14 14:11:25 INFO Compress:  125M/s Decompress:   369M/s(  540M/s) ratio: 68.4% - /Users/decster/projects/jnicompressions/testdata/plrabn12.txt
12/01/14 14:11:25 INFO Compress:  522M/s Decompress:   291M/s( 1595M/s) ratio: 18.2% - /Users/decster/projects/jnicompressions/testdata/ptt5
12/01/14 14:11:26 INFO Compress:  192M/s Decompress:   454M/s(  875M/s) ratio: 51.9% - /Users/decster/projects/jnicompressions/testdata/sum
12/01/14 14:11:26 INFO Compress:  737M/s Decompress:   546M/s( 2496M/s) ratio: 21.9% - /Users/decster/projects/jnicompressions/testdata/terasort
12/01/14 14:11:27 INFO Compress:  211M/s Decompress:   539M/s( 1060M/s) ratio: 50.9% - /Users/decster/projects/jnicompressions/testdata/urls.10K
12/01/14 14:11:27 INFO Compress:  244M/s Decompress:   578M/s(  974M/s) ratio: 59.4% - /Users/decster/projects/jnicompressions/testdata/xargs.1
12/01/14 14:11:27 INFO Compress:  233M/s Decompress:   426M/s(  944M/s) ratio: 45.2% - Total

