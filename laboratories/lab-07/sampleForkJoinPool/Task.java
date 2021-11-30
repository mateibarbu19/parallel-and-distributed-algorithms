package sampleForkJoinPool;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class Task extends RecursiveTask<Void> {
	private final String path;

	public Task(final String path) {
		this.path = path;
	}

	@Override
	protected Void compute() {
		final File file = new File(path);
		if (file.isFile()) {
			System.out.println(file.getPath());
			return null;
		} else if (file.isDirectory()) {
			final var files = file.listFiles();
			final List<Task> tasks = new ArrayList<>();
			if (files != null) {
				for (final var f : files) {
					final Task t = new Task(f.getPath());
					tasks.add(t);
					t.fork();
				}
			}
			for (final var task : tasks) {
				task.join();
			}
		}
		return null;
	}
}
