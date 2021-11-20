package readersWriters.readerPriority;

import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

public class Main {
    public static final int WRITERS = 6;
    public static final int READERS = 8;

    public static Integer currentReaders = 0;
    public static final boolean[] hasRead = new boolean[READERS];
    public static final boolean[] hasWritten = new boolean[WRITERS];

    // used to make sure threads try to go in the same to the shared zone
    // this is better for testing than a turnstile Sempahore
    public static CyclicBarrier barrier = new CyclicBarrier(READERS + WRITERS);
    public static Semaphore roomEmpty = new Semaphore (1);

    public static void main(final String[] args) throws InterruptedException {
        final Thread[] readers = new Reader[READERS];
        final Thread[] writers = new Writer[WRITERS];

        for (int i = 0; i < READERS; i++) {
            readers[i] = new Reader(i);
        }

        for (int i = 0; i < WRITERS; i++) {
            writers[i] = new Writer(i);
        }

        for (final var reader: readers) {
            reader.start();
        }

        for (final var writer: writers) {
            writer.start();
        }

        for (final var reader: readers) {
            reader.join();
        }

        for (final var writer: writers) {
            writer.join();
        }
    }
}
