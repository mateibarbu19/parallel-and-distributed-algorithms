package task7;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

import util.BSTOperations;
import util.BinarySearchTreeNode;

public class Main {
    public static void main(final String[] args) {
        BinarySearchTreeNode<Integer> binarySearchTree = new BinarySearchTreeNode<>(3);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 6);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 9);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 2);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 10);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 1);

        final ExecutorService tpe = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        final CompletableFuture<Integer> completableFuture = new CompletableFuture<>();
        final AtomicInteger counter = new AtomicInteger(0);
        counter.incrementAndGet();
        tpe.submit(new BSTRunnable<Integer>(binarySearchTree, 1, counter, completableFuture, tpe));

        try {
            final Integer value = completableFuture.get();
            if (value != null) {
                System.out.println("Found value: " + value);
            } else {
                System.out.println("Did not find value!");
            }
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
    }
}
