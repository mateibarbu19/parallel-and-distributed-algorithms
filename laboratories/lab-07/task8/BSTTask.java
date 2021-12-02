package task8;

import java.util.concurrent.RecursiveTask;

import util.BinarySearchTreeNode;

public class BSTTask<T extends Comparable<T>> extends RecursiveTask<Integer> {
    private final BinarySearchTreeNode<T> binarySearchTree;

    /**
     * @param binarySearchTree
     */
    public BSTTask(final BinarySearchTreeNode<T> binarySearchTree) {
        this.binarySearchTree = binarySearchTree;
    }

    @Override
    protected Integer compute() {
        if (binarySearchTree == null) {
            return 0;
        }

        final RecursiveTask<Integer> left = new BSTTask<T>(binarySearchTree.getLeft());
        final RecursiveTask<Integer> right = new BSTTask<T>(binarySearchTree.getRight());

        left.fork();
        right.fork();

        return 1 + Math.max(
                left.join(),
                right.join());
    }

}
