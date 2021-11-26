package parallelTree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class ReadTreePart implements Runnable {
	TreeNode tree;
	String fileName;

	public ReadTreePart(final TreeNode tree, final String fileName) {
		this.tree = tree;
		this.fileName = fileName;
	}

	@Override
	public void run() {
		try {
			final Scanner scanner = new Scanner(new File(fileName));
			TreeNode treeNode;

			while (scanner.hasNextInt()) {
				final int child = scanner.nextInt();
				final int root = scanner.nextInt();

				treeNode = tree.getNode(root);
				while (treeNode == null) {
					treeNode = tree.getNode(root);
				}

				treeNode.addChild(new TreeNode(child));
			}
		} catch (final FileNotFoundException e) {
			e.printStackTrace();
		}
		Main.noVerify.release();
	}
}
