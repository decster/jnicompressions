package compressions;

import java.nio.ByteBuffer;

public class Lz4Compression implements BlockCompression {
  public Lz4Compression() {
    NativeCompression.CheckNativeLibrary();
  } 

  @Override
  public int MaxCompressedSize(int uncompressedSize) {
    return Math.max((int)(uncompressedSize * 0.005), 8) + uncompressedSize;
  }

  @Override
  public native int Compress(byte[] src, int srcOffset, int srcLength, byte[] dest,
      int destOffset);

  @Override
  public native int Decompress(byte[] src, int srcOffset, int srcLength, byte[] dest,
      int destOffset);

  @Override
  public native int CompressDirect(ByteBuffer src, int srcOffset, int srcLength,
      ByteBuffer dest, int destOffset);

  @Override
  public native int DecompressDirect(ByteBuffer src, int srcOffset, int srcLength,
      ByteBuffer dest, int destOffset);
}
