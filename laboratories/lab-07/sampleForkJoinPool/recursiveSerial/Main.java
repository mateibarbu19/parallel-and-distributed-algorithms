package sampleForkJoinPool.recursiveSerial;

import java.io.File;

public class Main {
    public static void show(final String path) {
        final File file = new File(path);
        if (file.isFile()) {
            System.out.println(file.getPath());
        } else if (file.isDirectory()) {
            final var files = file.listFiles();
            if (files != null) {
                for (final var f : files) {
                    show(f.getPath());
                }
            }
        }
    }

    public static void main(final String[] args) {
        show("files");
    }
}
