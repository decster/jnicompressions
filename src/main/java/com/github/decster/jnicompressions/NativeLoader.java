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

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.DigestInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class NativeLoader {
  static boolean NativeCompressionLibraryLoaded = false;
  static String LoadInfo = "";

  /**
   * Computes the MD5 value of the input stream Copy from
   * snappy-java(code.google.com/p/snappy-java/)
   * @param input
   * @return
   * @throws IOException
   * @throws NoSuchAlgorithmException
   */
  static String md5sum(InputStream input) throws IOException {
    BufferedInputStream in = new BufferedInputStream(input);
    try {
      MessageDigest digest = java.security.MessageDigest.getInstance("MD5");
      DigestInputStream digestInputStream = new DigestInputStream(in, digest);
      for (; digestInputStream.read() >= 0;) {

      }
      ByteArrayOutputStream md5out = new ByteArrayOutputStream();
      md5out.write(digest.digest());
      return md5out.toString();
    } catch (NoSuchAlgorithmException e) {
      throw new IllegalStateException("MD5 algorithm is not available: " + e);
    } finally {
      in.close();
    }
  }

  /**
   * Copy from snappy-java(code.google.com/p/snappy-java/)
   * @param libFolderForCurrentOS
   * @param libraryFileName
   * @param targetFolder
   * @return
   */
  private static File extractLibraryFile(String libFolderForCurrentOS,
      String libraryFileName, String targetFolder) {
    String nativeLibraryFilePath = libFolderForCurrentOS + "/" + libraryFileName;
    File extractedLibFile = new File(targetFolder, libraryFileName);

    try {
      if (extractedLibFile.exists()) {
        // test md5sum value
        String md5sum1 = md5sum(NativeLoader.class
            .getResourceAsStream(nativeLibraryFilePath));
        String md5sum2 = md5sum(new FileInputStream(extractedLibFile));

        if (md5sum1.equals(md5sum2)) {
          return extractedLibFile;
        } else {
          // remove old native library file
          boolean deletionSucceeded = extractedLibFile.delete();
          if (!deletionSucceeded) {
            throw new IOException("failed to remove existing native library file: "
                + extractedLibFile.getAbsolutePath());
          }
        }
      }

      // Extract a native library file into the target directory
      InputStream reader = NativeLoader.class
          .getResourceAsStream(nativeLibraryFilePath);
      FileOutputStream writer = new FileOutputStream(extractedLibFile);
      byte[] buffer = new byte[8192];
      int bytesRead = 0;
      while ((bytesRead = reader.read(buffer)) != -1) {
        writer.write(buffer, 0, bytesRead);
      }

      writer.close();
      reader.close();

      // Set executable (x) flag to enable Java to load the native library
      if (!System.getProperty("os.name").contains("Windows")) {
        try {
          Runtime
              .getRuntime()
              .exec(
                  new String[] { "chmod", "755", extractedLibFile.getAbsolutePath() })
              .waitFor();
        } catch (Throwable e) {
        }
      }

      return extractedLibFile;
    } catch (IOException e) {
      e.printStackTrace(System.err);
      return null;
    }
  }

  static {
    String os = System.getProperty("os.name").toLowerCase();
    String arch = System.getProperty("os.arch").toLowerCase();
    if (os.contains("windows")) {
      os = "windows";
    } else if (os.contains("linux")) {
      os = "linux";
    } else if (os.contains("mac")) {
      os = "mac";
    }
    if (arch.contains("amd64") || arch.contains("x86_64")) {
      arch = "x86_64";
    } else {
      arch = "i386";
    }
    String libraryName = System.mapLibraryName("compressions");
    String nativeLibDir = "/com/github/decster/jnicompressions/native/" + os + "/"
        + arch;
    String nativeLibPath = nativeLibDir + "/" + libraryName;
    if (NativeLoader.class.getResource(nativeLibPath) != null) {
      // Temporary library folder. Use the value of native.library.temp.dir or java.io.tmpdir
      String tempFolder = new File(System.getProperty("native.library.temp.dir",
          System.getProperty("java.io.tmpdir"))).getAbsolutePath();
      // Extract and load a native library inside the jar file
      File nativeLibFile = extractLibraryFile(nativeLibDir, libraryName, tempFolder);
      try {
        System.load(nativeLibFile.getAbsolutePath());
        NativeCompressionLibraryLoaded = true;
      } catch (Exception e) {
        e.printStackTrace();
      }
    }
    if (NativeCompressionLibraryLoaded == false) {
      LoadInfo = String
          .format(
              "load native compressions library failed: os.name=%s os.arch=%s nativeLibPath=%s",
              System.getProperty("os.name"), System.getProperty("os.arch"),
              nativeLibPath);
    }
  }

  static void CheckNativeLibrary() {
    if (NativeCompressionLibraryLoaded == false) {
      throw new RuntimeException(LoadInfo);
    }
  }
  
  public static void main(String[] args) {
    CheckNativeLibrary();
    System.out.println(new File(System.getProperty("native.library.temp.dir",
          System.getProperty("java.io.tmpdir"))).getAbsolutePath());
    System.out.println("jnicompressions native library loaded");
  }
}
