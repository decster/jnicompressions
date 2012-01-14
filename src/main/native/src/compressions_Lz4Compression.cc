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
 * Class:     compressions_Lz4Compression
 * Method:    Compress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_compressions_Lz4Compression_Compress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jbyte * outputBuffer = (jbyte*)malloc(MaxLz4CompressedSize(srcLength));
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  jint osize = LZ4_compress((char*) inputBuffer, (char*) outputBuffer,
                            srcLength);
  if (osize >= 0) {
    jenv->SetByteArrayRegion(dest, destOffset, osize, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return osize;
}

/*
 * Class:     compressions_Lz4Compression
 * Method:    Decompress
 * Signature: ([BII[BI)I
 */
extern "C" JNIEXPORT jint JNICALL Java_compressions_Lz4Compression_Decompress(
    JNIEnv * jenv,
    jobject obj,
    jbyteArray src,
    jint srcOffset,
    jint srcLength,
    jbyteArray dest,
    jint destOffset) {
  jbyte * inputBuffer = (jbyte*)malloc(srcLength);
  jsize length = jenv->GetArrayLength(dest);
  jbyte * outputBuffer = (jbyte*)malloc(length - destOffset);
  jenv->GetByteArrayRegion(src, srcOffset, srcLength, inputBuffer);
  jint osize =
      LZ4_uncompress_unknownOutputSize((char*)inputBuffer,
                                       (char*)outputBuffer,
                                       srcLength,
                                       length - destOffset);
  if (osize >= 0) {
    jenv->SetByteArrayRegion(dest, destOffset, osize, outputBuffer);
  }
  free(inputBuffer);
  free(outputBuffer);
  return osize;
}

/*
 * Class:     compressions_Lz4Compression
 * Method:    CompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_compressions_Lz4Compression_CompressDirect(
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
                            (char*) destBuffer + destOffset, srcLength);
  return osize;
}

/*
 * Class:     compressions_Lz4Compression
 * Method:    DecompressDirect
 * Signature: (Ljava/nio/ByteBuffer;IILjava/nio/ByteBuffer;I)I
 */
extern "C" JNIEXPORT jint JNICALL Java_compressions_Lz4Compression_DecompressDirect(
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
  jint osize =
      LZ4_uncompress_unknownOutputSize((char*) srcBuffer + srcOffset,
                                       (char*) destBuffer + destOffset,
                                       srcLength, destBufferSize - destOffset);
  return osize;
}

