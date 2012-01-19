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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <jni.h>

extern "C" {
extern int LZ4_compress   (char* source, char* dest, int isize);
extern int LZ4_uncompress (char* source, char* dest, int osize);
extern int LZ4_uncompress_unknownOutputSize(char* source, char* dest,
                                            int isize, int maxOutputSize);
}


static int MaxLz4CompressedSize(int uncompressedSize) {
  int overhead = (int)(uncompressedSize * 0.005);
  if (overhead < 8) {
    overhead = 8;
  }
  return uncompressedSize + overhead;
}

/*
 * Method:    Compress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_Compress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jbyte * outputBuffer = (jbyte*)malloc(4+MaxLz4CompressedSize(srcLength));
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  jint osize = LZ4_compress((char*) inputBuffer, (char*) outputBuffer + 4,
                            srcLength);
  if (osize >= 0) {
    *(uint32_t*)outputBuffer = (uint32_t)srcLength;
    jenv->SetByteArrayRegion(dest, destOffset, osize + 4, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return osize + 4;
}

/*
 * Method:    Decompress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_Decompress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  if (srcLength < 4) {
    return -1;
  }
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jsize length = jenv->GetArrayLength(dest);
  jbyte * outputBuffer = (jbyte*)malloc(length - destOffset);
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  jint osize =
      LZ4_uncompress_unknownOutputSize((char*)inputBuffer+4,
                                       (char*)outputBuffer,
                                       srcLength-4,
                                       length - destOffset);
  if (osize >= 0) {
    jenv->SetByteArrayRegion(dest, destOffset, osize, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return osize;
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_CompressSimple(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src) {
  jsize srcLength = jenv->GetArrayLength(src);
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jbyte * outputBuffer = (jbyte*)malloc(4+MaxLz4CompressedSize(srcLength));
  jenv->GetByteArrayRegion(src, 0, srcLength, inputBuffer);
  jint osize = LZ4_compress((char*) inputBuffer, (char*) outputBuffer + 4,
                            srcLength);
  jbyteArray dest = NULL;
  if (osize >= 0) {
    *(uint32_t*)outputBuffer = (uint32_t)srcLength;
    dest = jenv->NewByteArray(osize+4);
    jenv->SetByteArrayRegion(dest, 0, osize+4, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return dest;
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_DecompressSimple(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src) {
  jsize srcLength = jenv->GetArrayLength(src);
  if (srcLength < 4) {
    return NULL;
  }
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jenv->GetByteArrayRegion(src, 0, srcLength, inputBuffer);
  jsize length = *(uint32_t*)inputBuffer;
  jbyte * outputBuffer = (jbyte*)malloc(length);
  jint osize =
      LZ4_uncompress_unknownOutputSize((char*)inputBuffer+4,
                                       (char*)outputBuffer,
                                       srcLength-4,
                                       length);
  jbyteArray dest = NULL;
  if (osize >= 0) {
    dest = jenv->NewByteArray(osize);
    jenv->SetByteArrayRegion(dest, 0, osize, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return dest;
}

/*
 * Method:    CompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_CompressDirect(
    JNIEnv * jenv,
    jobject obj,
    jobject src,
    jint srcOffset,
    jint srcLength,
    jobject dest,
    jint destOffset) {
  jbyte * srcBuffer = (jbyte*)jenv->GetDirectBufferAddress(src);
  jbyte * destBuffer = (jbyte*)jenv->GetDirectBufferAddress(dest);
  jsize destBufferSize = jenv->GetDirectBufferCapacity(dest);
  jint osize = LZ4_compress((char*) srcBuffer + srcOffset,
                            (char*) destBuffer + destOffset + 4, srcLength);
  *(uint32_t*)destBuffer = osize;
  return osize + 4;
}

/*
 * Method:    DecompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_Lz4Compression_DecompressDirect(
    JNIEnv * jenv,
    jobject obj,
    jobject src,
    jint srcOffset,
    jint srcLength,
    jobject dest,
    jint destOffset) {
  if (srcLength < 4) {
    return -1;
  }
  jbyte * srcBuffer = (jbyte*)jenv->GetDirectBufferAddress(src);
  jbyte * destBuffer = (jbyte*)jenv->GetDirectBufferAddress(dest);
  jsize destBufferSize = jenv->GetDirectBufferCapacity(dest);
  jint osize =
      LZ4_uncompress_unknownOutputSize((char*) srcBuffer + srcOffset + 4,
                                       (char*) destBuffer + destOffset,
                                       srcLength - 4, destBufferSize - destOffset);
  return osize;
}

