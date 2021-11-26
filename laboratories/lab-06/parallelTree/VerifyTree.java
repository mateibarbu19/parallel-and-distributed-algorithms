package parallelTree;

public class VerifyTree implements Runnable {
	TreeNode tree;

	public VerifyTree(final TreeNode tree) {
		this.tree = tree;
	}

	public boolean isCorrect(final TreeNode tree) {
		if (tree == null) {
			return true;
		}

		if (tree.name >= 64) {
			return true;
		}

		if (tree.left == null) {
			return false;
		}

		if (tree.right == null) {
			return false;
		}

		final boolean aux = ((tree.left.name + tree.right.name) == (tree.name * 4 + 1));
		return aux && isCorrect(tree.left) && isCorrect(tree.right);
	}

	@Override
	public void run() {
		Main.noVerify.acquireUninterruptibly(Main.NUMBER_OF_THREADS - 1);
		if (isCorrect(tree))
			System.out.println("Correct");
		else
			System.out.println("Wrong");

	}
}
