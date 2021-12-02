package task7;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

import util.BinarySearchTreeNode;

public class BSTRunnable<T extends Comparable<T>> implements Runnable {
    private final BinarySearchTreeNode<T> binarySearchTree;
    private final T value;
    private final AtomicInteger counter;
    private final CompletableFuture<T> completableFuture;
    private final ExecutorService tpe;

    /**
     * @param binarySearchTree
     * @param value
     * @param counter
     * @param completableFuture
     * @param tpe
     */
    public BSTRunnable(final BinarySearchTreeNode<T> binarySearchTree, final T value, final AtomicInteger counter,
            final CompletableFuture<T> completableFuture, final ExecutorService tpe) {
        this.binarySearchTree = binarySearchTree;
        this.value = value;
        this.counter = counter;
        this.completableFuture = completableFuture;
        this.tpe = tpe;
    }

    @Override
    public void run() {
        if (binarySearchTree != null) {
            if (value.equals(binarySearchTree.getValue())) {
                completableFuture.complete(binarySearchTree.getValue());
                tpe.shutdown();
            } else if (binarySearchTree.getValue().compareTo(value) > 0) {
                counter.incrementAndGet();
                final Runnable t = new BSTRunnable<T>(binarySearchTree.getLeft(), value, counter, completableFuture,
                        tpe);
                tpe.submit(t);
            } else {
                counter.incrementAndGet();
                final Runnable t = new BSTRunnable<T>(binarySearchTree.getRight(), value, counter, completableFuture,
                        tpe);
                tpe.submit(t);
            }
        }

        if (counter.decrementAndGet() == 0) {
            completableFuture.complete(null);
            tpe.shutdown();
        }
    }

}
