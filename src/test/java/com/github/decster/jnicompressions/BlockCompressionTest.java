/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.github.decster.jnicompressions;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Random;

import com.github.decster.jnicompressions.BlockCompression;
import com.github.decster.jnicompressions.Lz4Compression;

import junit.framework.TestCase;

public class BlockCompressionTest extends TestCase {

  static void CompressDecompress(BlockCompression bc, byte [] input) {
    byte [] output = new byte[bc.MaxCompressedSize(input.length)];
    byte [] decomp = new byte [input.length];
    int length = bc.Compress(input, 0, input.length, output, 0);
    assertTrue(length>0);
    int decomplen = bc.Decompress(output, 0, length, decomp, 0);
    assertEquals(input.length, decomplen);
    for (int i=0;i<input.length;i++) {
      assertEquals(input[i], decomp[i]);
    }
  }

  static void CompressDecompressDirect(BlockCompression bc, byte [] input) {
    ByteBuffer inputb = ByteBuffer.allocateDirect(input.length);
    inputb.put(input);
    ByteBuffer outputb = ByteBuffer.allocateDirect(bc.MaxCompressedSize(input.length));
    ByteBuffer decompb = ByteBuffer.allocateDirect(input.length);
    int length = bc.CompressDirect(inputb, 0, input.length, outputb, 0);
    assertTrue(length>0);
    int decomplen = bc.DecompressDirect(outputb, 0, length, decompb, 0);
    assertEquals(input.length, decomplen);
    for (int i=0;i<input.length;i++) {
      assertEquals(input[i], decompb.get(i));
    }
  }

  public void testCompression() {
    Random r = new Random();
    StringBuilder sb = new StringBuilder();
    for (int i=0;i<100000;i++) {
      sb.append(Integer.toString(r.nextInt(100000)/977*977));
      sb.append('\t');
      sb.append(Integer.toString(r.nextInt(100000)/957*957));
      sb.append('\n');
    }
    byte [] input = sb.toString().getBytes();
    CompressDecompress(new Lz4Compression(), input);
    CompressDecompressDirect(new Lz4Compression(), input);
    CompressDecompress(new SnappyCompression(), input);
    CompressDecompressDirect(new SnappyCompression(), input);
  }

  public void testPerformance() throws IOException {
    measurePerformance(new Lz4Compression(), 400);
    measurePerformance(new SnappyCompression(), 400);
  }

  public void measurePerformance(BlockCompression bc, int time) throws IOException {
    int blockSize = 64 * 1024;
    File [] allfiles = new File("testdata").listFiles();
    System.out.printf("%s: DirectByteBuffer interface, block size: %dK, time: %d\n", bc
        .getClass().getName(), blockSize / 1024, time);
    MeasureResult total = new MeasureResult();
    for (File f : allfiles) {
      if (f.isFile()) {
        byte [] data = LoadFile(f);
        MeasureResult result = measureDirect(bc, data, blockSize, time);
        System.out.printf("%22s: \t%s\n", f.getName(), result.toString());
        total.add(result);
      }
    }
    System.out.printf("%22s: \t%s\n\n", "Total", total.toString());
    System.out.printf("%s: byte array interface, block size: %dK, time: %d\n", bc
        .getClass().getName(), blockSize / 1024, time);
    total = new MeasureResult();
    for (File f : allfiles) {
      if (f.isFile()) {
        byte [] data = LoadFile(f);
        MeasureResult result = measure(bc, data, blockSize, time);
        System.out.printf("%22s: \t%s\n", f.getName(), result.toString());
        total.add(result);
      }
    }
    System.out.printf("%22s: \t%s\n\n", "Total", total.toString());
  }
  
  static byte [] LoadFile(File path) throws IOException {
    FileInputStream fin = new FileInputStream(path);
    long length = path.length();
    byte [] ret = new byte[(int)length];
    int rd = fin.read(ret);
    if (rd != length) {
      throw new IOException("read error");
    }
    return ret;
  }
  
  static class MeasureResult {
    public long compressTime;   // ns
    public long uncompressTime; // ns
    public long uncompressedSize; // bytes
    public long compressedSize;   // bytes

    public String toString() {
      return String.format(
          "Compress: %4.0fM/s Decompress: %5.0fM/s(%5.0fM/s) ratio: %.1f%%",
          (uncompressedSize / 1024.0 / 1024) / (compressTime / 1000000000.),
          (compressedSize / 1024.0 / 1024) / (uncompressTime / 1000000000.),
          (uncompressedSize / 1024.0 / 1024) / (uncompressTime / 1000000000.),
          compressedSize / (float) uncompressedSize * 100);
    }
    
    public void add(MeasureResult rhs) {
      compressTime += rhs.compressTime;
      uncompressTime += rhs.uncompressTime;
      uncompressedSize += rhs.uncompressedSize;
      compressedSize += rhs.compressedSize;
    }
  }

  public MeasureResult measure(BlockCompression bc, byte [] data, int blocksize, int time) {
    MeasureResult result = new MeasureResult();
    byte [] dest = new byte[bc.MaxCompressedSize(blocksize)];
    byte [] decomp = new byte[blocksize];
    for (int start = 0; start < data.length; start += blocksize) {
      int length = Math.min(data.length - start, blocksize);
      long startTime = System.nanoTime();
      int compressedSize = 0;
      for (int i=0; i< time; i++) {
        compressedSize = bc.Compress(data, start, length, dest, 0);
        result.uncompressedSize += length;
        result.compressedSize += compressedSize;
      }
      long endTime = System.nanoTime();
      result.compressTime += endTime - startTime;

      startTime = System.nanoTime();
      for (int i=0; i< time; i++) {
        int osize = bc.Decompress(dest, 0, compressedSize, decomp, 0);
      }
      endTime = System.nanoTime();
      result.uncompressTime += endTime - startTime;
    }
    return result;
  }

  public MeasureResult measureDirect(BlockCompression bc, byte [] rawData, int blocksize, int time) {
    MeasureResult result = new MeasureResult();
    ByteBuffer data = ByteBuffer.allocateDirect(rawData.length);
    data.put(rawData);
    ByteBuffer dest = ByteBuffer.allocateDirect(bc.MaxCompressedSize(blocksize));
    ByteBuffer decomp = ByteBuffer.allocateDirect(blocksize);
    for (int start = 0; start < data.position(); start += blocksize) {
      int length = Math.min(data.position() - start, blocksize);
      long startTime = System.nanoTime();
      int compressedSize = 0;
      for (int i=0; i< time; i++) {
        compressedSize = bc.CompressDirect(data, start, length, dest, 0);
        result.uncompressedSize += length;
        result.compressedSize += compressedSize;
      }
      long endTime = System.nanoTime();
      result.compressTime += endTime - startTime;

      startTime = System.nanoTime();
      for (int i=0; i< time; i++) {
        int osize = bc.DecompressDirect(dest, 0, compressedSize, decomp, 0);
      }
      endTime = System.nanoTime();
      result.uncompressTime += endTime - startTime;
    }
    return result;
  }
}

