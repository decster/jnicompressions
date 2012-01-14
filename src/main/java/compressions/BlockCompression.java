package compressions;

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
