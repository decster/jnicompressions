package compressions;

public class NativeCompression {
  static boolean NativeCompressionLibraryLoaded = false;
  static String LoadInfo = "";

  static {
    try {
      System.loadLibrary("compressions");
      NativeCompressionLibraryLoaded = true;
    } catch (Exception e) {
      String ldpath = System.getenv("LD_LIBRARY_PATH");
      if (ldpath == null) {
        ldpath = "";
      }
      LoadInfo = 
          String.format(
              "load native compressions library failed: java.library.path=%s LD_LIBRARY_PATH=%s",
              System.getProperty("java.library.path", ""),
              ldpath);
    }
  }

  static void CheckNativeLibrary() {
    if (NativeCompressionLibraryLoaded == false) {
      throw new RuntimeException(LoadInfo);
    }
  }
}
