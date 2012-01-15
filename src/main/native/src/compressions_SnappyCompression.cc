
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

#include <string.h>
#include <stdlib.h>
#include <jni.h>
#include "../snappy/snappy.h"

/*
 * Method:    Compress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_Compress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  jsize srcBufferSize = jenv->GetArrayLength(src);
  if (srcOffset + srcLength > srcBufferSize) {
    return -1;
  }
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jbyte * outputBuffer = (jbyte*)malloc(snappy::MaxCompressedLength(srcLength));
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  size_t compressedSize = 0;
  snappy::RawCompress((char*) inputBuffer, srcLength, (char*) outputBuffer,
                      &compressedSize);
  if (compressedSize >= 0) {
    jenv->SetByteArrayRegion(dest, destOffset, compressedSize, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return compressedSize;
}

/*
 * Method:    Decompress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_Decompress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  jsize srcBufferSize = jenv->GetArrayLength(src);
  if (srcOffset + srcLength > srcBufferSize) {
    return -1;
  }
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  size_t uncompressedSize;
  if (!snappy::GetUncompressedLength((const char*)inputBuffer, srcLength,
                                     &uncompressedSize)) {
    free(inputBuffer);
    return -1;
  }
  jsize length = jenv->GetArrayLength(dest);
  if (length < destOffset + uncompressedSize) {
    free(inputBuffer);
    return -2;
  }
  jbyte * outputBuffer = (jbyte*)malloc(uncompressedSize);
  if (snappy::RawUncompress((const char*)inputBuffer, srcLength,
                            (char *)outputBuffer)) {
    jenv->SetByteArrayRegion(dest, destOffset, uncompressedSize, outputBuffer);
    free(inputBuffer);
    free(outputBuffer);
    return uncompressedSize;
  } else {
    free(inputBuffer);
    free(outputBuffer);
    return -1;
  }
}

/*
 * Method:    CompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_CompressDirect(
    JNIEnv * jenv,
    jobject obj,
    jobject src,
    jint srcOffset,
    jint srcLength,
    jobject dest,
    jint destOffset) {
  jbyte * srcBuffer = (jbyte*)jenv->GetDirectBufferAddress(src);
  jbyte * destBuffer = (jbyte*)jenv->GetDirectBufferAddress(dest);
  jsize srcBufferSize = jenv->GetDirectBufferCapacity(src);
  if (srcOffset + srcLength > srcBufferSize) {
    return -1;
  }
  jsize destBufferSize = jenv->GetDirectBufferCapacity(dest);
  size_t compressedSize = 0;
  snappy::RawCompress((char*) srcBuffer+srcOffset, srcLength,
                      (char*) destBuffer+destOffset, &compressedSize);
  return compressedSize;
}

/*
 * Method:    DecompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_DecompressDirect(
    JNIEnv * jenv,
    jobject obj,
    jobject src,
    jint srcOffset,
    jint srcLength,
    jobject dest,
    jint destOffset) {
  jbyte * srcBuffer = (jbyte*)jenv->GetDirectBufferAddress(src);
  jbyte * destBuffer = (jbyte*)jenv->GetDirectBufferAddress(dest);
  jsize srcBufferSize = jenv->GetDirectBufferCapacity(src);
  if (srcOffset + srcLength > srcBufferSize) {
    return -1;
  }
  jsize destBufferSize = jenv->GetDirectBufferCapacity(dest);
  size_t uncompressedSize;
  if (!snappy::GetUncompressedLength((const char*)srcBuffer+srcOffset,
                                     srcLength,
                                     &uncompressedSize)) {
    return -1;
  }
  if (destBufferSize < destOffset + uncompressedSize) {
    return -2;
  }
  if (snappy::RawUncompress((const char*)srcBuffer + srcOffset,
                            srcLength,
                            (char *)destBuffer+destOffset)) {
    return uncompressedSize;
  }
  return -1;
}

