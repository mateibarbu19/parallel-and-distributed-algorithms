package example;

import java.util.concurrent.atomic.AtomicInteger;
 
public class Main {
    public static final int size = 1000000;
    public static final int noThreads = 8;
    public static int[] arr = new int[size];
    public static final Object lock = new Object();
 
    public static void main(final String[] args) {
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
 
        final Thread[] properThreads = new Thread[noThreads];
        final Thread[] atomicThreads = new Thread[noThreads];
 
        long startTime = System.nanoTime();
        for (int i = 0; i < properThreads.length; i++) {
            properThreads[i] = new ProperThread(i);
            properThreads[i].start();
        }
 
        for (final Thread properThread : properThreads) {
            try {
                properThread.join();
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
        }
        long stopTime = System.nanoTime();
        System.out.println("With locks = " + (stopTime - startTime));
 
        startTime = System.nanoTime();
        for (int i = 0; i < atomicThreads.length; i++) {
            atomicThreads[i] = new AtomicThread(i);
            atomicThreads[i].start();
        }
 
        for (final Thread atomicThread : atomicThreads) {
            try {
                atomicThread.join();
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
        }
        stopTime = System.nanoTime();
        System.out.println("With AtomicInteger = " + (stopTime - startTime));
 
        System.out.println("Atomic sum = " + AtomicThread.sum.get());
        System.out.println("Locking sum = " + ProperThread.sum);
    }
}
 
class AtomicThread extends Thread {
    public static AtomicInteger sum = new AtomicInteger(0);
    private final int id;
 
    public AtomicThread(final int id) {
        this.id = id;
    }
 
    @Override
    public void run() {
        final int start = id * (int) Math.ceil((double) Main.size / Main.noThreads);
        final int end = Math.min(Main.size, (id + 1) * (int) Math.ceil((double) Main.size / Main.noThreads));
        for (int i = start; i < end; i++) {
            sum.getAndAdd(Main.arr[i]);
        }
    }
}
 
class ProperThread extends Thread {
    public static int sum = 0;
    private final int id;
 
    public ProperThread(final int id) {
        this.id = id;
    }
 
    @Override
    public void run() {
        final int start = id * (int) Math.ceil((double) Main.size / Main.noThreads);
        final int end = Math.min(Main.size, (id + 1) * (int) Math.ceil((double) Main.size / Main.noThreads));
        for (int i = start; i < end; i++) {
            synchronized (Main.lock) {
                sum += Main.arr[i];
            }
        }
    }
}

 
