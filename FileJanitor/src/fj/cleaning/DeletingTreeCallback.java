package fj.cleaning;

import fj.util.FoundCallback;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class DeletingTreeCallback implements FoundCallback {

	private final TreeDeleter treeDeleter;
	public DeletingTreeCallback (final TreeDeleter treeDeleter) {
		this.treeDeleter = treeDeleter;
	}

	public void run (final Path file) {
		try {
			Files.walkFileTree(file, treeDeleter);
		}
		catch (final IOException io) {
			throw new RuntimeException(io);
		}
	}

}
