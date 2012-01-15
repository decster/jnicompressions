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

import java.nio.ByteBuffer;

public interface BlockCompression {
  /**
   * Get max possible compressed size for this input block size
   * @param uncompressedSize
   * @return max possible compressed size
   */
  int MaxCompressedSize(int uncompressedSize);

  // byte array interface, because of JNI restriction, 
  // src & dest buffer each will be copied once 
  // to/from native buffer, hence 2 additional malloc/free
  
  /**
   * Compress one block
   * @param src input buffer
   * @param srcOffset input block start offset
   * @param srcLength input block length
   * @param dest output buffer
   * @param destOffset output block start offset
   * @return output block size, < 0 if some error occured
   */
  public int Compress(byte[] src, int srcOffset, int srcLength, byte[] dest,
      int destOffset);

  /**
   * Decompress one block
   * @param src input buffer
   * @param srcOffset input block start offset
   * @param srcLength input block length
   * @param dest output buffer
   * @param destOffset output block start offset
   * @return output block size, < 0 if some error occured
   */
  public int Decompress(byte[] src, int srcOffset, int srcLength, byte[] dest,
      int destOffset);

  // DirectByteBuffer interface, no buffer copy overhead

  /**
   * Compress one block
   * @param src input buffer, must be DirectByteBuffer
   * @param srcOffset input block start offset
   * @param srcLength input block length
   * @param dest output buffer, must be DirectByteBuffer
   * @param destOffset output block start offset
   * @return output block size, < 0 if some error occured
   */
  public int CompressDirect(ByteBuffer src, int srcOffset, int srcLength,
      ByteBuffer dest, int destOffset);

  /**
   * Decompress one block
   * @param src input buffer, must be DirectByteBuffer
   * @param srcOffset input block start offset
   * @param srcLength input block length
   * @param dest output buffer, must be DirectByteBuffer
   * @param destOffset output block start offset
   * @return output block size, < 0 if some error occured
   */
  public int DecompressDirect(ByteBuffer src, int srcOffset, int srcLength,
      ByteBuffer dest, int destOffset);
}
