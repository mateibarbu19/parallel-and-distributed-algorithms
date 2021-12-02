package task8;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

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

        final ForkJoinPool forkJoinPool = new ForkJoinPool(Runtime.getRuntime().availableProcessors());
        final RecursiveTask<Integer> query = new BSTTask<>(binarySearchTree);
        forkJoinPool.execute(query);
        try {
            final Integer height = query.get();
            System.out.println(height);
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }
        forkJoinPool.shutdown();
    }
}
