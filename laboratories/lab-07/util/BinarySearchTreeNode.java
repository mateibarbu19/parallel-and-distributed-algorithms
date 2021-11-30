package util;

public class BinarySearchTreeNode<T extends Comparable<T>> {
    private T value;
    private BinarySearchTreeNode<T> left;
    private BinarySearchTreeNode<T> right;

    public BinarySearchTreeNode(final T value) {
        this.value = value;
    }

    public void setValue(final T value) {
        this.value = value;
    }

    public T getValue() {
        return value;
    }

    public BinarySearchTreeNode<T> setLeft(final BinarySearchTreeNode<T> left) {
        this.left = left;
        return this;
    }

    public BinarySearchTreeNode<T> setRight(final BinarySearchTreeNode<T> right) {
        this.right = right;
        return this;
    }

    public BinarySearchTreeNode<T> getLeft() {
        return left;
    }

    public BinarySearchTreeNode<T> getRight() {
        return right;
    }
}
