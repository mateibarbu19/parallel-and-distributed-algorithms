package sampleExecutorServiceCompletableFuture.recursiveSerial;

import java.io.File;

public class Main {
    public static void findFile(final String path, final String filename) {
        final File file = new File(path);
        if (file.isFile()) {
            if (file.getName().equals(filename)) {
                System.out.println(file.getAbsolutePath());
            }
        } else if (file.isDirectory()) {
            final var files = file.listFiles();
            if (files != null) {
                for (final var f : files) {
                    findFile(f.getPath(), filename);
                }
            }
        }
    }

    public static void main(final String[] args) {
        findFile("files", "somefile.txt");
    }
}
