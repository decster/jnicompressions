
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
  char* in = (char*) jenv->GetPrimitiveArrayCritical(src, 0);
  char* out = (char*) jenv->GetPrimitiveArrayCritical(dest, 0);
  if(in == 0 || out == 0) {
    return -1;
  }
  size_t compressedLength;
  snappy::RawCompress(in + srcOffset, (size_t) srcLength, out + destOffset, &compressedLength);
  jenv->ReleasePrimitiveArrayCritical(src, in, 0);
  jenv->ReleasePrimitiveArrayCritical(dest, out, 0);
  return (jint) compressedLength;
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
  char* in = (char*) jenv->GetPrimitiveArrayCritical(src, 0);
  char* out = (char*) jenv->GetPrimitiveArrayCritical(dest, 0);
  if(in == 0 || out == 0) {
    // out of memory
    return -1;
  }
  size_t uncompressedLength;
  snappy::GetUncompressedLength(in + srcOffset, (size_t) srcLength, &uncompressedLength);
  bool ret = snappy::RawUncompress(in + srcOffset, (size_t) srcLength, out + destOffset);
  jenv->ReleasePrimitiveArrayCritical(src, in, 0);
  jenv->ReleasePrimitiveArrayCritical(dest, out, 0);
  if(!ret) {
    return -1;
  }
  return (jint) uncompressedLength;
}


extern "C" JNIEXPORT jbyteArray JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_CompressSimple(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src) {
  jsize srcLength = jenv->GetArrayLength(src);
  jbyte * inputBuffer = (jbyte *)jenv->GetPrimitiveArrayCritical(src, 0);
  jbyte * outputBuffer = (jbyte*)malloc(snappy::MaxCompressedLength(srcLength));
  size_t compressedSize = 0;
  snappy::RawCompress((char*) inputBuffer, srcLength, (char*) outputBuffer,
                      &compressedSize);
  jbyteArray dest = NULL;
  if (compressedSize >= 0) {
    dest = jenv->NewByteArray(compressedSize);
    jenv->SetByteArrayRegion(dest, 0, compressedSize, outputBuffer);
  }
  jenv->ReleasePrimitiveArrayCritical(src, inputBuffer, 0);
  free(outputBuffer);
  return dest;
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_com_github_decster_jnicompressions_SnappyCompression_DecompressSimple(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src) {
  jsize srcLength = jenv->GetArrayLength(src);
  jbyte * inputBuffer = (jbyte *)jenv->GetPrimitiveArrayCritical(src, 0);
  size_t uncompressedSize;
  if (!snappy::GetUncompressedLength((const char*)inputBuffer, srcLength,
                                     &uncompressedSize)) {
    free(inputBuffer);
    return NULL;
  }
  jbyteArray dest = jenv->NewByteArray(uncompressedSize);
  jbyte * outputBuffer = (jbyte *)jenv->GetPrimitiveArrayCritical(dest, 0);
  if (snappy::RawUncompress((const char*)inputBuffer, srcLength,
                            (char *)outputBuffer)) {
    jenv->ReleasePrimitiveArrayCritical(src, inputBuffer, 0);
    jenv->ReleasePrimitiveArrayCritical(dest, outputBuffer, 0);
    return dest;
  } else {
    return NULL;
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

